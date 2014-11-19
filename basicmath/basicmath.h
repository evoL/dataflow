#ifndef BASICMATH_H
#define BASICMATH_H

#include <dataflow.h>

#ifdef BASIC_MATH_EXPORTS
#   define BASIC_MATH_API DATAFLOW_MODULE_EXPORT
#else
#   define BASIC_MATH_API DATAFLOW_MODULE_IMPORT
#endif

extern "C"
{
    BASIC_MATH_API const char * DATAFLOW_MODULE module();
    BASIC_MATH_API const char * DATAFLOW_MODULE types();
    BASIC_MATH_API const char * DATAFLOW_MODULE operations();
    BASIC_MATH_API unsigned int DATAFLOW_MODULE Integer_size();
    BASIC_MATH_API bool DATAFLOW_MODULE Integer_construct(const char * data, void * output);
    BASIC_MATH_API bool DATAFLOW_MODULE Real_construct(const char * data, void * output);
    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_outputs();
    BASIC_MATH_API bool AddIntegers_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool AddReals_execute(void * const * inputs, void * const * outputs);
}

#endif
