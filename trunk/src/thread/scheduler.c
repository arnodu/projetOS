#ifndef USE_PTHREAD

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>
#include <assert.h>
#include <linux/sched.h>
#include <valgrind/valgrind.h>
#include <sys/syscall.h>

#include "runqueue.h"
#include "scheduler.h"
#include "thread_t.h"

#define THREAD_STACK_SIZE 64*1024
#define NUM_CPU 2
//sysconf( _SC_NPROCESSORS_ONLN )

typedef struct _scheduler* scheduler;

static struct _scheduler* schedTab = NULL;
int* tidTab = NULL;

thread_t main_thread;

struct _scheduler{
	thread_t running;   //Thread courant
	ucontext_t context_detached;
	int context_detached_valgrind;
};

runqueue_t rq;

static int get_corenum()
{
	int tid = syscall(SYS_gettid);//gettid
	int i;
	for(i=0;i<NUM_CPU;i++)
	{
		if(tid==tidTab[i])
			return i;
	}
	assert(0);
}

void sched_clean()
{
	int i;
	for(i=0;i<NUM_CPU;i++)
	{
		scheduler sched = &schedTab[get_corenum()];

		free(sched->context_detached.uc_stack.ss_sp);
		if(sched->running!=main_thread)
			free(sched->running);
	}

	runqueue_free(rq);
	free(main_thread);
	free(schedTab);
	free(tidTab);
}

static int sched_init_core(int corenum)
{
	tidTab[corenum] = syscall(SYS_gettid);//gettid

	scheduler s = &schedTab[corenum];

	//Initialisation du contexte detached (pour libérer les piles au exit)
	getcontext(&s->context_detached);
	s->context_detached.uc_stack.ss_size = THREAD_STACK_SIZE;
	s->context_detached.uc_stack.ss_sp = malloc(THREAD_STACK_SIZE);
	s->context_detached.uc_link = NULL;
	s->context_detached_valgrind = VALGRIND_STACK_REGISTER(s->context_detached.uc_stack.ss_sp,
							s->context_detached.uc_stack.ss_sp + s->context_detached.uc_stack.ss_size);

	if(corenum==0)
		s->running = main_thread;
	else
	{
		s->running = NULL;
		sched_schedule();
	}

	return 0;
}

//Initialise le scheduler
//Retourne 0 si succes
int sched_init(){
	if(schedTab!=NULL)
		return 0;

	rq = runqueue_init();

	main_thread = malloc(sizeof(struct _thread_t));
	main_thread->retval = NULL;
	getcontext(&main_thread->context);
	main_thread->status = RUNNING;
	main_thread->stack = NULL;
	main_thread->context.uc_link = NULL;
	main_thread->waiting = NULL;

	assert(schedTab==NULL && tidTab==NULL);
	schedTab = malloc(sizeof(struct _scheduler)*NUM_CPU);
	tidTab = malloc(sizeof(int)*NUM_CPU);
	int i;
	for(i=1; i<NUM_CPU; i++){
		tidTab[i]=-1;
	}

	atexit(sched_clean);

 	for(i=1; i<NUM_CPU; i++){
 		//TODO se souvenir des stacks pour les libérer
		void * stack = malloc(THREAD_STACK_SIZE);
		int res = clone(sched_init_core, stack+THREAD_STACK_SIZE,  SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*) i);
		assert(res!=-1);
	}
	sched_init_core(0);

	return 0;
}

//Retourne le thread qui est actuellement éxécuté
//NULL si erreur
thread_t sched_runningThread()
{
	return schedTab[get_corenum()].running;
}

//Ajoute une tache à ordonnanceur
//0 si tout s'est bien passé
int sched_addThread(thread_t thread)
{
	runqueue_push(rq,thread);
	return 0;
}

//Passe du thread courant au thread donné
static void sched_switchToThread(thread_t thread)
{
	scheduler sched = &schedTab[get_corenum()];

	assert(thread->status == READY || (thread == sched->running && thread->status == RUNNING));

	thread_t oldRunning = NULL;
	if(sched->running!=NULL)
	{
		assert(sched->running->status == RUNNING || sched->running->status == TERMINATED || sched->running->status == WAITING);
		oldRunning = sched->running;
		if(oldRunning->status == RUNNING)
			oldRunning->status = READY;
	}
	sched->running = thread;
	sched->running->status = RUNNING;
	if(oldRunning!=NULL)
		swapcontext(&oldRunning->context,&thread->context);
	else
		setcontext(&thread->context);
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
	setcontext(&main_thread->context);
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
	scheduler sched = &schedTab[get_corenum()];

	if(thread_self()->stack!=NULL)
	{//Si on est pas dans le contexte main, il faut aller dans un autre contexte pour libérer la pile
		makecontext(&sched->context_detached,(void(*)(void))sched_detach_and_schedule_f,0);
		swapcontext(&sched->running->context, &sched->context_detached);
	}
	else//Si on est le main il n'y a rien à faire de plus que réeordonnancer
		sched_schedule();
}

#endif
