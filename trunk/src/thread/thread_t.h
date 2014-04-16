#ifndef __THREAD_T_H__
#define __THREAD_T_H__

#include <ucontext.h>
#include <sys/queue.h>

typedef struct _thread_t* thread_t;

struct _thread_t{
	ucontext_t 	context;
	void* 		stack;
	void* 		retval;
	enum {INIT, READY, RUNNING, TERMINATED}
				status;
	int valgrind_stackid;

	CIRCLEQ_ENTRY(_thread_t) entries;
};

//Initialise un thread vide
thread_t thread_s_init();

#endif
