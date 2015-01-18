#include "io_module.h"
#include <iostream>

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
        return "PrintInteger\0PrintReal\0PrintComplex\0PrintRational\0PrintString\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintInteger_inputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintReal_inputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintComplex_inputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintRational_inputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintString_inputs()
    {
        return "dstring.String\0";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintInteger_outputs()
    {
        return "";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintReal_outputs()
    {
        return "";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintComplex_outputs()
    {
        return "";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintRational_outputs()
    {
        return "";
    }

    BASIC_IO_API const char * DATAFLOW_MODULE PrintString_outputs()
    {
        return "";
    }

    BASIC_IO_API bool PrintInteger_execute(void * const * inputs, void * const *)
    {
        if (inputs == NULL) return false;

        dInteger& value = *(dInteger *)(inputs[0]);
        std::cout << value << "\n";
        return true;
    }

    BASIC_IO_API bool PrintReal_execute(void * const * inputs, void * const *)
    {
        if (inputs == NULL) return false;

        dReal& value = *(dReal *)(inputs[0]);
        std::cout << value << "\n";
        return true;
    }

    BASIC_IO_API bool PrintComplex_execute(void * const * inputs, void * const *)
    {
        if (inputs == NULL) return false;

        dComplex& value = *(dComplex *)(inputs[0]);
        std::cout << value.re << " " << value.im << "i\n";
        return true;
    }

    BASIC_IO_API bool PrintRational_execute(void * const * inputs, void * const *)
    {
        if (inputs == NULL) return false;

        dRational& value = *(dRational *)(inputs[0]);
        std::cout << value.numerator << " / " << value.denominator << "\n";
        return true;
    }

    BASIC_IO_API bool PrintString_execute(void * const * inputs, void * const *)
    {
        if (inputs == NULL) return false;

        dString& value = *(dString *)(inputs[0]);
        std::cout << value << "\n";
        return true;
    }
}
