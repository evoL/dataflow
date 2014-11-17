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
	BASIC_MATH_API unsigned int __cdecl Integer_size();
	BASIC_MATH_API bool __cdecl Integer_construct(const char * data, void * output);
	BASIC_MATH_API bool __cdecl Real_construct(const char * data, void * output);
	BASIC_MATH_API const char * __cdecl AddIntegers_inputs();
	BASIC_MATH_API const char * __cdecl AddReals_inputs();
	BASIC_MATH_API const char * __cdecl AddIntegers_outputs();
	BASIC_MATH_API const char * __cdecl AddReals_outputs();
	BASIC_MATH_API bool AddIntegers_execute(void * const * inputs, void * const * outputs);
	BASIC_MATH_API bool AddReals_execute(void * const * inputs, void * const * outputs);
}
