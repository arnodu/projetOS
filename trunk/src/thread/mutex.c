#include <assert.h>
#include <stdlib.h>
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
    (*mutex)->state = UNLOCKED;
    (*mutex)->owner = NULL;
    (*mutex)->waiting = runqueue_init();
}

int thread_mutex_destroy(thread_mutex_t * mutex)
{
    (*mutex)->state= DESTROYED;
    assert(runqueue_isEmpty((*mutex)->waiting));
    runqueue_free((*mutex)->waiting);
    free(*mutex);
}


int thread_mutex_lock(thread_mutex_t *mutex)
{
    thread_t me = thread_self();
    if((*mutex)-> state == LOCKED)
    {
        if((*mutex)->owner == me)
        {
            // Le même thread a appelé deux fois lock, il faut retourner une erreur.
            return -1;
        }
        else{
            // On ajoute le thread appelant dans la liste des threads qui attendent ce mutex.
            runqueue_push((*mutex)->waiting, me);
        }
    }
    while(__atomic_test_and_set (&(*mutex)->state, __ATOMIC_SEQ_CST));
    (*mutex)->owner = me;
    return 0;
}


int thread_mutex_unlock(thread_mutex_t *mutex)
{
    thread_t current;
    (*mutex)->state = UNLOCKED;
    (*mutex)->owner = NULL;
    while(!runqueue_isEmpty( (*mutex)->waiting  ))
    {
        current = runqueue_pop((*mutex)->waiting);
        sched_addThread(current);
    }
    return 0;
}
