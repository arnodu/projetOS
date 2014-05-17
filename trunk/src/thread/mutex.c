#include <assert.h>
#include <stdlib.h>
#include "scheduler.h"
#include "thread.h"
#include "runqueue.h"
#include "spinlock.h"

typedef struct _thread_mutex
{
    int locked;
    int spinlock_locking;
    thread_t owner;
    runqueue_t waiting;
}* thread_mutex_t;

thread_mutex_t mutex_s_init()
{
    thread_mutex_t mutex = (thread_mutex_t ) malloc(sizeof(struct _thread_mutex));
    return mutex;
}

int thread_mutex_init (thread_mutex_t * mutex)
{
    *mutex = mutex_s_init();
    (*mutex)->spinlock_locking = 0;
    (*mutex)->locked = 0;
    (*mutex)->owner = NULL;
    (*mutex)->waiting = runqueue_init();
}

int thread_mutex_destroy(thread_mutex_t * mutex)
{
    (*mutex)->locked= 0;
    assert(runqueue_isEmpty((*mutex)->waiting));
    runqueue_free((*mutex)->waiting);
    free(*mutex);
}



int thread_mutex_lock(thread_mutex_t *mutex)
{
    spinlock(&((*mutex)->spinlock_locking));

    /*if((*mutex)->locked)
    {
        runqueue_push((*mutex)->waiting, thread_self());
        spinunlock(&((*mutex)->spinlock_locking));
        sched_schedule();
        thread_mutex_lock(mutex);
    }
    else
    {
        (*mutex)->locked = 1;
        spinunlock(&((*mutex)->spinlock_locking));
    }*/
}


int thread_mutex_unlock(thread_mutex_t *mutex)
{
    /*(*mutex)->owner = NULL;
    spinlock(&((*mutex)->spinlock_locking));
    if(!runqueue_isEmpty( (*mutex)->waiting  ))
    {
        thread_t current = runqueue_pop((*mutex)->waiting);
        sched_addThread(current);
    }
    (*mutex)->locked = 0;*/
    spinunlock(&((*mutex)->spinlock_locking));

    return 0;
}
