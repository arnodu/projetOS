#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "spinlock.h"

#define THREADNUM 50

double a =0;

thread_mutex_t mutex;
int lock = 0;

void thread_f()
{
    spinunlock(&lock);
  //thread_mutex_lock(&mutex);
  //spinlock(&lock);

  double local,locala = a;
  local  = log( sqrt(a*a +1 )/exp(2.0 + 0.1*a));
    spinlock(&lock);
  thread_yield();
  spinunlock(&lock);
  a=  locala +1;

  //thread_mutex_unlock(&mutex);
  //spinunlock(&lock);
  spinlock(&lock);
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
    spinlock(&lock);
    err = thread_create(&thread[i], (void *)thread_f, NULL);
    spinunlock(&lock);
    if(err != 0)
    {
      fprintf(stderr, "Can't create thread %d", i);
      exit(-1);
    }
  }
  for(i = 0; i< THREADNUM; i++)
  {
    //thread_mutex_lock(&mutex);
	spinlock(&lock);
    thread_join(thread[i], &b);
	spinunlock(&lock);
	//thread_mutex_unlock(&mutex);
  }
  thread_mutex_destroy(&mutex);
  printf("%g \n", a);
  spinlock(&lock);
  return 0;
}
