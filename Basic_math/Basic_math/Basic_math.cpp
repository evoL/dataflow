#include "stdafx.h"
#include "Basic_math.h"

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
		return "AddInteger\0AddReal\0";
	}
}