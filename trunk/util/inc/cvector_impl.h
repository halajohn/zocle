/* zocle — Z OpenCL Environment
 * Copyright (C) 2009 Wei Hu <wei.hu.tw@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ZOCLE_UTIL_CVECTOR_IMPL_H_
#define ZOCLE_UTIL_CVECTOR_IMPL_H_

#include <cvector.h>

#define IMPLEMENT_CVECTOR(type)                                         \
  DECLARE_CVECTOR(type)                                                 \
                                                                        \
  struct cvector_##type {                                               \
    type *begin;                                                        \
    type *end;                                                          \
    type *capacity;                                                     \
  };                                                                    \
                                                                        \
  cvector_##type *cvector_##type##_new(void) {                          \
    cvector_##type *cvector = clOsalCalloc(sizeof(cvector_##type));     \
    if (NULL == cvector) {                                              \
      return NULL;                                                      \
    }                                                                   \
    cvector->begin = NULL;                                              \
    cvector->end = NULL;                                                \
    cvector->capacity = NULL;                                           \
    return cvector;                                                     \
  }                                                                     \
                                                                        \
  void cvector_##type##_delete(cvector_##type *cvector) {               \
    ASSERT(cvector != NULL);                                            \
    if (cvector->begin != NULL) {                                       \
      clOsalFree(cvector->begin);                                       \
      cvector->begin = NULL;                                            \
    }                                                                   \
    clOsalFree(cvector);                                                \
    cvector = NULL;                                                     \
  }                                                                     \
                                                                        \
  size_t cvector_##type##_size(cvector_##type *cvector) {               \
    ASSERT(cvector != NULL);                                            \
    return (cvector->end - cvector->begin);                             \
  }                                                                     \
                                                                        \
  size_t cvector_##type##_capacity(cvector_##type *cvector) {           \
    ASSERT(cvector != NULL);                                            \
    return (cvector->capacity - cvector->begin);                        \
  }                                                                     \
                                                                        \
  cl_int cvector_##type##_reserve(cvector_##type *cvector, size_t const capacity) \
  {                                                                     \
    size_t old_capacity;                                                \
    size_t old_size;                                                    \
    type *new_space;                                                    \
    ASSERT(cvector != NULL);                                            \
    old_capacity = cvector_##type##_capacity(cvector);                  \
    if (old_capacity >= capacity) {                                     \
      return CL_SUCCESS;                                                \
    }                                                                   \
    new_space = (type *)clOsalCalloc(capacity * sizeof(type));          \
    if (NULL == new_space) {                                            \
      return CL_OUT_OF_HOST_MEMORY;                                     \
    }                                                                   \
    old_size = cvector_##type##_size(cvector);                          \
    if (old_size != 0) {                                                \
      memcpy(new_space, cvector->begin, capacity * sizeof(type));       \
      clOsalFree(cvector->begin);                                       \
      cvector->begin = NULL;                                            \
      cvector->begin = new_space;                                       \
      cvector->end = (new_space + old_size);                            \
      cvector->capacity = (new_space + capacity);                       \
    }                                                                   \
    return CL_SUCCESS;                                                  \
  }                                                                     \
                                                                        \
  cl_int cvector_##type##_push_back(cvector_##type *cvector, type data) { \
    ASSERT(cvector != NULL);                                            \
    if (cvector->end == cvector->capacity) {                            \
      cl_int const result =                                             \
        cvector_##type##_reserve(cvector,                               \
                                 (0 == cvector_##type##_capacity(cvector)) \
                                 ? 1                                    \
                                 : cvector_##type##_capacity(cvector) + 10); \
      if (result != CL_SUCCESS) {                                       \
        return result;                                                  \
      }                                                                 \
    }                                                                   \
    (*cvector->end) = data;                                             \
    ++(cvector->end);                                                   \
    return CL_SUCCESS;                                                  \
  }                                                                     \
                                                                        \
  type *cvector_##type##_begin(cvector_##type *cvector) {               \
    ASSERT(cvector != NULL);                                            \
    return cvector->begin;                                              \
  }                                                                     \
                                                                        \
  type *cvector_##type##_end(cvector_##type *cvector) {                 \
    ASSERT(cvector != NULL);                                            \
    return cvector->end;                                                \
  }                                                                     \
  
#endif
