#include "mutex.h"
#include "scheduler.h"
#include "thread.h"
#include "runqueue.h"


typedef enum _mutex_state{
    UNLOCKED=0,
    LOCKED=1,
    DESTROYED,
}mutex_state;

typedef struct _thread_mutex
{
    mutex_state state;
    thread_t owner;
    runqueue_t waiting_threads;

} thread_mutex_t;


int thread_mutex_init (thread_mutex_t * mutex)
{
    mutex->state = UNLOCKED;
    mutex->owner = thread_self();
    mutex->waiting_threads = runqueue_init();
}

int thread_mutex_destroy(thread_mutex_t * mutex)
{
    mutex->state= DESTROYED;
    runqueue_free(mutex->waiting_threads);
}

int thread_mutex_lock(thread_mutex_t *mutex)
{

//    while(__atomic_test_and_set (&mutex->state, __ATOMIC_SEQ_CST));


}
