#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADNUM 50

long long a =1;
long long b= 0;
//pthread_mutex MYMUTEX;
pthread_mutex_t mutex;
int lock_v = 0;

void lock()
{
	while(__atomic_test_and_set(&lock_v, __ATOMIC_SEQ_CST));
}

void unlock()
{
	lock_v = 0;
}
void thread_f()
{
  //thread_mutex_lock(&mutex);

  int local =a;
  local = sqrt(local*local +2)/

  //thread_mutex_unlock(&mutex);
  return;
}

int main(void)
{
  pthread_mutex_init(&mutex, NULL);
  int err;
  void * b;
  pthread_t thread[THREADNUM];

  for(int i = 0; i< THREADNUM; i++)
  {
    err = pthread_create(&thread[i], NULL , (void *)thread_f, NULL);
    if(err != 0)
    {
      fprintf(stderr, "Can't create thread %d", i);
      exit(-1);
    }
  }
  for(int i = 0; i< THREADNUM; i++)
  {
    pthread_join(thread[i], &b);

  }
  pthread_mutex_destroy(&mutex);
  printf("%lld \n", a);
}
