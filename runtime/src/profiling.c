#include <cl.h>

#include <osal.h>
#include <cl_internal.h>

CL_API_ENTRY cl_int CL_API_CALL
clGetEventProfilingInfo(cl_event            event,
                        cl_profiling_info   param_name,
                        size_t              param_value_size,
                        void *              param_value,
                        size_t *            param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == event) {
    return CL_INVALID_EVENT;
  }
  switch (param_name) {
  case CL_PROFILING_COMMAND_QUEUED:
  case CL_PROFILING_COMMAND_SUBMIT:
  case CL_PROFILING_COMMAND_START:
  case CL_PROFILING_COMMAND_END:
    /* TODO: handle them. */
    break;
    
  default:
    return CL_INVALID_VALUE;
  }
  return CL_SUCCESS;
}
