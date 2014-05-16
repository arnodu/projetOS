#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include "thread.h"
#include "scheduler.h"

/* test de la preemption
 *
 */

static void * boucle(int *i)
{
  printf("Thread de while(1)");
  while(1);
}

static void * message(int *i)
{
	printf("Hello\n");
}

int main(int argc, char *argv[])
{
  int err;
  thread_t *ths;
  int i;
  i = 1;

  ths = malloc(2*sizeof(thread_t));
  assert(ths);
  
  err = thread_create(&ths[0], boucle, (void*) i);
  assert(!err);
	
  err = thread_create(&ths[1], message, (void*) i);
  assert(!err);
	
  free(ths);

  return 0;
}
