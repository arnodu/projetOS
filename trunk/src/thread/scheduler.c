#ifndef USE_PTHREAD

#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include <sys/queue.h>
#include "scheduler.h"
#include "thread_t.h"

static scheduler sched = NULL;

struct _scheduler{
	thread_t running;   //Thread courant
	thread_t main_thread;
	
};

typedef struct queue
{
	thread_t curr;
	CIRCLEQ_ENTRY(queue) prev_next_thread; 
} queue;

CIRCLEQ_HEAD(queue_head_last, queue) queue_head_last;

struct queue_head_last* _scheduler_head_last;

//Libère les ressources du scheduler (notament le thread main)
//Est appelé avant la fermeture du programme avec exit();
void sched_free()
{	queue* curr;
	CIRCLEQ_FOREACH(curr, _scheduler_head_last, prev_next_thread){		
		CIRCLEQ_REMOVE(_scheduler_head_last, curr, prev_next_thread);
	}	
	free(_scheduler_head_last);
	free(sched->main_thread);
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

	_scheduler_head_last = malloc(sizeof(struct queue_head_last));

	CIRCLEQ_INIT(_scheduler_head_last);
	
	//Initialisation du thread courant (main)
	sched->running = malloc(sizeof(struct _thread_t));
	sched->running->retval = NULL;
	getcontext(&sched->running->context);
	sched->running->status = RUNNING;
	sched->running->stack = NULL;
	sched->running->context.uc_link = NULL;

	sched->main_thread=sched->running;

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
	
	queue* q = malloc(sizeof(queue));
	q->curr = thread;
	CIRCLEQ_INSERT_TAIL(_scheduler_head_last, q, prev_next_thread);	
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
	//TODO: replacer par une vraie file
	if(CIRCLEQ_EMPTY(_scheduler_head_last))
		exit(0);
	queue* s = CIRCLEQ_FIRST(_scheduler_head_last);
	thread_t thread = s->curr;
	CIRCLEQ_REMOVE(_scheduler_head_last, s,prev_next_thread);	
	free(s);
	sched_switchToThread(thread);
	return 0;
}

#endif
