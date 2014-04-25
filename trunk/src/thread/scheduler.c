#ifndef USE_PTHREAD

#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "runqueue.h"
#include "scheduler.h"
#include "thread_t.h"

#include <valgrind/valgrind.h>

static scheduler sched = NULL;

struct _scheduler{
	thread_t running;   //Thread courant
	thread_t main_thread;
	ucontext_t * main_context; // Ce champ ne sera rempli que si quelqu'un fait un join sur le main.
};

runqueue_t rq;

//Libère les ressources du scheduler (notament le thread main)
//Est appelé avant la fermeture du programme avec exit();
#include <stdio.h>

__attribute__((destructor))
void sched_free()
{
	//if(sched->running->stack!=NULL) //si on est pas dans le thread main
		//free(sched->running->stack);
	runqueue_free(rq);

	// Libération du dernier thread qui s'executait
	VALGRIND_STACK_DEREGISTER(sched->running->valgrind_stackid);
	free(sched->running->stack);
	free(sched->running);


	// Si le dernier thread qui s'executait n'était pas le main, alors il faut libérer le main aussi.
	if(sched->running != sched->main_thread)
	{
		free(sched->main_thread);
	}
	free(sched);
}

//Initialise le scheduler
//Retourne 0 si succes

__attribute__((constructor))
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
	sched->running->waiting = NULL;
	sched->running->context.uc_link = NULL;

	// Initialisation du contexte sortant
	sched->main_context = NULL;
	sched->main_thread = sched->running;


	//Enregistrement de la fonction de libération à la fermeture du programme
	//atexit(sched_free);

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
    do
    {
        if(runqueue_isEmpty(rq))
        {
        	// Execution terminée .
        	// On revient sur le contexte du main pour pouvoir libérer la dernière pile.
        	setcontext(sched->main_context);
            exit(0);
        }
        thread = runqueue_pop(rq);
        if(thread->waiting != NULL)
        {
            if(thread->waiting->status == TERMINATED)
            {
                thread->waiting = NULL;
            }
            else
            {
                runqueue_push(rq,thread);
            }
        }
    }
    while(thread->waiting != NULL);

	sched_switchToThread(thread);
	return 0;
}

void sched_save_main_context(thread_t main)
{
	sched->main_context = &main->context;
}


#endif
