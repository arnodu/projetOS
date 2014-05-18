#include "runqueue.h"
#include "thread_t.h"
#include "usemaphore.h"
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
	usem_t semaphore;
	//thread_mutex_t mutex;
};


runqueue_t runqueue_init	()
{
	runqueue_t rq = malloc(sizeof(struct _runqueue_t));
	CIRCLEQ_INIT(&rq->list);
	return rq;
}
int runqueue_isEmpty(runqueue_t rq)
{
	return CIRCLEQ_EMPTY(&rq->list);
}
void runqueue_push	(runqueue_t rq, thread_t thread)
{
	CIRCLEQ_INSERT_TAIL(&rq->list, thread, entries);
}
thread_t runqueue_pop	(runqueue_t rq)
{
	assert(!runqueue_isEmpty(rq));
	thread_t e = CIRCLEQ_FIRST(&rq->list);
	CIRCLEQ_REMOVE(&rq->list, e, entries);

	return e;
}
void runqueue_free	(runqueue_t rq)
{
	free(rq);
}


runqueue_t runqueue_init_safe()
{
	runqueue_t rq = runqueue_init();
	usem_init(&rq->semaphore,0);
	//thread_mutex_init(&rq->mutex);
	rq->spinlock = 0;
	return rq;
}
int runqueue_isEmpty_safe(runqueue_t rq)
{
	//thread_mutex_lock(&rq->mutex);
	spinlock(&rq->spinlock);
    int res = runqueue_isEmpty(rq);
    //thread_mutex_unlock(&rq->mutex);
    spinunlock(&rq->spinlock);
    return res;
}
void runqueue_push_safe(runqueue_t rq, thread_t thread)
{
    //thread_mutex_lock(&rq->mutex);
    spinlock(&rq->spinlock);
    runqueue_push(rq,thread);
    //thread_mutex_unlock(&rq->mutex);
    spinunlock(&rq->spinlock);
    usem_post(&rq->semaphore);
}
thread_t runqueue_pop_safe(runqueue_t rq)
{
	usem_wait(&rq->semaphore);
    //thread_mutex_lock(&rq->mutex);
    spinlock(&rq->spinlock);
	thread_t res = runqueue_pop(rq);
    //thread_mutex_unlock(&rq->mutex);
    spinunlock(&rq->spinlock);
    return res;
}
void runqueue_free_safe(runqueue_t rq)
{
	usem_destroy(&rq->semaphore);
    //thread_mutex_destroy(&rq->mutex);
	runqueue_free(rq);
}
