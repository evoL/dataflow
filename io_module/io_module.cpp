#include "io_module.h"
#include <cstdlib>
#include <cstdio>

extern "C"
{
    BASIC_IO_API const char * DATAFLOW_MODULE module()
    {
        return "IO";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE types()
    {
        return "\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE operations()
    {
        return "PrintInteger\0PrintReal\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintInteger_inputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintReal_inputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintInteger_outputs()
    {
        return "\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintReal_outputs()
    {
        return "\0";
    }

    BASIC_IO_API bool PrintInteger_execute(void * const * inputs, void * const *)
    {
        dInteger value = *(dInteger *)(inputs[0]);
        printf("%d\n", value);
        return true;
    }

    BASIC_IO_API bool PrintReal_execute(void * const * inputs, void * const *)
    {
        dReal value = *(dReal *)(inputs[0]);
        printf("%f\n", value);
        return true;
    }
}
