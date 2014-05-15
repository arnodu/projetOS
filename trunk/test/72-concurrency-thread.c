#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define THREADNUM 50

double a =0;

thread_mutex_t mutex;

void thread_f()
{
  //thread_mutex_lock(&mutex);

  double local = a;
  local  = log( sqrt(a*a +1 )/exp(2.0 + 0.1*a));
  usleep(10000);
  a= a +1;
	//unlock();
  //thread_mutex_unlock(&mutex);
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
    thread_join(thread[i], &b);

  }
  thread_mutex_destroy(&mutex);
  printf("%g \n", a);
  return 0;
}
