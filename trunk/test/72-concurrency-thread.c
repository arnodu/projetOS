#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "spinlock.h"

#define THREADNUM 50

double a =0;

thread_mutex_t mutex;

void thread_f()
{
  //thread_mutex_lock(&mutex);
  //spinlock(&lock);

  double local,locala = a;
  local  = log( sqrt(a*a +1 )/exp(2.0 + 0.1*a));
  thread_yield();
  a=  locala +1;

  //thread_mutex_unlock(&mutex);
  //spinunlock(&lock);
  return;
}

int main(void)
{
  thread_mutex_init(&mutex);
  int err, i;
  void * b;
  thread_t thread[THREADNUM];

  for(i = 0; i< THREADNUM; i++)
  {
    err = thread_create(&thread[i], (void *)thread_f, NULL);
    if(err != 0)
    {
      fprintf(stderr, "Can't create thread %d", i);
      exit(-1);
    }
  }
  for(i = 0; i< THREADNUM; i++)
  {
    //thread_mutex_lock(&mutex);
    thread_join(thread[i], &b);
	//thread_mutex_unlock(&mutex);
  }
  thread_mutex_destroy(&mutex);
  printf("%g \n", a);
  //spinlock(&scheduler_spinlock);
  return 0;
}
