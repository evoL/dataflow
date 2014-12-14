#ifndef LIBRARY_LOADING_H
#define LIBRARY_LOADING_H

#include <string>
#include "interpreter_export.h"

#ifdef _WIN32
#   include <Windows.h>
#   define DATAFLOW_LIBRARY HMODULE
#   define DATAFLOW_PROCEDURE FARPROC
#else
#   define DATAFLOW_LIBRARY void *
#   define DATAFLOW_PROCEDURE void *
#endif

namespace dataflow
{
    INTERPRETER_NO_EXPORT DATAFLOW_LIBRARY library_load(const std::string & libraryName);
    INTERPRETER_NO_EXPORT DATAFLOW_PROCEDURE library_procedure(DATAFLOW_LIBRARY library, const std::string & name);
    INTERPRETER_NO_EXPORT bool   library_free(DATAFLOW_LIBRARY library);
}

#endif
