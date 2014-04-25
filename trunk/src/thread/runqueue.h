#ifndef RUNQUEUE_H_INCLUDED
#define RUNQUEUE_H_INCLUDED

#include "thread.h"

typedef struct _runqueue_t* runqueue_t;

inline runqueue_t 	runqueue_init	();
inline int 		runqueue_isEmpty(runqueue_t rq);
inline void 		runqueue_push	(runqueue_t rq, thread_t thread);
inline thread_t 	runqueue_pop	(runqueue_t rq);
inline void		runqueue_free	(runqueue_t rq);

#endif
