#ifndef USE_PTHREAD

#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "runqueue.h"
#include "scheduler.h"
#include "thread_t.h"

static scheduler sched = NULL;

struct _scheduler{
	thread_t running;   //Thread courant
};

runqueue_t rq;

//Libère les ressources du scheduler (notament le thread main)
//Est appelé avant la fermeture du programme avec exit();
#include <stdio.h>
void sched_free()
{
	//if(sched->running->stack!=NULL) //si on est pas dans le thread main
		//free(sched->running->stack);
	runqueue_free(rq);
	free(sched->running);
	free(sched);
}

//Initialise le scheduler
//Retourne 0 si succes
int sched_init()
{
	if(sched != NULL)
		return 0;
	//Alloc de la structure
	sched = malloc(sizeof(struct _scheduler));

	rq = runqueue_init();

	//Initialisation du thread courant (main)
	sched->running = malloc(sizeof(struct _thread_t));
	sched->running->retval = NULL;
	getcontext(&sched->running->context);
	sched->running->status = RUNNING;
	sched->running->stack = NULL;
	sched->running->context.uc_link = NULL;

	//Enregistrement de la fonction de libération à la fermeture du programme
	atexit(sched_free);

	return 0;
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
	runqueue_push(rq,thread);
	return 0;
}

//Passe du thread courant au thread donné
static void sched_switchToThread(thread_t thread)
{
	assert(thread->status == READY || (thread == sched->running && thread->status == RUNNING));
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
    thread_t thread;
    if(runqueue_isEmpty(rq))
        exit(0);
    thread = runqueue_pop(rq);
    sched_switchToThread(thread);
    return 0;
}

#endif
