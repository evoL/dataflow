#include "LibraryLoader.h"

Library LibraryLoader::load(const std::string moduleName)
{
    DATAFLOW_LIBRARY dllId = dataflow::library_load(moduleName);

    if (!dllId) {
        throw LibraryLoadError("Library '" + moduleName + "' not found");
    }

    Library library;
    library.dllId = dllId;
    library.ownsDll = true;

    // Module name

    module_list_fn moduleFun = (module_list_fn) dataflow::library_procedure(dllId, "module");

    if (!moduleFun) {
        throw LibraryLoadError("Function 'module' not found");
    }

    library.name = (*moduleFun)();

    // Types

    type_list_fn typesFun = (type_list_fn) dataflow::library_procedure(dllId, "types");

    if (!typesFun) {
        throw LibraryLoadError("Function 'types' not found");
    }

    const char * typesData = (*typesFun)();

    while (*typesData) {
        fillType(library, typesData);
        typesData += strlen(typesData) + 1;
    }

    // Operations

    operation_list_fn operationsFun = (operation_list_fn) dataflow::library_procedure(dllId, "operations");

    if (!operationsFun) {
        throw LibraryLoadError("Function 'operations' not found");
    }

    const char * operationsData = (*operationsFun)();

    while (*operationsData) {
        fillOperation(library, operationsData);
        operationsData += strlen(operationsData) + 1;
    }

    return library;
}

void LibraryLoader::fillType(Library & library, const char * typeName)
{
    std::string typeNameStr = typeName;

    if (library.typeSizes.find(typeNameStr) != library.typeSizes.end())
        return;

    // type size

    std::string typeSizeStr = typeNameStr + "_size";
    type_size_fn sizeFun = (type_size_fn)dataflow::library_procedure(library.dllId, typeSizeStr.c_str());

    if (!sizeFun) {
        throw LibraryLoadError("Function '" + typeSizeStr + "' not found");
    }

    unsigned int size = (*sizeFun)();

    // construct function

    std::string typeConstructStr = typeNameStr + "_construct";
    constructor_fn constructFun = (constructor_fn)dataflow::library_procedure(library.dllId, typeConstructStr.c_str());

    if (!constructFun) {
        throw LibraryLoadError("Function '" + typeConstructStr + "' not found");
    }

    library.types.push_back(typeName);
    library.typeSizes[typeNameStr] = size;
    library.constructors[typeNameStr] = constructFun;

    // destruct function (if exists)

    std::string typeDestructStr = typeNameStr + "_destruct";
    destructor_fn destructFun = (destructor_fn)dataflow::library_procedure(library.dllId, typeDestructStr.c_str());

    if (destructFun)
        library.destructors[typeNameStr] = destructFun;

    return;
}

void LibraryLoader::fillOperation(Library & library, const char * operationName)
{
    std::string operationNameStr = operationName;

    if (library.inputs.find(operationNameStr) != library.inputs.end())
        return;

    // input arguments

    std::string operationInputsStr = operationNameStr + "_inputs";
    type_list_fn inputsFun = (type_list_fn)dataflow::library_procedure(library.dllId, operationInputsStr.c_str());

    if (!inputsFun) {
        throw LibraryLoadError("Function '" + operationInputsStr + "' not found");
    }

    const char * inputsData = (*inputsFun)();

    std::vector<std::string> inputsTemp;

    while (*inputsData) {
        inputsTemp.push_back(inputsData);
        inputsData += strlen(inputsData) + 1;
    }

    // output arguments

    std::string operationOutputsStr = operationNameStr + "_outputs";
    type_list_fn outputsStr = (type_list_fn)dataflow::library_procedure(library.dllId, operationOutputsStr.c_str());

    if (!outputsStr) {
        throw LibraryLoadError("Function '" + operationOutputsStr + "' not found");
    }

    const char * outputsData = (*outputsStr)();

    std::vector<std::string> outputsTemp;

    while (*outputsData) {
        outputsTemp.push_back(outputsData);
        outputsData += strlen(outputsData) + 1;
    }

    // execute function

    std::string executeStr = operationNameStr + "_execute";
    execute_fn executeFun = (execute_fn)dataflow::library_procedure(library.dllId, executeStr.c_str());

    if (!executeFun) {
        throw LibraryLoadError("Function '" + executeStr + "' not found");
    }

    library.operations.push_back(operationName);
    library.inputs[operationNameStr] = inputsTemp;
    library.outputs[operationNameStr] = outputsTemp;
    library.executes[operationNameStr] = executeFun;
}
