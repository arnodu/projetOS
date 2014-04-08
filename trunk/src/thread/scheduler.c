#ifndef USE_PTHREAD

#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>

#include "scheduler.h"
#include "thread_t.h"

static scheduler sched = NULL;

struct _scheduler{
	thread_t running;//Thread courant
	thread_t main_thread;//Thread main: c'est le thread courant au moment de sched_init (on supposera que init est appelée pour la 1ere fois dans le main)
};

#define RQ_SIZE 1000
//TODO: replacer par une vraie file
thread_t runqueue[RQ_SIZE];
int rq_begin;
int rq_end;

//Libère les ressources du scheduler (notament le thread main)
//Est appelé avant la fermeture du programme avec exit();
void sched_free()
{
	free(sched->main_thread);
	free(sched);
}

//Initialise le scheduler
//Retourne 0 si succes
int sched_init()
{
	if(sched!=NULL)
		return 0;
	//Alloc de la structure
	sched = malloc(sizeof(struct _scheduler));

	//Initialisation du thread courant (main)
	sched->running = malloc(sizeof(struct _thread_t));
	sched->running->retval = NULL;
	getcontext(&sched->running->context);
	sched->running->status = RUNNING;

	sched->main_thread=sched->running;

	//Enregistrement de la fonction de libération à la fermeture du programme
	atexit(sched_free);

	return 0;
}

//Fonction appelée à la création d'un thread
//Wrap la fonction donnée en paramètre pour que la fonction retourne bien avec thread_exit
static void thread_f(void* (*func)(void*), void* funcarg)
{
	void* res = func(funcarg);
	thread_exit(res);
}

//fait makecontext sur le contexte du thread donné pour qu'il utilise la fonction func
//si func termine, thread_exit est appelé avec la valeur de retour (voir thread_f)
int sched_makecontext(thread_t thread, void* (*func)(void*), void* funcarg)
{
	makecontext(&thread->context, (void (*)(void)) thread_f,  2, func, funcarg);
}

//Retourne le thread qui est actuellement éxécuté
//NULL si erreur
thread_t sched_runningThread()
{
	return sched->running;
}

//Ajoute une tache à ordonnanceur
//0 si tout s'est bien passé
int sched_addThread(thread_t thread)
{
	//TODO: replacer par une vraie file
	runqueue[rq_end++]=thread;
	if(rq_end>=RQ_SIZE)
		rq_end=0;
	return 0;
}

//Passe du thread courant au thread donné
static void sched_switchToThread(thread_t thread)
{
	assert(thread->status == READY);
	assert(sched->running->status == RUNNING || sched->running->status == TERMINATED);
	thread_t oldRunning = sched->running;
	sched->running = thread;
	if(oldRunning->status == RUNNING)
		oldRunning->status = READY;
	sched->running->status = RUNNING;
	swapcontext(&oldRunning->context,&thread->context);
}

//Demande au scheduler de swapper sur le prochain thread
//retourne 0 si tout s'est bien passé
int sched_schedule()
{
	//TODO: replacer par une vraie file
	thread_t thread = runqueue[rq_begin++];
	if(rq_begin>=RQ_SIZE)
		rq_begin = 0;
	sched_switchToThread(thread);
	return 0;
}

#endif