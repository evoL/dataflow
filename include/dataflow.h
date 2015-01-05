#ifndef DATAFLOW_H
#define DATAFLOW_H

#include <cstdint>

// Exporting DLL symbols for Visual C++
#ifdef _MSC_VER
#   define DATAFLOW_MODULE_EXPORT __declspec(dllexport)
#   define DATAFLOW_MODULE_IMPORT __declspec(dllimport)
#   define DATAFLOW_MODULE __cdecl
#else
#   define DATAFLOW_MODULE_EXPORT
#   define DATAFLOW_MODULE_IMPORT
#   define DATAFLOW_MODULE
#endif

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif


// Basic dataflow types
typedef int32_t dInteger;
static_assert(sizeof(dInteger) == 4, "dInteger is not of size 4");

typedef float dReal;
static_assert(sizeof(dReal) == 4, "dReal is not of size 4");

typedef char * dString;
#ifdef ENVIRONMENT64
static_assert(sizeof(dString) == 8, "dString is not of size 8");
#elif defined ENVIRONMENT32
static_assert(sizeof(dString) == 4, "dString is not of size 4");
#else
#error 
#endif


#endif
