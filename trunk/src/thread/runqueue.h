#ifndef RUNQUEUE_H_INCLUDED
#define RUNQUEUE_H_INCLUDED

#include "thread.h"

typedef struct _runqueue_t* runqueue_t;

runqueue_t 	runqueue_init	();
int 		runqueue_isEmpty(runqueue_t rq);
void 		runqueue_push	(runqueue_t rq, thread_t thread);
thread_t 	runqueue_pop	(runqueue_t rq);
void		runqueue_free	(runqueue_t rq);

runqueue_t 	runqueue_init_safe	();
int 		runqueue_isEmpty_safe(runqueue_t rq);
void 		runqueue_push_safe	(runqueue_t rq, thread_t thread);
thread_t 	runqueue_pop_safe	(runqueue_t rq);
void		runqueue_free_safe	(runqueue_t rq);

#endif
