#include <stdlib.h>
#include <ucontext.h>

#include "scheduler.h"
#include "thread_t.h"

#define ONTERMINATE_STACK_SIZE 64*1024

struct _scheduler{
	thread_t running;//Thread courant
	ucontext_t onTerminate;//Contient le contexte à switcher lorsqu'un contexte termine
};

//TODO: replacer par une vraie file
thread_t runqueue[5];
int rq_begin;
int rq_end;

//Fonction appelée losqu'un thread termine
static void onTerminate_f(scheduler sched)
{
	while(1)
	{
		sched->running->status = TERMINATED;
		sched->running->retval = sched->running->context.uc_stack.ss_sp;
		sched_schedule(sched);
	}
}

//dit si le scheduler a été initialisé
int sched_initialized(scheduler sched)
{
	return sched!=NULL;
}

//Initialise le scheduler
//Retourne 0 si succes
int sched_init(scheduler* sched)
{
	//Alloc de la structure
	scheduler s = *sched =malloc(sizeof(struct _scheduler));

	//Initialisation du thread courant
	s->running = thread_s_init();
	s->running->status = READY;

	//Initialisation de onTerminate
	getcontext(&s->onTerminate);
	s->onTerminate.uc_stack.ss_size = ONTERMINATE_STACK_SIZE;
	s->onTerminate.uc_stack.ss_sp = malloc(ONTERMINATE_STACK_SIZE);
	s->onTerminate.uc_link = NULL;
	makecontext(&s->onTerminate, (void (*)(void))onTerminate_f, 1, s);
	return 0;
}

//Retourne le thread qui est actuellement éxécuté
//NULL si erreur
thread_t sched_runningThread(scheduler sched)
{
	return sched->running;
}

//Retourne le contexte sur lequel il faut switcher quand un context termine
//NULL si erreur
ucontext_t* sched_onTerminateContext(scheduler sched)
{
	return &sched->onTerminate;
}

//Ajoute une tache à ordonnanceur
//0 si tout s'est bien passé
int sched_addThread(scheduler sched, thread_t thread)
{
	//TODO: replacer par une vraie file
	runqueue[rq_end++]=thread;
	if(rq_end>=5)
		rq_end=0;
	return 0;
}

static void sched_switchToThread(scheduler sched, thread_t thread)
{
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
	if(rq_begin>=5)
		rq_begin = 0;
	sched_switchToThread(sched, thread);
	return 0;
}
