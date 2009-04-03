#ifndef ZOCLE_UTIL_CVECTOR_H_
#define ZOCLE_UTIL_CVECTOR_H_

#include <string.h>

#include <cl.h>

#define DECLARE_CVECTOR(type)                                           \
  struct cvector_##type;                                                \
  typedef struct cvector_##type cvector_##type;                         \
                                                                        \
  extern cvector_##type *cvector_##type##_new(void);                    \
  extern void cvector_##type##_delete(cvector_##type *cvector);         \
  extern size_t cvector_##type##_size(cvector_##type *cvector);         \
  extern size_t cvector_##type##_capacity(cvector_##type *cvector);     \
  extern cl_int cvector_##type##_reserve(cvector_##type *cvector, size_t const capacity); \
  extern cl_int cvector_##type##_push_back(cvector_##type *cvector, type data); \
  extern type *cvector_##type##_begin(cvector_##type *cvector);         \
  extern type *cvector_##type##_end(cvector_##type *cvector);           \
  
#endif
