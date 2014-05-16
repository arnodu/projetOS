#ifndef USE_PTHREAD

#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include <valgrind/valgrind.h>
#include "runqueue.h"
#include "scheduler.h"
#include "thread_t.h"

#define THREAD_STACK_SIZE 64*1024

static scheduler sched = NULL;

struct _scheduler{
	thread_t running;   //Thread courant
	thread_t main_thread;
	ucontext_t context_detached;
	int context_detached_valgrind;
};

runqueue_t rq;

//Libère les ressources du scheduler (notament le thread main)
//Est appelé avant la fermeture du programme avec exit();
#include <stdio.h>
void sched_free()
{
	runqueue_free(rq);
	free(sched->context_detached.uc_stack.ss_sp);
	if(sched->running!=sched->main_thread)
		free(sched->main_thread);
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
	sched->running->waiting = NULL;

	sched->main_thread = sched->running;

	//Initialisation du contexte detached (pour libérer les piles au exit)
	getcontext(&sched->context_detached);
	sched->context_detached.uc_stack.ss_size = THREAD_STACK_SIZE;
	sched->context_detached.uc_stack.ss_sp = malloc(THREAD_STACK_SIZE);
	sched->context_detached.uc_link = NULL;
	sched->context_detached_valgrind = VALGRIND_STACK_REGISTER(sched->context_detached.uc_stack.ss_sp,
							sched->context_detached.uc_stack.ss_sp + sched->context_detached.uc_stack.ss_size);

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
	assert(sched->running->status == RUNNING || sched->running->status == TERMINATED || sched->running->status == WAITING);

	thread_t oldRunning = sched->running;
	sched->running = thread;
	if(oldRunning->status == RUNNING)
		oldRunning->status = READY;
	sched->running->status = RUNNING;
	swapcontext(&oldRunning->context,&thread->context);
}

int sched_waitThread(thread_t thread)
{
    if(thread->status != TERMINATED)
    {
        thread_t self = thread_self();
        self->status = WAITING;
        thread->waiting = self;
        if(0!=sched_schedule())
            return -1;
    }
    return 0;
}

void switch_to_main_stack()
{
	setcontext(&sched->main_thread->context);
}

//Demande au scheduler de swapper sur le prochain thread
//retourne 0 si tout s'est bien passé
int sched_schedule()
{
    thread_t thread;
    if(runqueue_isEmpty(rq))
    {
    	switch_to_main_stack();
		exit(0);
    }
    thread = runqueue_pop(rq);
    sched_switchToThread(thread);
    return 0;
}

void sched_detach_and_schedule_f()
{
	thread_t self = thread_self();
	if(self->stack!=NULL)
	{
		VALGRIND_STACK_DEREGISTER(self->valgrind_stackid);
		free(self->stack);
	}
	sched_schedule();
}

void sched_detach_and_schedule()
{
	if(thread_self()->stack!=NULL)
	{//Si on est pas dans le contexte main, il faut aller dans un autre contexte pour libérer la pile
		makecontext(&sched->context_detached,sched_detach_and_schedule_f,0);
		swapcontext(&sched->running->context, &sched->context_detached);
	}
	else//Si on est le main il n'y a rien à faire de plus que réeordonnancer
		sched_schedule();
}

#endif
