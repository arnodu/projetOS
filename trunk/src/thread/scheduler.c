#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>

#include "scheduler.h"
#include "thread_t.h"

struct _scheduler{
	thread_t running;//Thread courant
};

#define RQ_SIZE 1000
//TODO: replacer par une vraie file
thread_t runqueue[RQ_SIZE];
int rq_begin;
int rq_end;

void thread_f(scheduler sched,void* (*func)(void*), void* funcarg)
{
	void* res = func(funcarg);
	thread_exit(res);
}

//Initialise le scheduler
//Retourne 0 si succes
int sched_init(scheduler* sched)
{
	if(*sched!=NULL)
		return 0;
	//Alloc de la structure
	scheduler s = *sched =malloc(sizeof(struct _scheduler));

	//Initialisation du thread courant
	s->running = thread_s_init();
	s->running->status = READY;

	return 0;
}

int sched_makecontext(scheduler sched, thread_t thread, void* (*func)(void*), void* funcarg)
{
	makecontext(&thread->context, (void (*)(void)) thread_f,  3, sched, func, funcarg);
}

//Retourne le thread qui est actuellement éxécuté
//NULL si erreur
thread_t sched_runningThread(scheduler sched)
{
	return sched->running;
}

//Ajoute une tache à ordonnanceur
//0 si tout s'est bien passé
int sched_addThread(scheduler sched, thread_t thread)
{
	//TODO: replacer par une vraie file
	runqueue[rq_end++]=thread;
	if(rq_end>=RQ_SIZE)
		rq_end=0;
	return 0;
}

static void sched_switchToThread(scheduler sched, thread_t thread)
{
	assert(thread->status == READY);
	thread_t oldRunning = sched->running;
	sched->running = thread;
	swapcontext(&oldRunning->context,&thread->context);
}

//Demande au scheduler de swapper sur le prochain thread
//retourne 0 si tout s'est bien passé
int sched_schedule(scheduler sched)
{
	//TODO: replacer par une vraie file
	thread_t thread = runqueue[rq_begin++];
	if(rq_begin>=RQ_SIZE)
		rq_begin = 0;
	sched_switchToThread(sched, thread);
	return 0;
}
