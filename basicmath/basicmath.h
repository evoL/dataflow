#ifndef BASICMATH_H
#define BASICMATH_H

#include <dataflow.h>

#ifdef BASIC_MATH_EXPORTS
#   define BASIC_MATH_API DATAFLOW_MODULE_EXPORT
#else
#   define BASIC_MATH_API DATAFLOW_MODULE_IMPORT
#endif

#pragma pack(1)

struct dComplex {
    dReal re;
    dReal im;
};

struct dRational {
    dInteger numerator;
    dInteger denominator;
};

#pragma pack()

extern "C"
{
    BASIC_MATH_API const char * DATAFLOW_MODULE module();
    BASIC_MATH_API const char * DATAFLOW_MODULE types();
    BASIC_MATH_API const char * DATAFLOW_MODULE operations();
    BASIC_MATH_API unsigned int DATAFLOW_MODULE Integer_size();
    BASIC_MATH_API unsigned int DATAFLOW_MODULE Real_size();
    BASIC_MATH_API unsigned int DATAFLOW_MODULE Rational_size();
    BASIC_MATH_API unsigned int DATAFLOW_MODULE Complex_size();
    BASIC_MATH_API bool DATAFLOW_MODULE Integer_construct(const char * data, void * output);
    BASIC_MATH_API bool DATAFLOW_MODULE Real_construct(const char * data, void * output);
    BASIC_MATH_API bool DATAFLOW_MODULE Rational_construct(const char * data, void * output);
    BASIC_MATH_API bool DATAFLOW_MODULE Complex_construct(const char * data, void * ouput);
    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddRationals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddComplex_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubIntegers_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubReals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubRationals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubComplex_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultIntegers_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultReals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultRationals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultComplex_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivIntegers_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivReals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivRationals_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivComplex_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromInteger_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE IntegerFromReal_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromRational_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromInteger_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromReal_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromRational_inputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddRationals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE AddComplex_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubIntegers_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubReals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubRationals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE SubComplex_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultIntegers_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultReals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultRationals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE MultComplex_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivIntegers_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivReals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivRationals_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE DivComplex_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromInteger_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE IntegerFromReal_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromRational_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromInteger_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromReal_outputs();
    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromRational_outputs();
    BASIC_MATH_API bool DATAFLOW_MODULE AddIntegers_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE AddReals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE AddRationals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE AddComplex_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE SubIntegers_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE SubReals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE SubRationals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE SubComplex_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE MultIntegers_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE MultReals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE MultRationals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE MultComplex_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE DivIntegers_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE DivReals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE DivRationals_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE DivComplex_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE RealFromInteger_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE IntegerFromReal_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE RealFromRational_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE ComplexFromInteger_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE ComplexFromReal_execute(void * const * inputs, void * const * outputs);
    BASIC_MATH_API bool DATAFLOW_MODULE ComplexFromRational_execute(void * const * inputs, void * const * outputs);

    // private functions
    void normalize_rational(dRational & n);
    unsigned int gcd(unsigned int a, unsigned int b);
    unsigned int lcm(unsigned int a, unsigned int b);
}

#endif
