#include <assert.h>
#include <stdlib.h>
#include "usemaphore.h"
#include "scheduler.h"
#include "thread.h"
#include "runqueue.h"
#include "spinlock.h"

struct _usem
{
    int permits;
    int spinlock;
    runqueue_t waiting;
};

usem_t usem_s_init()
{
    usem_t sem = (usem_t) malloc(sizeof(struct _usem));
    return sem;
}

int usem_init(usem_t *sem, unsigned int value)
{
    *sem = usem_s_init();
    (*sem)->spinlock = 0 ;
    (*sem)->permits = value;
    (*sem)->waiting = runqueue_init();
}


int usem_destroy(usem_t *sem)
{
    (*sem)-> permits = 0;
    assert(runqueue_isEmpty((*sem)-> waiting));
    runqueue_free((*sem)-> waiting);
    free(*sem);
}


int usem_wait(usem_t *sem)
{
    while(1)
    {
        spinlock(&((*sem)->spinlock));

        if((*sem)->permits<=0)
        {
            runqueue_push((*sem)->waiting, thread_self());
            spinunlock(&((*sem)->spinlock));
            sched_schedule();
        }
        else
        {
            (*sem)->permits++;
            spinunlock(&((*sem)->spinlock));
            break;
        }
    }
}

int usem_post(usem_t * sem)
{
    spinlock(&((*sem)->spinlock));
    if(!runqueue_isEmpty( (*sem)->waiting  ))
    {
        thread_t current = runqueue_pop((*sem)->waiting);
        sched_addThread(current);
    }
    (*sem)->permits--;
    spinunlock(&(*sem)->spinlock);
}

int usem_getvalue(usem_t * sem, int *svalue)
{
    *svalue = (*sem)-> permits;
}

int thread_mutex_init (thread_mutex_t * mutex)
{
    return usem_init(mutex,1);
}

int thread_mutex_destroy(thread_mutex_t * mutex)
{
    return usem_destroy(mutex);
}

int thread_mutex_lock(thread_mutex_t *mutex)
{
    return usem_wait(mutex);
}


int thread_mutex_unlock(thread_mutex_t *mutex)
{
    return usem_post(mutex);
}
