#ifndef USE_PTHREAD
#include <stdlib.h>

#include "thread.h"
#include "thread_t.h"
#include "scheduler.h"

#include <valgrind/valgrind.h>

#define THREAD_STACK_SIZE 64*1024

thread_t thread_self(void)
{
	sched_init();
	return sched_runningThread();
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
	//Pour valgrind
	thread->valgrind_stackid = VALGRIND_STACK_REGISTER(thread->context.uc_stack.ss_sp,
							thread->context.uc_stack.ss_sp + thread->context.uc_stack.ss_size);

	return thread;
}

//Fonction appelée à la création d'un thread
//Wrap la fonction donnée en paramètre pour que la fonction retourne bien avec thread_exit
static void function_wrapper(void* (*func)(void*), void* funcarg)
{
	void* res = func(funcarg);
	thread_exit(res);
}

//fait makecontext sur le contexte du thread donné pour qu'il utilise la fonction func
//si func termine, thread_exit est appelé avec la valeur de retour (voir function_wrapper)
int _makecontext(thread_t thread, void* (*func)(void*), void* funcarg)
{
	makecontext(&thread->context, (void (*)(void)) function_wrapper,  2, func, funcarg);
	// ATTENTION :  Makecontext retourne void... pourquoi la notre retourne un int?
	// Ajouté return 0
	return 0;
}

int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg)
{
	int res=sched_init();
	if(res != 0)//Erreur: sched_init
		return -1;
	*newthread = thread_s_init();
	if(*newthread == NULL)//Erreur : thread_s_init
		return -1;

	_makecontext(*newthread, func, funcarg);
	//makecontext(&(*newthread)->context, (void (*)(void)) func, 1, funcarg);
	(*newthread)->status = READY;

	res = sched_addThread(*newthread);
	if(res != 0)//Erreur: sched_addThread
		return -1;

	return 0;
}

int thread_yield(void)
{
	sched_init();
	int res;
	res = sched_addThread(thread_self());
	if(res != 0)//Erreur: sched_addThread
		return -1;
	res = sched_schedule();
	if(res != 0)//Erreur: sched_addThread
		return -1;
	return 0;
}

int thread_join(thread_t thread, void **retval)
{
	sched_init();
	while(thread->status != TERMINATED)
	{
		if(0!=thread_yield())
			return -1;
	}

	if(retval != NULL)
	{
		*retval=thread->retval;
	}
	if(thread->stack!=NULL)//si c'est pas le main
	{
		VALGRIND_STACK_DEREGISTER(thread->valgrind_stackid);
		free(thread->stack);
	}
	free(thread);

	return 0;
}

void thread_exit(void *retval)
{
	sched_init();
	//Modification du statut du thread courant
	thread_t thread = thread_self();
	thread->retval = retval;
	thread->status = TERMINATED;

	//On de mande au scheduler de changer de thread sans rajouter celui ci
	sched_schedule();
}

#endif
