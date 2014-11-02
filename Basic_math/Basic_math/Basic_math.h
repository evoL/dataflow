#ifdef BASIC_MATH_EXPORTS
#define BASIC_MATH_API __declspec(dllexport)
#else
#define BASIC_MATH_API __declspec(dllimport)
#endif

extern "C"
{
	BASIC_MATH_API const char * __cdecl module();
	BASIC_MATH_API const char * __cdecl types();
	BASIC_MATH_API const char * __cdecl operations();
}
