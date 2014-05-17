#ifndef __THREAD_H__
#define __THREAD_H__

#ifndef USE_PTHREAD

/* identifiant de thread
 * NB: pourra être un entier au lieu d'un pointeur si ca vous arrange,
 *     mais attention aux inconvénient des tableaux de threads
 *     (consommation mémoire, cout d'allocation, ...).
 */

typedef struct _thread_t* thread_t;

/* recuperer l'identifiant du thread courant.
 */
extern thread_t thread_self(void);

/* creer un nouveau thread qui va exécuter la fonction func avec l'argument funcarg.
 * renvoie 0 en cas de succès, -1 en cas d'erreur.
 */
extern int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg);

/* passer la main à un autre thread.
 */
extern int thread_yield(void);

/* attendre la fin d'exécution d'un thread.
 * la valeur renvoyée par le thread est placée dans *retval.
 * si retval est NULL, la valeur de retour est ignorée.
 */
extern int thread_join(thread_t thread, void **retval);

/* terminer le thread courant en renvoyant la valeur de retour retval.
 * cette fonction ne retourne jamais.
 *
 * L'attribut noreturn aide le compilateur à optimiser le code de
 * l'application (élimination de code mort). Attention à ne pas mettre
 * cet attribut dans votre interface tant que votre thread_exit()
 * n'est pas correctement implémenté (il ne doit jamais retourner).
 */
extern void thread_exit(void *retval) __attribute__ ((__noreturn__));

typedef struct _thread_mutex * thread_mutex_t;


/**
 *\brief Initializes a mutex to default values, This function does not
 * allocate the memory. You need to allocate the memory yourself with malloc().
 * The state of the mutex after calling this function is UNLOCKED.
 * You need to destroy a mutex before calling thread_mutex_init on it again.
 *\return 0 on success.
 **/
extern int thread_mutex_init(thread_mutex_t * mutex);

/**
 * \brief Destroys a mutex, and sets its sate to an undefined value.
 *  destroyed mutexes can be re-initialized with thread_mutex_init.
 * 	It is safer to only destroy initialized and unlocked mutexes. Destroying
 *  a locked mutex may result in undefined behaviour.
 * \return 0 on success, and -1 on failure
 **/
extern int thread_mutex_destroy(thread_mutex_t *mutex);

/**
 * \brief If the mutex isn't already locked, sets its state to LOCKED, and
 * gives the ownership of that mutex to the calling thread. If the mutex
 * is already locked, then the calling thread blocks until the mutex
 * becomes available.
 * \return 0 on success, and -1 on failure
 **/
extern int thread_mutex_lock(thread_mutex_t *mutex);



/* Do we implement this?
int thread_mutex_trylock(thread_mutex_t *mutex);
*/


/**
 *\brief Releases the mutex object referenced by mutex.
 * The scheduling policy decides which of the potential waiting threads is
 * going to acquire the mutex.
 * \return 0 on success, and -1 on failure
 **/
extern int thread_mutex_unlock(thread_mutex_t *mutex);


#else /* USE_PTHREAD */

/* Si on compile avec -DUSE_PTHREAD, ce sont les pthreads qui sont utilisés */
#include <sched.h>
#include <pthread.h>

#define thread_self pthread_self
#define thread_create(th, func, arg) pthread_create(th, NULL, func, arg)
#define thread_yield sched_yield
#define thread_join pthread_join
#define thread_exit pthread_exit

#define thread_mutex_init(mutex)    pthread_mutex_init(mutex, NULL)
#define thread_mutex_destroy        pthread_mutex_destroy
#define thread_mutex_lock          pthread_mutex_lock
#define thread_mutex_unlock         pthread_mutex_unlock
typedef  pthread_t thread_t;
typedef  pthread_mutex_t thread_mutex_t;
#endif /* USE_PTHREAD */

#endif /* __THREAD_H__ */
