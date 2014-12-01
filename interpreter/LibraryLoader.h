#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

#include "library_loading.h"
#include "Library.h"
#include <string>
#include <stdexcept>

class LibraryLoadError : public std::runtime_error
{
public:
    LibraryLoadError(const std::string& what): std::runtime_error(what) {}
};

class LibraryLoader
{
public:
    Library load(const std::string moduleName);

private:
    void fillType(Library & library, const char * typeName);
    void fillOperation(Library & library, const char * operationName);
};

#endif
