#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

#include "ModuleLoader.h"
#include "library_loading.h"

#ifdef _MSC_VER
#   define DATAFLOW_CALLING_CONVENTION __cdecl
#else
#   define DATAFLOW_CALLING_CONVENTION
#endif

typedef const char * (DATAFLOW_CALLING_CONVENTION * fun_module_type)();
typedef const char * (DATAFLOW_CALLING_CONVENTION * fun_types_type)();
typedef const char * (DATAFLOW_CALLING_CONVENTION * fun_operations_type)();
typedef bool (DATAFLOW_CALLING_CONVENTION * fun_constructor_type)(const char *, void *);
typedef bool (DATAFLOW_CALLING_CONVENTION * fun_execute_type)(void * const *, void * const *);
typedef unsigned int (DATAFLOW_CALLING_CONVENTION * fun_size_type)();

class LibraryLoader : public ModuleLoader
{
private:
    bool loaded;
    DATAFLOW_LIBRARY dllId;
    std::string name;
    std::vector<std::string> types;
    std::vector<std::string> operations;
    std::string error;
    std::unordered_map<std::string, unsigned int> typeSizes;
    std::unordered_map<std::string, fun_constructor_type> constructors;
    std::unordered_map<std::string, std::vector<std::string> > inputs;
    std::unordered_map<std::string, std::vector<std::string> > outputs;
    std::unordered_map<std::string, fun_execute_type> executes;

public:
    LibraryLoader();
    ~LibraryLoader();

    bool load(const std::string moduleName);
    void clear();
    const std::string & getLastError();

    const std::string & getName();
    const std::vector<std::string> & getTypes();
    const std::vector<std::string> & getOperations();
    const std::unordered_map<std::string, unsigned int> & getSizes();
    const std::unordered_map<std::string, std::vector<std::string> > & getInputs();
    const std::unordered_map<std::string, std::vector<std::string> > & getOutputs();

    bool constructType(const std::string & typeName, const std::string & data, void * out);
    bool execute(const std::string & operationName, const std::vector<void *> & inputs, const std::vector<void *> & outputs);

private:
    bool getType(const char * typeName);
    bool getOperation(const char * operationName);
};

#endif
