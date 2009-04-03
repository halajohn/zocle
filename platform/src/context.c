#include <cl.h>

#include <string.h>

#include <osal.h>
#include <cl_internal.h>

static cl_int
clInitContext(cl_context  context,
              logging_fn  pfn_notify,
              void *      user_data) {
  ASSERT(context != NULL);
  context->pfn_notify = pfn_notify;
  context->user_data = user_data;
  
  context->command_queues = cvector_cl_command_queue_new();
  if (NULL == context->command_queues) {
    return CL_OUT_OF_HOST_MEMORY;
  }
  
  context->mem_objs = cvector_cl_mem_new();
  if (NULL == context->mem_objs) {
    clOsalFree(context->command_queues);
    return CL_OUT_OF_HOST_MEMORY;
  }
  return CL_SUCCESS;
}

CL_API_ENTRY cl_context CL_API_CALL
clCreateContext(cl_context_properties * properties,
                cl_uint                 num_devices,
                const cl_device_id *    devices,
                logging_fn              pfn_notify,
                void *                  user_data,
                cl_int *                errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
  cl_context context = NULL;
  cl_int return_code = CL_SUCCESS;
  
  if ((properties != NULL) ||
      (NULL == devices) ||
      (0 == num_devices)) {
    return_code = CL_INVALID_VALUE;
    goto error;
  }
  
  /* TODO: handle CL_INVALID_DEVICE, CL_INVALID_DEVICE_LIST,
   * CL_DEVICE_NOT_AVAILABLE
   */
  
  context = clOsalCalloc(sizeof(struct _cl_context));
  if (NULL == context) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  context->devices = clOsalCalloc(num_devices * sizeof(cl_device_id));
  if (NULL == context->devices) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  memcpy(context->devices, devices, num_devices * sizeof(cl_device_id));
  {
    cl_int const result = clInitContext(context, pfn_notify, user_data);
    switch (result) {
    case CL_SUCCESS:
      break;
    case CL_OUT_OF_HOST_MEMORY:
      return_code = CL_OUT_OF_HOST_MEMORY;
      goto error;
    default:
      ASSERT(0);
      break;
    }
  }
  clRetainContext(context);
  return_code = CL_SUCCESS;
  goto success;
  
 error:
  if (context != NULL) {
    clOsalFree(context);
    context = NULL;
  }
  
 success:
  if (errcode_ret != NULL) {
    return_code = return_code;
  }
  return context;
}

CL_API_ENTRY cl_context CL_API_CALL
clCreateContextFromType(cl_context_properties * properties,
                        cl_device_type          device_type,
                        logging_fn              pfn_notify,
                        void *                  user_data,
                        cl_int *                errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
  cl_context context = NULL;
  cl_uint num_devices;
  cl_uint result;
  cl_device_id *devices = NULL;
  cl_int return_code = CL_SUCCESS;
  
  if (properties != NULL) {
    return_code = CL_INVALID_VALUE;
    goto error;
  }
  
  result = clGetDeviceIDs(device_type, 0, NULL, &num_devices);
  switch (result) {
  case CL_INVALID_DEVICE_TYPE:
  case CL_DEVICE_NOT_FOUND:
    return_code = result;
    goto error;
    
  default:
    break;
  }
  /* TODO: handle CL_DEVICE_NOT_AVAILABLE */
  devices = clOsalCalloc(num_devices * sizeof(cl_device_id));
  if (NULL == devices) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  result = clGetDeviceIDs(device_type, num_devices, devices, NULL);
  
  context = clOsalCalloc(sizeof(struct _cl_context));
  if (NULL == context) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  context->devices = clOsalCalloc(num_devices * sizeof(cl_device_id));
  if (NULL == context->devices) {
    return_code = CL_OUT_OF_HOST_MEMORY;
    goto error;
  }
  memcpy(context->devices, devices, num_devices * sizeof(cl_device_id));
  {
    cl_int const result = clInitContext(context, pfn_notify, user_data);
    switch (result) {
    case CL_SUCCESS:
      break;
    case CL_OUT_OF_HOST_MEMORY:
      return_code = CL_OUT_OF_HOST_MEMORY;
      goto error;
    default:
      ASSERT(0);
      break;
    }
  }
  clRetainContext(context);
  return_code = CL_SUCCESS;
  goto success;
  
 error:
  if (context != NULL) {
    clOsalFree(context);
    context = NULL;
  }
  if (devices != NULL) {
    clOsalFree(devices);
    devices = NULL;
  }
  
 success:
  if (errcode_ret != NULL) {
    return_code = return_code;
  }
  return context;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainContext(cl_context context) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == context) {
    return CL_INVALID_CONTEXT;
  }
  ++context->ref_count;
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseContext(cl_context context) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == context) {
    return CL_INVALID_CONTEXT;
  }
  --context->ref_count;
  if (0 == context->ref_count) {
    cl_command_queue *cmd_queue;
    
    clOsalFree(context->devices);
    for (cmd_queue = cvector_cl_command_queue_begin(context->command_queues);
         cmd_queue != cvector_cl_command_queue_end(context->command_queues);
         ++cmd_queue) {
      clReleaseCommandQueue(*cmd_queue);
    }
    /* TODO: release memory object */
    clOsalFree(context);
  }
  return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetContextInfo(cl_context         context, 
                 cl_context_info    param_name, 
                 size_t             param_value_size, 
                 void *             param_value, 
                 size_t *           param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
  if (NULL == context) {
    return CL_INVALID_CONTEXT;
  }
  switch (param_name) {
  case CL_CONTEXT_REFERENCE_COUNT:
    if (param_value != NULL) {
      if (param_value_size < sizeof(context->ref_count)) {
        return CL_INVALID_VALUE;
      }
      memcpy(param_value, &(context->ref_count), sizeof(context->ref_count));
    }
    if (param_value_size_ret != NULL) {
      (*param_value_size_ret) = sizeof(context->ref_count);
    }
    break;
    
  case CL_CONTEXT_DEVICES:
  case CL_CONTEXT_PROPERTIES:
    break;
    
  default:
    return CL_INVALID_VALUE;
  }
  return CL_SUCCESS;
}
