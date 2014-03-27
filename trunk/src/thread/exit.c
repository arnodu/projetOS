#include "ordo.h"
#include "thread.h"
#include "thread_t.h"
#include <stdio.h>
void thread_exit(void *retval)
{
	ucontext_t* uc = &thread_self()->ucontext;
	//La pile courante n'est plus utile
	uc->uc_stack.ss_flags = SS_DISABLE;
	//Elle est remplacée par la valeur de retour voulue
	//warning:la taille de la pile ne correspond plus à la mémoir allouée
	uc->uc_stack.ss_sp = retval;
	//On execute l'ordo sans avoir ajouté le thread courant pour ne jamais y retourner
	printf("exit:%p -> return:%s\n",thread_self(),(char*)uc->uc_stack.ss_sp);
	thread_self()->terminated=1;
	ordo_execute();
}
