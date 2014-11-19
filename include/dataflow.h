#ifndef DATAFLOW_H
#define DATAFLOW_H

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

#endif
