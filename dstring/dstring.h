#ifndef BASICMATH_H
#define BASICMATH_H

#include <dataflow.h>

#ifdef DSTRING_EXPORTS
#   define DSTRING_API DATAFLOW_MODULE_EXPORT
#else
#   define DSTRING_API DATAFLOW_MODULE_IMPORT
#endif

extern "C"
{
    DSTRING_API const char * DATAFLOW_MODULE module();
    DSTRING_API const char * DATAFLOW_MODULE types();
    DSTRING_API const char * DATAFLOW_MODULE operations();
    DSTRING_API unsigned int DATAFLOW_MODULE String_size();
    DSTRING_API bool DATAFLOW_MODULE String_construct(const char * data, void * output);
    DSTRING_API bool DATAFLOW_MODULE String_destruct(void * data);
}

#endif
