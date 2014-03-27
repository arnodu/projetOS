#include "thread.h"
#include "thread_t.h"

#include <assert.h>
#include <stdio.h>

int thread_join(thread_t thread, void **retval)
{
	//Si l'execution du thread n'est pas terminée
	while(!thread->terminated)
	{
		thread_yield();//On donne la main à un autre thread
	}
	//Quand on a terminé, la valeur de la pile du thread est la valeur de retour
	if(retval!=NULL)
		*retval = thread->ucontext.uc_stack.ss_sp;
	printf("join:%p retourne %s\n",thread,(char*)thread->ucontext.uc_stack.ss_sp);

	return 0;
}
