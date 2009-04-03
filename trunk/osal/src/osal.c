#include <osal.h>

#include <stdlib.h>

void *clOsalMalloc(size_t const size) {
  return malloc(size);
}

void *clOsalCalloc(size_t const size) {
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}

void clOsalFree(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
  }
}
