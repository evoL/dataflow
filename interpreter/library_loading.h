#ifndef LIBRARY_LOADING_H
#define LIBRARY_LOADING_H

#include <string>

namespace dataflow
{
    void * library_load(const std::string& libraryName);
    void * library_procedure(void * library, const std::string& name);
    bool   library_free(void * library);
}

#endif
