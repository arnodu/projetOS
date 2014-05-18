#include "thread.h"

void spinlock(int * lock)
{
    while(__atomic_test_and_set (lock, __ATOMIC_SEQ_CST )){
        //thread_yield();
    }
}
void spinunlock(int * lock)
{
    *lock = 0;
}
