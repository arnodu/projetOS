#include "thread.h"
#include "thread_t.h"
#include "ordo.h"

int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg)
{
	//Initialisation de l'ordonnanceur si il n'est aps encore initialisé
	ordo_init();

	//Allocation du thread
	thread_t thread = *newthread = malloc(sizeof(struct thread_t));
	ucontext_t* uc = &thread->ucontext;
	thread->terminated=0;

	//Fabrication du ucontext
	getcontext(uc);
	uc->uc_stack.ss_size = 64*1024;
	uc->uc_stack.ss_sp = malloc(uc->uc_stack.ss_size);
	uc->uc_link = ordo_getUC();
	makecontext(uc, (void (*)(void)) func, 1, funcarg);

	//Ajout du thread à l'ordo
	ordo_addThread(thread);

	//TODO gestion d'erreurs
	return 0;
}
