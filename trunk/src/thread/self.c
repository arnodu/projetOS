#include "thread.h"
#include "thread_t.h"

#include <assert.h>

thread_t thread_self(void)
{
	return ordo_getRunningThread();
}
