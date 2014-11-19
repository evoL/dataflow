#include "basicmath.h"
#include <cstdlib>

extern "C"
{
    BASIC_MATH_API const char * DATAFLOW_MODULE module()
    {
        return "basicmath";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE types()
    {
        return "Integer\0Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE operations()
    {
        return "AddIntegers\0AddReals\0";
    }

    BASIC_MATH_API unsigned int DATAFLOW_MODULE Integer_size()
    {
        return sizeof(int);
    }

    BASIC_MATH_API unsigned int DATAFLOW_MODULE Real_size()
    {
        return sizeof(double);
    }

    BASIC_MATH_API bool DATAFLOW_MODULE Integer_construct(const char * data, void * output)
    {
        *(int *)output = atoi(data);
        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE Real_construct(const char * data, void * output)
    {
        *(double *)output = atof(data);
        return true;
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_inputs()
    {
        return "basicmath.Integer\0basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_inputs()
    {
        return "basicmath.Real\0basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_outputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API bool AddIntegers_execute(void * const * inputs, void * const * outputs)
    {
        *(int *)(outputs[0]) = *(int *)(inputs[0]) + *(int *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool AddReals_execute(void * const * inputs, void * const * outputs)
    {
        *(double *)(outputs[0]) = *(double *)(inputs[0]) + *(double *)(inputs[1]);
        return true;
    }
}
