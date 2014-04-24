#ifndef __THREAD_T_H__
#define __THREAD_T_H__

#include <ucontext.h>
#include <sys/queue.h>

typedef struct _thread_t{
	ucontext_t 	context;
	void* 		stack;
	void* 		retval;
	enum {INIT, READY, RUNNING, TERMINATED}
				status;
	int valgrind_stackid;
	thread_t waiting;

	CIRCLEQ_ENTRY(_thread_t) entries;
}* thread_t;

//Initialise un thread vide
thread_t thread_s_init();

#endif
