#include "Library.h"

Library::~Library()
{
    freeDll();
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

Library::Library(Library && other)
{
    dllId = other.dllId;
    ownsDll = other.ownsDll;
    other.ownsDll = false;
    moveFields(std::move(other));
}

Library & Library::operator=(Library && other)
{
    freeDll();
    dllId = other.dllId;
    ownsDll = other.ownsDll;
    other.ownsDll = false;
    moveFields(std::move(other));
    return *this;
}

void Library::freeDll()
{
    if (ownsDll)
        dataflow::library_free(dllId);

    ownsDll = false;
}

void Library::moveFields(Library && other)
{
    name = std::move(other.name);
    types = std::move(other.types);
    operations = std::move(other.operations);
    typeSizes = std::move(other.typeSizes);
    constructors = std::move(other.constructors);
    destructors = std::move(other.destructors);
    inputs = std::move(other.inputs);
    outputs = std::move(other.outputs);
    executes = std::move(other.executes);
}
