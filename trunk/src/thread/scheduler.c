#ifndef USE_PTHREAD

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include <valgrind/valgrind.h>
#include "runqueue.h"
#include "scheduler.h"
#include "thread_t.h"

#define THREAD_STACK_SIZE 64*1024
#define NUM_CPU sysconf( _SC_NPROCESSORS_ONLN )

// static scheduler sched[NUM_CPU];
static scheduler* sched = NULL;

thread_t main_thread;

struct _scheduler{
	thread_t running;   //Thread courant
	ucontext_t context_detached;
	int context_detached_valgrind;
};

runqueue_t rq;

//Libère les ressources du scheduler (notament le thread main)
//Est appelé avant la fermeture du programme avec exit();
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
int sched_init_clone(){
	main_thread = malloc(sizeof(struct _thread_t));
	main_thread->retval = NULL;
	getcontext(&main_thread->context);
	main_thread->status = RUNNING;
	main_thread->stack = NULL;
	main_thread->context.uc_link = NULL;
	main_thread->waiting = NULL;

	sched = malloc(sizeof(scheduler)*NUM_CPU);

 	for(int i=1; i<NUM_CPU; i++){
		void * stack = malloc(THREAD_STACK_SIZE);
		clone(sched_init, stack+THREAD_STACK_SIZE,  CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &sched[i]); 		
	}	
	sched_init(sched[0]);
}

int sched_init(scheduler* sched)
{
	if(sched != NULL)
		return 0;
	//Alloc de la structure
	
	rq = runqueue_init();

	//Initialisation du thread courant (main)

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
