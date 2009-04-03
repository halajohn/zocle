#ifndef ZOCLE_PLATFORM_OSAL_H_
#define ZOCLE_PLATFORM_OSAL_H_

#include <string.h>
#include <assert.h>

#define ASSERT(x) assert(x)

extern void *clOsalMalloc(size_t const /* size */);
extern void *clOsalCalloc(size_t const /* size */);
extern void clOsalFree(void * /* ptr */);

#endif
