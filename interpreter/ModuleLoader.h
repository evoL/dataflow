#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <string>
#include <vector>
#include <unordered_map>

class ModuleLoader
{
public:
    virtual bool                                                                load(const std::string module_name) = 0;
    virtual void                                                                clear() = 0;
    virtual const std::string&                                                  get_last_error() = 0;

    virtual const std::string&                                                  get_name() = 0;
    virtual const std::vector<std::string>&                                     get_types() = 0;
    virtual const std::vector<std::string>&                                     get_operations() = 0;
    virtual const std::unordered_map<std::string, unsigned int>&                get_sizes() = 0;
    virtual const std::unordered_map<std::string, std::vector<std::string> >&   get_inputs() = 0;
    virtual const std::unordered_map<std::string, std::vector<std::string> >&   get_outputs() = 0;

    virtual bool                                                                construct_type(const std::string& type_name, const std::string& data, void * out) = 0;
    virtual bool                                                                execute(const std::string& operation_name, const std::vector<void *>& inputs, const std::vector<void *>& outputs) = 0;
};

#endif
