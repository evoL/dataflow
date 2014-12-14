#include "Interpreter.h"
#include <string>


Interpreter::Interpreter(ProjectModel & model) : model(model)
{

}

void Interpreter::interpret()
{
    datastore.reset();
    auto entry_points = model.getEntryPoints();

    for (auto point : entry_points) {
        model.getBlock(point).accept(*this);
    }
}

void Interpreter::visit(const Constructor & constructor)
{
    const Library & library = model.getLibraries().at(constructor.module);
    auto type_size = library.getSizes().at(constructor.type);

    if (constructor.outputs.size() != 1)
        throw InterpreterError("Invalid constructor output count detected - should be 1.");

    auto output_id = constructor.outputs[0].id;
    auto allocated_space = datastore.createEntry(output_id, type_size);

    try {
		library.constructType(constructor.type, constructor.data, allocated_space);
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
    auto & output_types = library.getOutputs().at(operation.name);

    if (output_types.size() != operation.outputs.size())
        throw InterpreterError("Library outputs information is inconsistent with block output information");

    auto current_type = output_types.begin();
    auto current_output = operation.outputs.begin();

    while (current_type != output_types.end()) {

        // BARDZO tymczasowe - wynikla jakas rozbieznosc i czasem typy maja przedrostek z nazwa modulu, a czasem nie
        std::string current_type_name{ current_type->begin() + current_type->find_last_of('.') + 1, current_type->end() };

        auto current_type_size = library.getSizes().at(current_type_name);
        datastore.createEntry(current_output->id, current_type_size);

        current_type++;
        current_output++;
    }
}

void Interpreter::executeOperation(const Operation & operation)
{
    auto & library = model.getLibraries().at(operation.module);
    std::vector<void *> input_locations;
    std::vector<void *> output_locations;

    for (auto & input : operation.inputs)
        input_locations.push_back(datastore[input.second.outputId]);

    for (auto & output : operation.outputs)
        output_locations.push_back(datastore[output.id]);

    try {
		library.execute(operation.name, input_locations, output_locations);
	} catch (LibraryError& err) {
		throw InterpreterError(std::string("Library error: ") + err.what());
	}
}
