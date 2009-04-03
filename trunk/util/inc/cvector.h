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
