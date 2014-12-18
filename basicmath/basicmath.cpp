#include "basicmath.h"
#include <cstdlib>
#include <cstdio>

extern "C"
{
    BASIC_MATH_API const char * DATAFLOW_MODULE module()
    {
        return "basicmath";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE types()
    {
        return "Integer\0Real\0Rational\0Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE operations()
    {
        return "AddIntegers\0AddReals\0AddRationals\0AddComplex\0SubIntegers\0SubReals\0SubRationals\0SubComplex\0MultIntegers\0MultReals\0MultRationals\0MultComplex\0DivIntegers\0DivReals\0DivRationals\0DivComplex\0RealFromInteger\0RealFromRational\0ComplexFromInteger\0ComplexFromReal\0ComplexFromRational\0";
    }

    BASIC_MATH_API unsigned int DATAFLOW_MODULE Integer_size()
    {
        return sizeof(dInteger);
    }

    BASIC_MATH_API unsigned int DATAFLOW_MODULE Real_size()
    {
        return sizeof(dReal);
    }

    BASIC_MATH_API unsigned int DATAFLOW_MODULE Rational_size()
    {
        return sizeof(dRational);
    }

    BASIC_MATH_API unsigned int DATAFLOW_MODULE Complex_size()
    {
        return sizeof(dComplex);
    }

    BASIC_MATH_API bool DATAFLOW_MODULE Integer_construct(const char * data, void * output)
    {
        *(dInteger *)output = atoi(data);
        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE Real_construct(const char * data, void * output)
    {
        *(dReal *)output = (dReal)atof(data);
        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE Rational_construct(const char * data, void * output)
    {
        dRational& r = *(dRational *)output;

        if (sscanf(data, "%d %d", &r.numerator, &r.denominator) < 2)
            return false;

        if (r.denominator == 0)
            return false;

        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE Complex_construct(const char * data, void * output)
    {
        dComplex& z = *(dComplex *)output;

        if (sscanf(data, "%d %d", &z.re, &z.im) < 2)
            return false;

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

    BASIC_MATH_API const char * DATAFLOW_MODULE AddRationals_inputs()
    {
        return "basicmath.Rational\0basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddComplex_inputs()
    {
        return "basicmath.Complex\0basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubIntegers_inputs()
    {
        return "basicmath.Integer\0basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubReals_inputs()
    {
        return "basicmath.Real\0basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubRationals_inputs()
    {
        return "basicmath.Rational\0basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubComplex_inputs()
    {
        return "basicmath.Complex\0basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultIntegers_inputs()
    {
        return "basicmath.Integer\0basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultReals_inputs()
    {
        return "basicmath.Real\0basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultRationals_inputs()
    {
        return "basicmath.Rational\0basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultComplex_inputs()
    {
        return "basicmath.Complex\0basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivIntegers_inputs()
    {
        return "basicmath.Integer\0basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivReals_inputs()
    {
        return "basicmath.Real\0basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivRationals_inputs()
    {
        return "basicmath.Rational\0basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivComplex_inputs()
    {
        return "basicmath.Complex\0basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromInteger_inputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE IntegerFromReal_inputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromRational_inputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromInteger_inputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromReal_inputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromRational_inputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddIntegers_outputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddReals_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddRationals_outputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE AddComplex_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubIntegers_outputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubReals_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubRationals_outputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE SubComplex_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultIntegers_outputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultReals_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultRationals_outputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE MultComplex_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivIntegers_outputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivReals_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivRationals_outputs()
    {
        return "basicmath.Rational\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE DivComplex_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromInteger_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE IntegerFromReal_outputs()
    {
        return "basicmath.Integer\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE RealFromRational_outputs()
    {
        return "basicmath.Real\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromInteger_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromReal_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API const char * DATAFLOW_MODULE ComplexFromRational_outputs()
    {
        return "basicmath.Complex\0";
    }

    BASIC_MATH_API bool AddIntegers_execute(void * const * inputs, void * const * outputs)
    {
        *(dInteger *)(outputs[0]) = *(dInteger *)(inputs[0]) + *(dInteger *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool AddReals_execute(void * const * inputs, void * const * outputs)
    {
        *(dReal *)(outputs[0]) = *(dReal *)(inputs[0]) + *(dReal *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool AddRationals_execute(void * const * inputs, void * const * outputs)
    {
        dRational& r1 = *(dRational *)(inputs[0]);
        dRational& r2 = *(dRational *)(inputs[1]);
        dRational& res = *(dRational *)(outputs[0]);

        res.numerator = r1.numerator * r2.denominator + r2.numerator * r1.denominator;
        res.denominator = r1.denominator * r2.denominator;
        normalize_rational(res);

        return true;
    }

    BASIC_MATH_API bool AddComplex_execute(void * const * inputs, void * const * outputs)
    {
        dComplex& z1 = *(dComplex *)(inputs[0]);
        dComplex& z2 = *(dComplex *)(inputs[1]);
        dComplex& res = *(dComplex *)(outputs[0]);

        res.re = z1.re + z2.re;
        res.im = z1.im + z2.im;

        return true;
    }

    BASIC_MATH_API bool SubIntegers_execute(void * const * inputs, void * const * outputs)
    {
        *(dInteger *)(outputs[0]) = *(dInteger *)(inputs[0]) - *(dInteger *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool SubReals_execute(void * const * inputs, void * const * outputs)
    {
        *(dReal *)(outputs[0]) = *(dReal *)(inputs[0]) - *(dReal *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool SubRationals_execute(void * const * inputs, void * const * outputs)
    {
        dRational& r1 = *(dRational *)(inputs[0]);
        dRational& r2 = *(dRational *)(inputs[1]);
        dRational& res = *(dRational *)(outputs[0]);

        res.numerator = r1.numerator * r2.denominator - r2.numerator * r1.denominator;
        res.denominator = r1.denominator * r2.denominator;
        normalize_rational(res);

        return true;
    }

    BASIC_MATH_API bool SubComplex_execute(void * const * inputs, void * const * outputs)
    {
        dComplex& z1 = *(dComplex *)(inputs[0]);
        dComplex& z2 = *(dComplex *)(inputs[1]);
        dComplex& res = *(dComplex *)(outputs[0]);

        res.re = z1.re - z2.re;
        res.im = z1.im - z2.im;

        return true;
    }

    BASIC_MATH_API bool MultIntegers_execute(void * const * inputs, void * const * outputs)
    {
        *(dInteger *)(outputs[0]) = *(dInteger *)(inputs[0]) * *(dInteger *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool MultReals_execute(void * const * inputs, void * const * outputs)
    {
        *(dReal *)(outputs[0]) = *(dReal *)(inputs[0]) * *(dReal *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool MultRationals_execute(void * const * inputs, void * const * outputs)
    {
        dRational& r1 = *(dRational *)(inputs[0]);
        dRational& r2 = *(dRational *)(inputs[1]);
        dRational& res = *(dRational *)(outputs[0]);

        res.numerator = r1.numerator * r2.numerator;
        res.denominator = r1.denominator * r2.denominator;
        normalize_rational(res);

        return true;
    }

    BASIC_MATH_API bool MultComplex_execute(void * const * inputs, void * const * outputs)
    {
        dComplex& z1 = *(dComplex *)(inputs[0]);
        dComplex& z2 = *(dComplex *)(inputs[1]);
        dComplex& res = *(dComplex *)(outputs[0]);

        res.re = z1.re * z2.re - z1.im * z2.im;
        res.im = z1.im * z2.re + z2.im * z1.re;

        return true;
    }

    BASIC_MATH_API bool DivIntegers_execute(void * const * inputs, void * const * outputs)
    {
        *(dInteger *)(outputs[0]) = *(dInteger *)(inputs[0]) / *(dInteger *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool DivReals_execute(void * const * inputs, void * const * outputs)
    {
        *(dReal *)(outputs[0]) = *(dReal *)(inputs[0]) / *(dReal *)(inputs[1]);
        return true;
    }

    BASIC_MATH_API bool DivRationals_execute(void * const * inputs, void * const * outputs)
    {
        dRational& r1 = *(dRational *)(inputs[0]);
        dRational& r2 = *(dRational *)(inputs[1]);
        dRational& res = *(dRational *)(outputs[0]);

        res.numerator = r1.numerator * r2.denominator;
        if ((res.denominator = r1.denominator * r2.numerator) < 0)
            return false;

        normalize_rational(res);

        return true;
    }

    BASIC_MATH_API bool DivComplex_execute(void * const * inputs, void * const * outputs)
    {
        dComplex& z1 = *(dComplex *)(inputs[0]);
        dComplex& z2 = *(dComplex *)(inputs[1]);
        dComplex& res = *(dComplex *)(outputs[0]);

        dReal divisor = z2.re * z2.re + z2.im * z2.im;
        res.re = (z1.re * z2.re + z1.im * z2.im) / divisor;
        res.im = (z1.im * z2.re - z2.im * z1.re) / divisor;

        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE RealFromInteger_execute(void * const * inputs, void * const * outputs)
    {
        dInteger& i = *(dInteger *)(inputs[0]);
        dReal& r = *(dReal *)(outputs[0]);
        r = (dReal)i;

        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE IntegerFromReal_execute(void * const * inputs, void * const * outputs)
    {
        dReal& r = *(dReal *)(inputs[0]);
        dInteger& i = *(dInteger *)(outputs[0]);
        i = (dInteger)r;

        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE RealFromRational_execute(void * const * inputs, void * const * outputs)
    {
        dRational& ratio = *(dRational *)(inputs[0]);
        dReal& r = *(dReal *)(outputs[0]);

        r = (dReal)ratio.numerator / (dReal)ratio.denominator;
        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE ComplexFromInteger_execute(void * const * inputs, void * const * outputs)
    {
        dInteger& i = *(dInteger *)(inputs[0]);
        dComplex& z = *(dComplex *)(outputs[0]);

        z.re = (dReal)i;
        z.im = 0.0;
        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE ComplexFromReal_execute(void * const * inputs, void * const * outputs)
    {
        dReal& r = *(dReal *)(inputs[0]);
        dComplex& z = *(dComplex *)(outputs[0]);

        z.re = r;
        z.im = 0.0;
        return true;
    }

    BASIC_MATH_API bool DATAFLOW_MODULE ComplexFromRational_execute(void * const * inputs, void * const * outputs)
    {
        dRational& ratio = *(dRational *)(inputs[0]);
        dComplex& z = *(dComplex *)(outputs[0]);

        z.re = (dReal)ratio.numerator / (dReal)ratio.denominator;
        z.im = 0.0;
        return true;
    }


    void normalize_rational(dRational& r)
    {
        int sgn = 1;
        if (r.numerator < 0) {
            sgn = -1;
            r.numerator *= -1;
        }
        if (r.denominator < 0) {
            sgn = - sgn;
            r.denominator *= -1;
        }

        int d = (int)gcd((unsigned)r.numerator, (unsigned)r.denominator);

        r.numerator = sgn * (r.numerator / d);
        r.denominator = r.denominator / d;
    }

    unsigned int gcd(unsigned int a, unsigned int b)
    {
        do {
            if (a < b) {
                int temp = a;
                a = b;
                b = temp;
            }
            a = a % b;
        } while (a);

        return b;
    }

    unsigned int lcm(unsigned int a, unsigned int b)
    {
        return a * b / gcd(a, b);
    }
}
