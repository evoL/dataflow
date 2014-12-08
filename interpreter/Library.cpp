#include "Library.h"

Library::~Library()
{
    dataflow::library_free(dllId);
}

const std::string & Library::getName() const
{
    return name;
}

const std::vector<std::string> & Library::getTypes() const
{
    return types;
}

const std::vector<std::string> & Library::getOperations() const
{
    return operations;
}

const std::unordered_map<std::string, unsigned int> & Library::getSizes() const
{
    return typeSizes;
}

const std::unordered_map<std::string, std::vector<std::string> > & Library::getInputs() const
{
    return inputs;
}

const std::unordered_map<std::string, std::vector<std::string> > & Library::getOutputs() const
{
    return outputs;
}

void Library::constructType(const std::string & typeName, const std::string & data, void * out) const
{
    auto res = constructors.find(typeName);

    if (res == constructors.end()) {
        throw LibraryError("Unknown type");
    }

    bool succeeded = (*res->second)(data.c_str(), out);

    if (!succeeded) {
        throw LibraryError("Runtime error: constructor '" + typeName + "'");
    }
}

void Library::destructType(const std::string & typeName, void * data) const
{
    auto res = destructors.find(typeName);

    if (res == destructors.end()) return;

    bool succeeded = (*res->second)(data);

    if (!succeeded) {
        throw LibraryError("Runtime error: destructor '" + typeName = "'");
    }
}

void Library::execute(const std::string & operationName, const std::vector<void *> & in, const std::vector<void *> & out) const
{
    auto res = executes.find(operationName);

    if (res == executes.end()) {
        throw LibraryError("Unknown operation");
    }

    bool succeeded = (*res->second)(in.data(), out.data());

    if (!succeeded) {
        throw LibraryError("Runtime error: operation '" + operationName + "'");
    }
}
