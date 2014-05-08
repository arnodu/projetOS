#ifndef THREAD_MUTEX_H
#define THREAD_MUTEX_H

typedef thread_mutex_t;

/**
 *\brief Initializes a mutex to default values, This function does not
 * allocate the memory. You need to allocate the memory yourself with malloc().
 * The state of the mutex after calling this function is UNLOCKED.
 *\return 0 on success, ////////!\\\\\\\\\\(?) on error
 **/
int thread_mutex_init(thread_mutex_t * mutex);

/**
 * \brief Destroys a mutex, and sets its sate to an undefined value.
 *  destroyed mutexes can be re-initialized with thread_mutex_init.
 * 	It is safer to only destroy initialized and unlocked mutexes. Destroying
 *  a locked mutex may result in undefined behaviour.
 **/
 int thread_mutex_destroy(thread_mutex_t *mutex);

/**
 * \brief If the mutex isn't already locked, sets its state to LOCKED, and
 * gives the ownership of that mutex to the calling thread. If the mutex
 * is already locked, then the calling thread blocks until the mutex 
 * becomes available.
 **/
int thread_mutex_lock(thread_mutex_t *mutex);



/* Do we implement this?
int thread_mutex_trylock(thread_mutex_t *mutex);
*/


/**
 *\brief Releases the mutex object referenced by mutex.
 * The scheduling policy decides which of the potential waiting threads is
 * going to acquire the mutex.
 **/
int thread_mutex_unlock(thread_mutex_t *mutex);

#endif
