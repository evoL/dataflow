#ifndef IO_MODULE_H
#define IO_MODULE_H

#include <dataflow.h>
#include <basicmath.h>

#ifdef BASIC_IO_EXPORTS
#   define BASIC_IO_API DATAFLOW_MODULE_EXPORT
#else
#   define BASIC_IO_API DATAFLOW_MODULE_IMPORT
#endif

extern "C"
{
    BASIC_IO_API const char * DATAFLOW_MODULE module();
    BASIC_IO_API const char * DATAFLOW_MODULE types();
    BASIC_IO_API const char * DATAFLOW_MODULE operations();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintInteger_inputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintReal_inputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintComplex_inputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintString_inputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintInteger_outputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintReal_outputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintComplex_outputs();
    BASIC_IO_API const char * DATAFLOW_MODULE PrintString_outputs();
    BASIC_IO_API bool PrintInteger_execute(void * const * inputs, void * const * outputs);
    BASIC_IO_API bool PrintReal_execute(void * const * inputs, void * const * outputs);
    BASIC_IO_API bool PrintComplex_execute(void * const * inputs, void * const * outputs);
    BASIC_IO_API bool PrintString_execute(void * const * inputs, void * const * outputs);
}

#endif
