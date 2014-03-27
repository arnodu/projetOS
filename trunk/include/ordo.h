#ifndef __ORDO_H__
#define __ORDO_H__

#include <ucontext.h>
#include "thread.h"

//Initialise l'ordonnanceur
int				ordo_init();
//Retourne l'ucontext à setcontext pour appelr l'ordonnanceur
ucontext_t* 	ordo_getUC();
//Ajoute un thread à ordonnanceur.
int 			ordo_addThread(thread_t thread);
//Change de contexte pour passer la main à l'ordonnanceur (n'ajoute pas le thread courant à l'ordo)
void  			ordo_execute();

thread_t		ordo_getRunningThread();

#endif
