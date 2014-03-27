#include "thread.h"
#include "thread_t.h"

int thread_yield(void)
{
		ordo_addThread(thread_self());
	ordo_execute();
}
