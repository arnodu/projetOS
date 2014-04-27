#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"

/* Test de l'appel à exit dans un thread
 */

static void * thfunc(void *dummy __attribute__((unused)))
{
  thread_exit((void*)0xdeadbeef);
}

static void * thfunc2(void *dummy __attribute__((unused)))
{
  exit(0);
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

  err = thread_join(th, &res);
  assert(!err);
  assert(res == (void*) 0xdeadbeef);

  err = thread_join(th2, &res);
  assert(!err);
  assert(res == (void*) 0xbeefdead);

  printf("join OK\n");
  return 1;
}
