#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <string>
#include <vector>
#include <unordered_map>

class ModuleLoader
{
public:
    virtual bool load(const std::string moduleName) = 0;
    virtual void clear() = 0;
    virtual const std::string & getLastError() = 0;

    virtual const std::string & getName() = 0;
    virtual const std::vector<std::string> & getTypes() = 0;
    virtual const std::vector<std::string> & getOperations() = 0;
    virtual const std::unordered_map<std::string, unsigned int> & getSizes() = 0;
    virtual const std::unordered_map<std::string, std::vector<std::string> > & getInputs() = 0;
    virtual const std::unordered_map<std::string, std::vector<std::string> > & getOutputs() = 0;

    virtual bool constructType(const std::string & typeName, const std::string & data, void * out) = 0;
    virtual bool execute(const std::string & operationName, const std::vector<void *> & inputs, const std::vector<void *> & outputs) = 0;
};

#endif
