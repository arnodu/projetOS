#include <stdio.h>
#include <assert.h>
#include "thread.h"

/* test de la préemption
 */

static void * thfunc(void *dummy __attribute__((unused)))
{
  while(1);
}

static void * thfunc2(void *dummy __attribute__((unused)))
{
  printf("Preemption OK\n");
  return (void*) 0xbeefdead;
}

int main()
{
  thread_t th, th2;
  int err;
  void *res = NULL;

  err = thread_create(&th, thfunc, NULL);
  assert(!err);
  err = thread_create(&th2, thfunc2, NULL);
  assert(!err);

  err = thread_join(th2, &res);
  assert(!err);
  return (!(res == (void*) 0xbeefdead));

}
