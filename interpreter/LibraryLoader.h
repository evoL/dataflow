#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

#include "library_loading.h"
#include "Library.h"
#include <string>
#include <stdexcept>
#include "interpreter_export.h"

class INTERPRETER_EXPORT LibraryLoadError : public std::runtime_error
{
public:
    LibraryLoadError(const std::string & what): std::runtime_error(what) {}
};

class INTERPRETER_EXPORT LibraryLoader
{
public:
    static Library load(const std::string moduleName);

private:
    static void fillType(Library & library, const char * typeName);
    static void fillOperation(Library & library, const char * operationName);
};

#endif
