#include <cl.h>

#include <osal.h>
#include <cl_internal.h>

CL_API_ENTRY cl_kernel CL_API_CALL
clCreateKernel(cl_program      program,
               char const *    kernel_name,
               cl_int *        errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
  cl_int return_code = CL_SUCCESS;
  cl_kernel kernel = NULL;
  
  if (NULL == program) {
    return_code = CL_INVALID_PROGRAM;
    goto error;
  }
  if (NULL == kernel_name) {
    return_code = CL_INVALID_VALUE;
    goto error;
  }
  /* TODO: handle CL_INVALID_PROGRAM_EXECUTABLE */
  /* TODO: handle CL_INVALID_KERNEL_NAME */
  /* TODO: handle CL_INVALID_KERNEL_DEFINITION */
  
  kernel = clOsalCalloc(sizeof(struct _cl_kernel));
  if (NULL == kernel) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  kernel->name = kernel_name;
  clRetainKernel(kernel);
  goto success;
  
 error:
  if (kernel != NULL) {
    clOsalFree(kernel);
    kernel = NULL;
  }
  
 success:
  if (errcode_ret != NULL) {
    (*errcode_ret) = return_code;
  }
  return kernel;
}

CL_API_ENTRY cl_int CL_API_CALL
clCreateKernelsInProgram(cl_program     program,
                         cl_uint        num_kernels,
                         cl_kernel *    kernels,
                         cl_uint *      num_kernels_ret) CL_API_SUFFIX__VERSION_1_0 {
  cl_int return_code = CL_SUCCESS;
  cl_kernel kernel = NULL;
  
  if (NULL == program) {
    return_code = CL_INVALID_PROGRAM;
    goto error;
  }
  /* TODO: handle CL_INVALID_PROGRAM_EXECUTABLE */
  clRetainKernel(kernel);
  goto success;
  
 error:
 success:
  return return_code;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainKernel(cl_kernel    kernel) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == kernel) {
    return CL_INVALID_KERNEL;
  }
  ++kernel->ref_count;
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseKernel(cl_kernel   kernel) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == kernel) {
    return CL_INVALID_KERNEL;
  }
  --kernel->ref_count;
  if (0 == kernel->ref_count) {
    /* TODO: wait all queued execution instances of kernel have finished. */
  }
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArg(cl_kernel    kernel,
               cl_uint      arg_index,
               size_t       arg_size,
               const void * arg_value) CL_API_SUFFIX__VERSION_1_0 {
  /* TODO: implement this function */
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelInfo(cl_kernel       kernel,
                cl_kernel_info  param_name,
                size_t          param_value_size,
                void *          param_value,
                size_t *        param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == kernel) {
    return CL_INVALID_KERNEL;
  }
  switch (param_name) {
  case CL_KERNEL_FUNCTION_NAME:
  case CL_KERNEL_NUM_ARGS:
  case CL_KERNEL_REFERENCE_COUNT:
  case CL_KERNEL_CONTEXT:
  case CL_KERNEL_PROGRAM:
    /* TODO: handle them. */
    break;
    
  default:
    return CL_INVALID_VALUE;
  }
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelWorkGroupInfo(cl_kernel                  kernel,
                         cl_device_id               device,
                         cl_kernel_work_group_info  param_name,
                         size_t                     param_value_size,
                         void *                     param_value,
                         size_t *                   param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == kernel) {
    return CL_INVALID_KERNEL;
  }
  switch (param_name) {
  case CL_KERNEL_WORK_GROUP_SIZE:
  case CL_KERNEL_COMPILE_WORK_GROUP_SIZE:
    /* TODO: handle them. */
    break;
    
  default:
    return CL_INVALID_VALUE;
  }
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNDRangeKernel(cl_command_queue command_queue,
                       cl_kernel        kernel,
                       cl_uint          work_dim,
                       const size_t *   global_work_offset,
                       const size_t *   global_work_size,
                       const size_t *   local_work_size,
                       cl_uint          num_events_in_wait_list,
                       const cl_event * event_wait_list,
                       cl_event *       event) CL_API_SUFFIX__VERSION_1_0 {
  cl_uint i;
  cl_command cmd = NULL;
  cl_int return_code = CL_SUCCESS;
  cl_event event_allocated = NULL;
  
  if (NULL == command_queue) {
    return_code = CL_INVALID_COMMAND_QUEUE;
    goto error;
  }
  if (NULL == kernel) {
    return_code = CL_INVALID_MEM_OBJECT;
    goto error;
  }
  /* TODO: implement this function. */
  if ((NULL == event_wait_list) && (num_events_in_wait_list > 0)) {
    return_code = CL_INVALID_EVENT_WAIT_LIST;
    goto error;
  }
  if ((event_wait_list != NULL) && (0 == num_events_in_wait_list)) {
    return_code = CL_INVALID_EVENT_WAIT_LIST;
    goto error;
  }
  for (i = (num_events_in_wait_list - 1); i >= 0; --i) {
    if (NULL == event_wait_list[i]) {
      return_code = CL_INVALID_EVENT_WAIT_LIST;
      goto error;
    }
  }
  cmd = clOsalCalloc(sizeof(struct _cl_command));
  if (NULL == cmd) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  cmd->type = CL_COMMAND_NDRANGE_KERNEL;
  cmd->num_events_in_wait_list = num_events_in_wait_list;
  cmd->event_wait_list = event_wait_list;
  
  cmd->u._cl_ndrange_kernel_command.kernel = kernel;
  
  if (event != NULL) {
    event_allocated = clOsalCalloc(sizeof(struct _cl_event));
    if (NULL == event_allocated) {
      return_code = CL_OUT_OF_HOST_MEMORY;
      goto error;
    }
    event_allocated->command = cmd;
    (*event) = event_allocated;
  }
  
  cvector_cl_command_push_back(command_queue->commands, cmd);
  cmd->execution_status = CL_QUEUED;
  
  goto success;
  
 error:
  if (cmd != NULL) {
    clOsalFree(cmd);
    cmd = NULL;
  }
  if (event_allocated != NULL) {
    clOsalFree(event_allocated);
    event_allocated = NULL;
  }
  
 success:
  return return_code;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueTask(cl_command_queue  command_queue,
              cl_kernel         kernel,
              cl_uint           num_events_in_wait_list,
              const cl_event *  event_wait_list,
              cl_event *        event) CL_API_SUFFIX__VERSION_1_0 {
  cl_uint i;
  cl_command cmd = NULL;
  cl_int return_code = CL_SUCCESS;
  cl_event event_allocated = NULL;
  
  if (NULL == command_queue) {
    return_code = CL_INVALID_COMMAND_QUEUE;
    goto error;
  }
  if (NULL == kernel) {
    return_code = CL_INVALID_MEM_OBJECT;
    goto error;
  }
  /* TODO: implement this function. */
  if ((NULL == event_wait_list) && (num_events_in_wait_list > 0)) {
    return_code = CL_INVALID_EVENT_WAIT_LIST;
    goto error;
  }
  if ((event_wait_list != NULL) && (0 == num_events_in_wait_list)) {
    return_code = CL_INVALID_EVENT_WAIT_LIST;
    goto error;
  }
  for (i = (num_events_in_wait_list - 1); i >= 0; --i) {
    if (NULL == event_wait_list[i]) {
      return_code = CL_INVALID_EVENT_WAIT_LIST;
      goto error;
    }
  }
  cmd = clOsalCalloc(sizeof(struct _cl_command));
  if (NULL == cmd) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  cmd->type = CL_COMMAND_TASK;
  cmd->num_events_in_wait_list = num_events_in_wait_list;
  cmd->event_wait_list = event_wait_list;
  
  cmd->u._cl_task_command.kernel = kernel;
  
  if (event != NULL) {
    event_allocated = clOsalCalloc(sizeof(struct _cl_event));
    if (NULL == event_allocated) {
      return_code = CL_OUT_OF_HOST_MEMORY;
      goto error;
    }
    event_allocated->command = cmd;
    (*event) = event_allocated;
  }
  
  cvector_cl_command_push_back(command_queue->commands, cmd);
  cmd->execution_status = CL_QUEUED;
  
  goto success;
  
 error:
  if (cmd != NULL) {
    clOsalFree(cmd);
    cmd = NULL;
  }
  if (event_allocated != NULL) {
    clOsalFree(event_allocated);
    event_allocated = NULL;
  }
  
 success:
  return return_code;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNativeKernel(cl_command_queue  command_queue,
                      void (*user_func)(void *), 
                      void *            args,
                      size_t            cb_args, 
                      cl_uint           num_mem_objects,
                      const cl_mem *    mem_list,
                      const void **     args_mem_loc,
                      cl_uint           num_events_in_wait_list,
                      const cl_event *  event_wait_list,
                      cl_event *        event) CL_API_SUFFIX__VERSION_1_0 {
  cl_uint i;
  cl_command cmd = NULL;
  cl_int return_code = CL_SUCCESS;
  cl_event event_allocated = NULL;
  
  if (NULL == command_queue) {
    return_code = CL_INVALID_COMMAND_QUEUE;
    goto error;
  }
  /* TODO: implement this function. */
  if ((NULL == event_wait_list) && (num_events_in_wait_list > 0)) {
    return_code = CL_INVALID_EVENT_WAIT_LIST;
    goto error;
  }
  if ((event_wait_list != NULL) && (0 == num_events_in_wait_list)) {
    return_code = CL_INVALID_EVENT_WAIT_LIST;
    goto error;
  }
  for (i = (num_events_in_wait_list - 1); i >= 0; --i) {
    if (NULL == event_wait_list[i]) {
      return_code = CL_INVALID_EVENT_WAIT_LIST;
      goto error;
    }
  }
  cmd = clOsalCalloc(sizeof(struct _cl_command));
  if (NULL == cmd) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  cmd->type = CL_COMMAND_NATIVE_KERNEL;
  cmd->num_events_in_wait_list = num_events_in_wait_list;
  cmd->event_wait_list = event_wait_list;
  
  cmd->u._cl_native_kernel_command.user_func = user_func;
  
  if (event != NULL) {
    event_allocated = clOsalCalloc(sizeof(struct _cl_event));
    if (NULL == event_allocated) {
      return_code = CL_OUT_OF_HOST_MEMORY;
      goto error;
    }
    event_allocated->command = cmd;
    (*event) = event_allocated;
  }
  
  cvector_cl_command_push_back(command_queue->commands, cmd);
  cmd->execution_status = CL_QUEUED;
  
  goto success;
  
 error:
  if (cmd != NULL) {
    clOsalFree(cmd);
    cmd = NULL;
  }
  if (event_allocated != NULL) {
    clOsalFree(event_allocated);
    event_allocated = NULL;
  }
  
 success:
  return return_code;
}
