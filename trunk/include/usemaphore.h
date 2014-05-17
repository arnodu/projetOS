#ifndef __USEMAPHORE_H__
#define __USEMAPHORE_H__

#ifndef USE_PTHREAD

typedef struct _usem * usem_t;

/** 
 * \brief Initializes a semaphore at the address pointed by sem.
 * Semaphores allocated with usem_init should be destroyed with usem_destroy.
 * \return 0 on success, -1 on error.
 */
int usem_init(usem_t *sem, unsigned int value);

/** 
 * \brief Destroys the semaphore pointed by sem.
 * Only semaphores initialized with usem_init should be destroyed using this function.
 * \return 0 on success, -1 on error
 */
int usem_destroy(usem_t *sem);

/**
 * \brief Increment the semaphore pointed by sem, allowing other threads
 * blocked in usem_wait to lock the semaphore
 * \return 0 on success, -1 on failure
 */
int usem_post(usem_t * sem);

/**
 * \brief Decrements (locks) and returns immediately  the semaphore
 * pointed by sem if its value is greater than 0.
 * Else, the call blocks until until it is possible to decrement it
 * \return 0 on success, -1 on failure.
 */
int usem_wait(usem_t *sem);

/**
 * \brief Places the current value of the semaphore sem into the the integer svalue.
 * \return 0 on success, -1 on failure.
 */
int usem_getvalue(usem_t * sem, int *svalue);


// Possible extensions : usem_trywait, usem_timedwait, usem_open, usem_close.
#else /* USE_PTHREAD */

/* Si on compile avec -DUSE_PTHREAD, ce sont les pthreads qui sont utilisés */
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>

typedef usem_t sem_t;

#define usem_init(sem, value) sem_init(sem, 0, value)
#define usem_destroy sem_destroy
#define usem_post sem_post
#define usem_wait sem_wait
#define usem_getvalue sem_getvalue

#endif /* USE_PTHREAD */

#endif /* __THREAD_H__ */
