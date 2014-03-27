#include "thread_t.h"
#include "ordo.h"

#include <stdio.h>
#include <assert.h>

ucontext_t uc_ordo;
int initialized = 0;

thread_t running;
thread_t threads[3];
int nthreads = 0;

void switchToThread(thread_t thread)
{
	thread_t oldRunning = running;
	running = thread;
	swapcontext(&oldRunning->ucontext,&running->ucontext);
}
int ordo_i = -1;
void ordo_func()
{
	ordo_i++;
	if(ordo_i>=3)
		ordo_i=0;
	sleep(1);
	printf("ordo: ->thread%d:%p\n",ordo_i,threads[ordo_i]);
	switchToThread(threads[ordo_i]);
}

void on_terminated()
{
	printf("Thread terminated: %p\n",running);
	running->terminated=1;
	ordo_func();
}

//Initialise l'ordonnanceur
int	ordo_init()
{
	if(!initialized)
	{
		getcontext(&uc_ordo);
		uc_ordo.uc_stack.ss_size = 64*1024;
		uc_ordo.uc_stack.ss_sp = malloc(uc_ordo.uc_stack.ss_size);
		uc_ordo.uc_link = NULL;
		makecontext(&uc_ordo, on_terminated, 0);

		running = malloc(sizeof(struct thread_t));
		getcontext(&running->ucontext);
		running->ucontext.uc_link = NULL;
		running->terminated=0;
		initialized = 1;
	}
}
//Retourne l'ucontext à setcontext pour appelr l'ordonnanceur
ucontext_t* 	ordo_getUC()
{
	return &uc_ordo;
}
//Ajoute un thread à ordonnanceur.
int ordo_addThread(thread_t thread)
{
	threads[nthreads] = thread;
	nthreads++;
	if(nthreads>=3)
	{
		nthreads = 0;
	}
}

thread_t ordo_getRunningThread()
{
	return running;
}

//Change de contexte pour passer la main à l'ordonnanceur (n'ajoute pas le thread courant à l'ordo)
void ordo_execute()
{
	ordo_func();
}
