#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h> /* ne compile pas avec -std=c89 ou -std=c99 */

ucontext_t thread_main, thread1, scheduler;

void thread1_f(int numero)
{
  printf("thread1:j'affiche le numéro %d\n", numero);
}

void scheduler_f()
{
	printf("Entre dans le sheduler 1:\n");
	printf("\tscheduler decide d'executer main\n");
	swapcontext(&scheduler,&thread_main);
	printf("Entre dans le sheduler 2:\n");
	printf("\tscheduler decide d'executer func\n");
	swapcontext(&scheduler,&thread1);
	printf("Entre dans le sheduler 3:\n");
	printf("\tscheduler decide d'executer main\n");
	swapcontext(&scheduler,&thread_main);
	printf("le scheduler termine\n");
}

int main() {
	getcontext(&thread_main);
	getcontext(&scheduler); /* initialisation de uc avec valeurs coherentes
		    * (pour éviter de tout remplit a la main ci-dessous) */

	scheduler.uc_stack.ss_size = 64*1024;
	scheduler.uc_stack.ss_sp = malloc(thread1.uc_stack.ss_size);
	scheduler.uc_link = NULL;
	makecontext(&scheduler, (void (*)(void)) scheduler_f, 1, 34);
	getcontext(&thread1); /* initialisation de uc avec valeurs coherentes
		    * (pour éviter de tout remplit a la main ci-dessous) */

	thread1.uc_stack.ss_size = 64*1024;
	thread1.uc_stack.ss_sp = malloc(thread1.uc_stack.ss_size);
	thread1.uc_link = &scheduler;
	makecontext(&thread1, (void (*)(void)) thread1_f, 1, 34);
	printf("Swap 1: main->sheduler\n");
	swapcontext(&thread_main,&scheduler);
	printf("Swap 2: main->sheduler\n");
	swapcontext(&thread_main,&scheduler);
	printf("main:fin\n");
	return 0;
}
