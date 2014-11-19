#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

#include "ModuleLoader.h"

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
    bool                                                        loaded;
    void *                                                      dll_id;
    std::string                                                 name;
    std::vector<std::string>                                    types;
    std::vector<std::string>                                    operations;
    std::string                                                 error;
    std::unordered_map<std::string, unsigned int>               type_sizes;
    std::unordered_map<std::string, fun_constructor_type>       constructors;
    std::unordered_map<std::string, std::vector<std::string> >  inputs;
    std::unordered_map<std::string, std::vector<std::string> >  outputs;
    std::unordered_map<std::string, fun_execute_type>           executes;

public:
    LibraryLoader();
    ~LibraryLoader();

    bool                                                                load(const std::string module_name);
    void                                                                clear();
    const std::string&                                                  get_last_error();

    const std::string&                                                  get_name();
    const std::vector<std::string>&                                     get_types();
    const std::vector<std::string>&                                     get_operations();
    const std::unordered_map<std::string, unsigned int>&                get_sizes();
    const std::unordered_map<std::string, std::vector<std::string> >&   get_inputs();
    const std::unordered_map<std::string, std::vector<std::string> >&   get_outputs();

    bool                                                                construct_type(const std::string& type_name, const std::string& data, void * out);
    bool                                                                execute(const std::string& operation_name, const std::vector<void *>& inputs, const std::vector<void *>& outputs);

private:
    bool                                                                get_type(const char * type_name);
    bool                                                                get_operation(const char * operation_name);
};

#endif
