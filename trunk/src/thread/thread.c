#include <stdlib.h>

#include "thread.h"
#include "thread_t.h"
#include "scheduler.h"

#ifndef NDEBUG
#include <valgrind/valgrind.h>
#endif

#define THREAD_STACK_SIZE 64*1024

//le scheduler utilisé par tous les threads
static scheduler sched = NULL;

thread_t thread_self(void)
{
	sched_init(&sched);
	return sched_runningThread(sched);
}

//Initialise un thread vide
thread_t thread_s_init()
{
	thread_t thread = malloc(sizeof(struct _thread_t));
	if(thread==NULL)//Erreur: malloc
		return NULL;
	//Init de l'etat
	thread->status = INIT;
	thread->retval = NULL;

	//Init de la stack
	thread->stack = malloc(THREAD_STACK_SIZE);
	if(thread->stack == NULL)
	{//Erreur: malloc
		free(thread);
		return NULL;
	}

	//Init du context (pile+link)
	int res = getcontext(&thread->context);
	if(res != 0)
	{//erreur:getContext
		free(thread->stack);
		free(thread);
		return NULL;
	}
	thread->context.uc_stack.ss_size = THREAD_STACK_SIZE;
	thread->context.uc_stack.ss_sp = thread->stack;
	thread->context.uc_link = NULL;
#ifndef NDEBUG
	//Pour valgrind
	VALGRIND_STACK_REGISTER(thread->context.uc_stack.ss_sp,
							thread->context.uc_stack.ss_sp + thread->context.uc_stack.ss_size);
 #endif

	return thread;
}

int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg)
{
	sched_init(&sched);
	if(sched == NULL)
		return -1;
	*newthread = thread_s_init();
	if(*newthread == NULL)//Erreur : thread_s_init
		return -1;

	sched_makecontext(sched, *newthread, func, funcarg);
	//makecontext(&(*newthread)->context, (void (*)(void)) func, 1, funcarg);
	(*newthread)->status = READY;

	int res = sched_addThread(sched, *newthread);
	if(res != 0)//Erreur: sched_addThread
		return -1;

	return 0;
}

int thread_yield(void)
{
	sched_init(&sched);
	int res;
	res = sched_addThread(sched, thread_self());
	if(res != 0)//Erreur: sched_addThread
		return -1;
	res = sched_schedule(sched);
	if(res != 0)//Erreur: sched_addThread
		return -1;
	return 0;
}

int thread_join(thread_t thread, void **retval)
{
	sched_init(&sched);
	while(thread->status != TERMINATED)
	{
		if(0!=thread_yield())
			return -1;
	}

	if(retval != NULL)
	{
		*retval=thread->retval;
	}

	return 0;
}

void thread_exit(void *retval)
{
	sched_init(&sched);
	//Modification du statut du thread courant
	thread_t thread = thread_self();
	thread->retval = retval;
	thread->status = TERMINATED;

	//On de mande au scheduler de changer de thread sans rajouter celui ci
	sched_schedule(sched);
}
