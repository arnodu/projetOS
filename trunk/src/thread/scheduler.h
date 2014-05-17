#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <ucontext.h>

#include "thread.h"

//Initialise le scheduler
//Retourne 0 si succes
int sched_init();

//Retourne le thread qui est actuellement éxécuté
//NULL si erreur
thread_t sched_runningThread();

//Ajoute une tache à ordonnanceur
//0 si tout s'est bien passé
int sched_addThread(thread_t thread);

//Fait en sorte que le thread courant attende thread
//Retourne 0 si tou s'est bien passé
int sched_waitThread(thread_t thread);

//Demande au scheduler de swapper sur le prochain thread
//retourne 0 si tout s'est bien passé
int sched_schedule();

//Appelle le scheduler dans un contexte à part
void sched_detach_and_schedule();

#endif
