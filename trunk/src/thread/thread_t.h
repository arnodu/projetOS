#ifndef __THREAD_T_H__
#define __THREAD_T_H__

#include <ucontext.h>

struct _thread_t{
	ucontext_t 	context;
	void* 		stack;
	void* 		retval;
	enum {INIT, READY, RUNNING, TERMINATED}
				status;
#ifndef DEBUG
	int valgrind_stackid;
#endif
};

//Initialise un thread vide
thread_t thread_s_init();

#endif
