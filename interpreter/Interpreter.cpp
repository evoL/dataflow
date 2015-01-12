#include "Interpreter.h"
#include <string>

Interpreter::Interpreter(ProjectModel & model) : model(model)
{

}

void Interpreter::interpret()
{
    datastore.reset();
    auto entryPoints = model.getEntryPoints();

    for (auto point : entryPoints) {
        model.getBlock(point).accept(*this);
    }
}

void Interpreter::visit(const Constructor & constructor)
{
    const Library & library = model.getLibraries().at(constructor.module);
    auto typeSize = library.getSizes().at(constructor.type);

    if (constructor.outputs.size() != 1)
        throw InterpreterError("Invalid constructor output count detected - should be 1.");

    auto outputId = constructor.outputs[0].id;

    auto libraryPtr = &library;
    std::string typeName = constructor.type;
    auto allocatedSpace = datastore.createEntry(outputId, typeSize, [libraryPtr, typeName](void * address) {
        libraryPtr->destructType(typeName, address);
    });

    try {
        library.constructType(constructor.type, constructor.data, allocatedSpace);
    } catch (LibraryError& err) {
        throw InterpreterError(std::string("Library error: ") + err.what());
    }
}

void Interpreter::visit(const Operation & operation)
{
    ensureAllInputsAreComputed(operation);
    allocateOutputs(operation);
    executeOperation(operation);
}

void Interpreter::ensureAllInputsAreComputed(const Operation & operation)
{
    for (auto & input : operation.inputs) {
        if (!datastore.isComputed(input.second.outputId)) {
            input.second.outputBlock->accept(*this);
        }
    }
}

void Interpreter::allocateOutputs(const Operation & operation)
{
    auto & library = model.getLibraries().at(operation.module);
    auto & outputTypes = library.getOutputs().at(operation.name);
    auto libraryPtr = &library;

    if (outputTypes.size() != operation.outputs.size())
        throw InterpreterError("Library outputs information is inconsistent with block output information");

    auto currentType = outputTypes.begin();
    auto currentOutput = operation.outputs.begin();

    while (currentType != outputTypes.end()) {

        std::string currentTypeName{ currentType->begin() + currentType->find_last_of('.') + 1, currentType->end() };

        auto currentTypeSize = library.getSizes().at(currentTypeName);
        datastore.createEntry(currentOutput->id, currentTypeSize, [libraryPtr, currentTypeName](void * address){
            libraryPtr->destructType(currentTypeName, address);
        });

        currentType++;
        currentOutput++;
    }
}

void Interpreter::executeOperation(const Operation & operation)
{
    auto & library = model.getLibraries().at(operation.module);
    std::vector<void *> inputLocations;
    std::vector<void *> outputLocations;

    for (auto & input : operation.inputs)
        inputLocations.push_back(datastore[input.second.outputId]);

    for (auto & output : operation.outputs)
        outputLocations.push_back(datastore[output.id]);

    try {
        library.execute(operation.name, inputLocations, outputLocations);
    } catch (LibraryError& err) {
        throw InterpreterError(std::string("Library error: ") + err.what());
    }
}
