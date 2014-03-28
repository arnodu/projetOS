#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <ucontext.h>

#include "thread.h"

typedef struct _scheduler* scheduler;

//dit si le scheduler a été initialisé
int sched_initialized(scheduler sched);

//Initialise le scheduler
//Retourne 0 si succes
int sched_init(scheduler* sched);

//Retourne le thread qui est actuellement éxécuté
//NULL si erreur
thread_t sched_runningThread(scheduler sched);

//Retourne le contexte sur lequel il faut switcher quand un context termine
//NULL si erreur
ucontext_t* sched_onTerminateContext(scheduler sched);

//Ajoute une tache à ordonnanceur
//0 si tout s'est bien passé
int sched_addThread(scheduler sched, thread_t thread);

//Demande au scheduler de swapper sur le prochain thread
//retourne 0 si tout s'est bien passé
int sched_schedule(scheduler sched);

#endif
