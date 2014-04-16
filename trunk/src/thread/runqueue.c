#include "runqueue.h"
#include "thread_t.h"
#include <sys/queue.h>
#include <stdlib.h>
#include <assert.h>

//typedef struct _runqueue_entry
//{
//	thread_t thread;
//	CIRCLEQ_ENTRY(_runqueue_entry) entries;
//} *runqueue_entry;

//Def d'une liste circtulaire de type struct _runqueue_t
//contenant des entrées de type struct _runqueue_entry
CIRCLEQ_HEAD(_runqueue_t, _thread_t);


runqueue_t runqueue_init	()
{
	runqueue_t rq = malloc(sizeof(struct _runqueue_t));
	CIRCLEQ_INIT(rq);
	return rq;
}
int runqueue_isEmpty(runqueue_t rq)
{
	return CIRCLEQ_EMPTY(rq);
}
void runqueue_push	(runqueue_t rq, thread_t thread)
{
	//runqueue_entry entry = malloc(sizeof(struct _runqueue_entry));
	//entry->thread = thread;

	CIRCLEQ_INSERT_TAIL(rq, thread, entries);
}
thread_t runqueue_pop	(runqueue_t rq)
{
	thread_t e = CIRCLEQ_FIRST(rq);
	//thread_t thread = e->thread;
	CIRCLEQ_REMOVE(rq, e, entries);
	//free(e);
	return e;
}
void runqueue_free	(runqueue_t rq)
{
	assert(runqueue_isEmpty(rq));
	free(rq);

//	runqueue_entry curr;
//	CIRCLEQ_FOREACH(curr, rq, entries){
//		CIRCLEQ_REMOVE(rq, curr, entries);
//		free(curr);
//	}
}
