#include "LibraryLoader.h"

#include <cstring>

LibraryLoader::LibraryLoader():
    loaded(false),
    dllId(0)
{

}

LibraryLoader::~LibraryLoader()
{
    if (dllId) dataflow::library_free(dllId);
}

bool LibraryLoader::load(const std::string moduleName)
{
    dllId = dataflow::library_load(moduleName);

    if (!dllId) {
        error = "Library not found";
        return false;
    }

    // Module name

    fun_module_type moduleFun = (fun_module_type)dataflow::library_procedure(dllId, "module");

    if (!moduleFun) {
        error = "Function 'module' not found";
        return false;
    }

    name = (*moduleFun)();


    // Types

    fun_types_type typesFun = (fun_types_type)dataflow::library_procedure(dllId, "types");

    if (!typesFun) {
        error = "Function 'types' not found";
        return false;
    }

    const char * typesData = (*typesFun)();

    while (*typesData) {
        if (!getType(typesData))
            return false;

        typesData += strlen(typesData) + 1;
    }


    // Operations

    fun_operations_type operationsFun = (fun_operations_type)dataflow::library_procedure(dllId, "operations");

    if (!operationsFun) {
        error = "Function 'operations' not found";
        return false;
    }

    const char * operationsData = (*operationsFun)();

    while (*operationsData) {
        if (!getOperation(operationsData))
            return false;

        operationsData += strlen(operationsData) + 1;
    }

    return true;
}

void LibraryLoader::clear()
{
    if (dllId) dataflow::library_free(dllId);

    loaded = false;
    dllId = 0;
    name.clear();
    types.clear();
    operations.clear();
    error.clear();
    typeSizes.clear();
    constructors.clear();
    inputs.clear();
    outputs.clear();
    executes.clear();
}

const std::string & LibraryLoader::getLastError()
{
    return error;
}

const std::string & LibraryLoader::getName()
{
    return name;
}

const std::vector<std::string> & LibraryLoader::getTypes()
{
    return types;
}

const std::vector<std::string> & LibraryLoader::getOperations()
{
    return operations;
}

const std::unordered_map<std::string, unsigned int> & LibraryLoader::getSizes()
{
    return typeSizes;
}

const std::unordered_map<std::string, std::vector<std::string> > & LibraryLoader::getInputs()
{
    return inputs;
}

const std::unordered_map<std::string, std::vector<std::string> > & LibraryLoader::getOutputs()
{
    return outputs;
}

bool LibraryLoader::constructType(const std::string & typeName, const std::string & data, void * out)
{
    auto res = constructors.find(typeName);

    if (res == constructors.end()) {
        error = "Unknown type";
        return false;
    }

    bool succeeded = (*res->second)(data.c_str(), out);

    if (!succeeded) {
        error = "Runtime error: constructor '" + typeName + "'";
        return false;
    }

    return true;
}

bool LibraryLoader::destructType(const std::string & typeName, void * data)
{
    auto res = destructors.find(typeName);

    if (res == destructors.end())
        return true;

    bool succeeded = (*res->second)(data);

    if (!succeeded) {
        error = "Runtime error: destructor '" + typeName = "'";
        return false;
    }

    return true;
}

bool LibraryLoader::execute(const std::string & operationName, const std::vector<void *> & in, const std::vector<void *> & out)
{
    auto res = executes.find(operationName);

    if (res == executes.end()) {
        error = "Unknown operation";
        return false;
    }

    bool succeeded = (*res->second)(in.data(), out.data());

    if (!succeeded) {
        error = "Runtime error: operation '" + operationName + "'";
        return false;
    }

    return true;
}

bool LibraryLoader::getType(const char * typeName)
{
    std::string typeNameStr = typeName;

    if (typeSizes.find(typeNameStr) != typeSizes.end())
        return true;

    // type size

    std::string typeSizeStr = typeNameStr + "_size";
    fun_size_type sizeFun = (fun_size_type)dataflow::library_procedure(dllId, typeSizeStr.c_str());

    if (!sizeFun) {
        error = "Function '" + typeSizeStr + "' not found";
        return false;
    }

    unsigned int size = (*sizeFun)();

    // construct function

    std::string typeConstructStr = typeNameStr + "_construct";
    fun_constructor_type constructFun = (fun_constructor_type)dataflow::library_procedure(dllId, typeConstructStr.c_str());

    if (!constructFun) {
        error = "Function '" + typeConstructStr + "' not found";
        return false;
    }

    types.push_back(typeName);
    typeSizes[typeNameStr] = size;
    constructors[typeNameStr] = constructFun;

    // destruct function (if exists)

    std::string typeDestructStr = typeNameStr + "_destruct";
    fun_destructor_type destructFun = (fun_destructor_type)dataflow::library_procedure(dllId, typeDestructStr.c_str());

    if (destructFun)
        destructors[typeNameStr] = destructFun;

    return true;
}

bool LibraryLoader::getOperation(const char * operationName)
{
    std::string operationNameStr = operationName;

    if (inputs.find(operationNameStr) != inputs.end())
        return true;

    // input arguments

    std::string operationInputsStr = operationNameStr + "_inputs";
    fun_types_type inputsFun = (fun_types_type)dataflow::library_procedure(dllId, operationInputsStr.c_str());

    if (!inputsFun) {
        error = "Function '" + operationInputsStr + "' not found";
        return false;
    }

    const char * inputsData = (*inputsFun)();

    std::vector<std::string> inputsTemp;

    while (*inputsData) {
        inputsTemp.push_back(inputsData);
        inputsData += strlen(inputsData) + 1;
    }


    // output arguments

    std::string operationOutputsStr = operationNameStr + "_outputs";
    fun_types_type outputsStr = (fun_types_type)dataflow::library_procedure(dllId, operationOutputsStr.c_str());

    if (!outputsStr) {
        error = "Function '" + operationOutputsStr + "' not found";
        return false;
    }

    const char * outputsData = (*outputsStr)();

    std::vector<std::string> outputsTemp;

    while (*outputsData) {
        outputsTemp.push_back(outputsData);
        outputsData += strlen(outputsData) + 1;
    }


    // execute function

    std::string executeStr = operationNameStr + "_execute";
    fun_execute_type executeFun = (fun_execute_type)dataflow::library_procedure(dllId, executeStr.c_str());

    if (!executeFun) {
        error = "Function '" + executeStr + "' not found";
        return false;
    }

    operations.push_back(operationName);
    inputs[operationNameStr] = inputsTemp;
    outputs[operationNameStr] = outputsTemp;
    executes[operationNameStr] = executeFun;

    return true;
}
