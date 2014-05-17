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
CIRCLEQ_HEAD(_runqueue_list, _thread_t);

struct _runqueue_t{
	struct _runqueue_list list;
	int spinlock;
};


runqueue_t runqueue_init	()
{
	runqueue_t rq = malloc(sizeof(struct _runqueue_t));
	CIRCLEQ_INIT(&rq->list);
	rq->spinlock = 0;
	return rq;
}
int runqueue_isEmpty(runqueue_t rq)
{
	spinlock(&rq->spinlock);

	int res = CIRCLEQ_EMPTY(&rq->list);

	spinunlock(&rq->spinlock);

	return res;
}
void runqueue_push	(runqueue_t rq, thread_t thread)
{
	spinlock(&rq->spinlock);

	CIRCLEQ_INSERT_TAIL(&rq->list, thread, entries);

	spinunlock(&rq->spinlock);
}
thread_t runqueue_pop	(runqueue_t rq)
{
	spinlock(&rq->spinlock);

	assert(!runqueue_isEmpty(rq));
	thread_t e = CIRCLEQ_FIRST(&rq->list);
	CIRCLEQ_REMOVE(&rq->list, e, entries);

	spinunlock(&rq->spinlock);
	return e;
}
void runqueue_free	(runqueue_t rq)
{
	//assert(runqueue_isEmpty(rq));
	free(rq);

//	runqueue_entry curr;
//	CIRCLEQ_FOREACH(curr, rq, entries){
//		CIRCLEQ_REMOVE(rq, curr, entries);
//		free(curr);
//	}
}
