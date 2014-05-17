#include <assert.h>
#include <stdlib.h>
#include "usemaphore.h"
#include "scheduler.h"
#include "thread.h"
#include "runqueue.h"

typedef struct _usem
{
    int permits;
    int spinlock_locking;
    runqueue_t waiting;
}* usem_t;

void spinlock(int * lock)
{
    while(__atomic_test_and_set (lock, __ATOMIC_SEQ_CST )){
        thread_yield();
    }
}
void spinunlock(int * lock)
{
    *lock = 0;
}

usem_t usem_s_init()
{
    usem_t sem = (usem_t) malloc(sizeof(struct _usem));
    return sem;
}

int usem_init(usem_t *sem, unsigned int value)
{
    *sem = usem_s_init();
    (*sem)-> permits = value;
    (*sem)-> waiting = runqueue_init();
    (*sem)->spinlock_locking = 0 ;

}


int usem_destroy(usem_t *sem)
{
    (*sem)-> permits = 0;
    assert(runqueue_isEmpty((*sem)-> waiting));
    runqueue_free((*sem)-> waiting);

}


int usem_wait(usem_t *sem)
{
}

int usem_post(usem_t * sem)
{
}

int usem_getvalue(usem_t * sem, int *svalue)
{
    *svalue = (*sem)-> permits;
}
