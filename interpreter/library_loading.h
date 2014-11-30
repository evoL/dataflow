#ifndef LIBRARY_LOADING_H
#define LIBRARY_LOADING_H

#include <string>

#ifdef _WIN32
#   include <Windows.h>
#   define DATAFLOW_LIBRARY HMODULE
#else
#   define DATAFLOW_LIBRARY void *
#endif

namespace dataflow
{
    DATAFLOW_LIBRARY library_load(const std::string & libraryName);
    void * library_procedure(DATAFLOW_LIBRARY library, const std::string & name);
    bool   library_free(DATAFLOW_LIBRARY library);
}

#endif
