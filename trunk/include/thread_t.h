#ifndef __THEAD_T_H__
#define __THEAD_T_H__

#include <stdlib.h>
#include <ucontext.h>
#include "ordo.h"

struct thread_t{
  ucontext_t ucontext;
  int terminated;
};

#endif
