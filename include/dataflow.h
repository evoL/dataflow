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


// Basic dataflow types
typedef int32_t dInteger;
static_assert(sizeof(dInteger) == 4, "dInteger is not of size 4");

typedef float dReal;
static_assert(sizeof(dReal) == 4, "dReal is not of size 4");

#endif
