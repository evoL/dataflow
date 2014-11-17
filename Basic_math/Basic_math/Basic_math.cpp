#include "stdafx.h"
#include "Basic_math.h"
#include <cstdlib>

extern "C"
{
	BASIC_MATH_API const char * __cdecl module()
	{
		return "Basic_math";
	}

	BASIC_MATH_API const char * __cdecl types()
	{
		return "Integer\0Real\0";
	}

	BASIC_MATH_API const char * __cdecl operations()
	{
		return "AddIntegers\0AddReals\0";
	}

	BASIC_MATH_API unsigned int __cdecl Integer_size()
	{
		return sizeof(int);
	}

	BASIC_MATH_API unsigned int __cdecl Real_size()
	{
		return sizeof(double);
	}

	BASIC_MATH_API bool __cdecl Integer_construct(const char * data, void * output)
	{
		*(int *)output = atoi(data);
		return true;
	}

	BASIC_MATH_API bool __cdecl Real_construct(const char * data, void * output)
	{
		*(double *)output = atof(data);
		return true;
	}

	BASIC_MATH_API const char * __cdecl AddIntegers_inputs()
	{
		return "Basic_math.Integer\0Basic_math.Integer\0";
	}

	BASIC_MATH_API const char * __cdecl AddReals_inputs()
	{
		return "Basic_math.Real\0Basic_math.Real\0";
	}

	BASIC_MATH_API const char * __cdecl AddIntegers_outputs()
	{
		return "Basic_math.Integer\0";
	}

	BASIC_MATH_API const char * __cdecl AddReals_outputs()
	{
		return "Basic_math.Real\0";
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