#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADNUM 40

long long a =1;
long long b= 0;
//pthread_mutex MYMUTEX;
pthread_mutex_t * mutex;
int * lock = 0;

void thread_f()
{
  //pthread_mutex_lock(mutex);
  a=a+b;
  b= a+b;
  //pthread_mutex_unlock(mutex);
  return;
}

int main(void)
{
  mutex = malloc(sizeof(*mutex));
  pthread_mutex_init(mutex, NULL);
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
  pthread_mutex_destroy(mutex);
  printf("%lld \n", a);
}
