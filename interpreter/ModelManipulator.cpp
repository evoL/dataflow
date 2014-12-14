#include "ModelManipulator.h"
#include "LibraryLoader.h"
#include <set>

ModelManipulator::ModelManipulator(ProjectModel& model)
:model(model)
,maxOutputId(0)
,maxBlockId(0)
{
    if (!checkModelCorrectness())
        throw ModelManipulatorError("Invalid model");

    reassignIds();
}


void ModelManipulator::setProjectName(std::string& name)
{
    model.name = name;
}
    
void ModelManipulator::addLibrary(std::string& name)
{
    if (model.libraries.find(name) != model.libraries.end())
        return;

    try {
        model.libraries[name] = LibraryLoader::load(name);
    }
    catch (LibraryLoadError& error)
    {
        throw ModelManipulatorError(std::string("Error while importing library: ").append(error.what()));
    }
}
    
void ModelManipulator::deleteLibrary(std::string& name)
{
    for (auto& it : model.blocks) {
        if (it.second->module == name)
            deleteBlock(it.first);
    }
    model.libraries.erase(name);
}
    
void ModelManipulator::setEntryPoint(int blockId)
{
    if (model.blocks.find(blockId) == model.blocks.end())
        return;

    for (auto& it : model.entryPoints) {
        if (it == blockId)
            return;
    }
    model.entryPoints.push_back(blockId);
}

void ModelManipulator::unsetEntryPoint(int blockId)
{
    for (std::vector<int>::iterator it=model.entryPoints.begin(); it<model.entryPoints.end(); ++it) {
        if (*it == blockId) {
            model.entryPoints.erase(it);
            return;
        }
    }
}

void ModelManipulator::addOperation(std::string& module, std::string& name, Position position)
{
    auto libFound = model.libraries.find(module);
    if (libFound == model.libraries.end())
        throw ModelManipulatorError("Module " + module + " not imported");
    
    auto outputsFound = libFound->second.getOutputs().find(name);
    if (outputsFound == libFound->second.getOutputs().end())
        throw ModelManipulatorError("Operation " + name + " not found in module " + module);
    
    maxBlockId++;
    model.blocks[maxBlockId] = std::shared_ptr<Block>(new Operation(maxBlockId, module, name, position));
    auto& outputs = model.blocks[maxBlockId]->outputs;
    for (int i=0; i<(int)outputsFound->second.size(); ++i)
        outputs.push_back(OutputTransition {++maxOutputId});
}

void ModelManipulator::addConstructor(std::string& module, std::string& type, Position position)
{
    auto libFound = model.libraries.find(module);
    if (libFound == model.libraries.end())
        throw ModelManipulatorError("Module " + module + " not imported");
    
    bool typeFound = false;
    for (auto& it : libFound->second.getTypes()) {
        if (it == type) {
            typeFound = true;
            break;
        }
    }
    
    if (!typeFound)
        throw ModelManipulatorError("Type " + type + " not found in module " + module);
    
    maxBlockId++;
    model.blocks[maxBlockId] = std::shared_ptr<Block>(new Constructor(maxBlockId, module, type, position));
    model.blocks[maxBlockId]->outputs.push_back(OutputTransition {++maxOutputId});
}

void ModelManipulator::deleteBlock(int blockId)
{
    if (model.blocks.find(blockId) == model.blocks.end())
        return;

    auto& blockPtr = model.blocks[blockId];
    
    for (auto& it : model.blocks) {
        if (it.second->blockType() != BlockType::Operation)
            continue;

        Operation& op = dynamic_cast<Operation &>(*(it.second));
        auto& inputs = op.inputs;

        std::vector<int> toErase;
        for (auto& it2 : inputs) {
            if (it2.second.outputBlock == blockPtr)
                toErase.push_back(it2.first);
        }

        for (auto it2 : toErase)
            inputs.erase(it2);
    }

    model.blocks.erase(blockId);
}

void ModelManipulator::setConstructorData(int blockId, std::string& data)
{
    auto foundBlock = model.blocks.find(blockId);
    if (foundBlock == model.blocks.end() || foundBlock->second->blockType() != BlockType::Constructor)
        throw ModelManipulatorError("Invalid constructor Id");
    
    Constructor& cons = dynamic_cast<Constructor &>(*(foundBlock->second));
    cons.data = data;
}

void ModelManipulator::setBlockPosition(int blockId, Position position)
{
    auto foundBlock = model.blocks.find(blockId);
    if (foundBlock == model.blocks.end())
        throw ModelManipulatorError("Invalid block Id");

    foundBlock->second->position = position;
}

void ModelManipulator::addConnection(int outputBlockId, int outputIndex, int inputBlockId, int inputIndex)
{
    auto inputBlockFound = model.blocks.find(inputBlockId);
    if (inputBlockFound == model.blocks.end() || inputBlockFound->second->blockType() != BlockType::Operation)
        throw ("Invalid input block Id");

    auto outputBlockFound = model.blocks.find(outputBlockId);
    if (outputBlockFound == model.blocks.end())
        throw ModelManipulatorError("Invalid output block Id");

    Operation& inputBlock = dynamic_cast<Operation&>(*(inputBlockFound->second));
    Block& outputBlock = dynamic_cast<Block&>(*(outputBlockFound->second));

    if (inputBlock.inputs.find(inputIndex) != inputBlock.inputs.end())
        throw ModelManipulatorError("Multiple connections to single input slot not allowed");

    Library& inputLib = model.libraries[inputBlock.module];
    Library& outputLib = model.libraries[outputBlock.module];

    auto inputTypes = inputLib.getInputs().find(inputBlock.name);
    if (inputIndex >= inputTypes->second.size() || inputIndex < 0)
        throw ModelManipulatorError("Invalid input index");
    const std::string& inputType = inputTypes->second[inputIndex];

    if (outputBlock.blockType() == BlockType::Operation) {
        Operation& outputOperation = dynamic_cast<Operation&>(outputBlock);
        auto outputTypes = outputLib.getOutputs().find(outputOperation.name);
        if (outputIndex > outputTypes->second.size() || outputIndex < 0)
            throw ModelManipulatorError("Invalid output index");
        const std::string& outputType = outputTypes->second[outputIndex];
        if (outputType != inputType)
            throw ModelManipulatorError("Connection does not type itself");

        inputBlock.inputs[inputIndex] = InputTransition { outputBlock.outputs[outputIndex].id, outputBlockFound->second };

    } else {
        Constructor& outputConstructor = dynamic_cast<Constructor&>(outputBlock);
        if (outputIndex != 0)
            throw ("Invalid output index");
        std::string outputType = outputConstructor.module + "." + outputConstructor.type;
        if (outputType != inputType)
            throw ModelManipulatorError("Connection does not type itself");

        inputBlock.inputs[inputIndex] = InputTransition { outputConstructor.outputs[0].id, outputBlockFound->second };
    }
}

void ModelManipulator::deleteConnection(int blockId, int inputIndex)
{
    auto blockFound = model.blocks.find(blockId);
    if (blockFound == model.blocks.end() || blockFound->second->blockType() != BlockType::Operation)
        return;

    Operation& op = dynamic_cast<Operation&>(*(blockFound->second));
    op.inputs.erase(inputIndex);
}

bool ModelManipulator::checkModelCorrectness()
{    
    if (model.name.length() == 0)
        return false;

    LibraryMap& libraries = model.libraries;
    std::vector<int>& entryPoints = model.entryPoints;
    BlocksMap& blocks = model.blocks;

    // iterate over blocks

    std::set<int> outputIds;
    
    for (auto& it : blocks) {
        Block& block = dynamic_cast<Block &>(*it.second);

        // check if the desired library is loaded
        auto found_lib = model.libraries.find(block.module);
        if (found_lib == model.libraries.end())
            return false;

        Library& lib = found_lib->second;

        if (block.id > maxBlockId)
            maxBlockId = block.id;

        switch (block.blockType()) {
        case BlockType::Operation:
        {
            // check if library contains desired operation                       
            Operation& op = dynamic_cast<Operation &>(block);
            auto outputs = lib.getOutputs().find(op.name);
            if (outputs == lib.getOutputs().end())
                return false;

            // check if operation has defined adequate number of outputs
            if (outputs->second.size() != op.outputs.size())
                return false;

            // check if output ids are unique in the context of whole model
            for (auto& it2 : op.outputs) {
                if (outputIds.find(it2.id) != outputIds.end())
                    return false;
                outputIds.insert(it2.id);
                if (it2.id > maxOutputId)
                    maxOutputId = it2.id;
            }

            auto inputs = lib.getInputs().find(op.name);
            if (inputs == lib.getInputs().end())
                return false;

            // check if there are specified redundant inputs
            if (inputs->second.size() < op.inputs.size())
                return false;

            for (auto& it2 : op.inputs) {
                if (it2.first >= (int)inputs->second.size())
                    return false;

                // check if the input is connected to a block
                std::shared_ptr<Block> outputBlock = it2.second.outputBlock;
                if (outputBlock.get() == 0)
                    return false;

                // get the type of input
                const std::string& input_type = inputs->second[it2.first];

                // get the type of output
                auto found_output_lib = libraries.find(outputBlock->module);
                if (found_output_lib == libraries.end())
                    return false;
                if (outputBlock->blockType() == BlockType::Operation) {
                    auto found_output_list = found_output_lib->second.getOutputs().find(dynamic_cast<Operation &>(*outputBlock).name);
                    if (found_output_list == found_output_lib->second.getOutputs().end())
                        return false;

                    // and if it is the correct one
                    bool output_found = false;
                    for (int i=0; i<(int)outputBlock->outputs.size(); i++) {
                        if (outputBlock->outputs[i].id == it2.second.outputId) {
                            // ... and if the types match
                            if (found_output_list->second[i] != input_type)
                                return false;
                        
                            output_found = true;
                            break;
                        }
                    }
                    if (!output_found)
                        return false;
                } else {
                    if (outputBlock->outputs.size() != 1)
                        return false;
                    if (outputBlock->outputs[0].id != it2.second.outputId)
                        return false;

                    if ((outputBlock->module + "." + dynamic_cast<Constructor&>(*outputBlock).type) != input_type)
                        return false;
                }

            }

            break;
        }

        case BlockType::Constructor:
        {
            // check if library contains desired constructor
            Constructor& cons = dynamic_cast<Constructor &>(block);
            
            bool type_found = false;
            for (auto& it : lib.getTypes()) {
                if (it == cons.type) {
                    type_found = true;
                    break;
                }
            }

            // check if operation has defined adequate number of outputs
            if (cons.outputs.size() != 1)
                return false;

            // check if output ids are unique in the context of whole model
            if (outputIds.find(cons.outputs[0].id) != outputIds.end())
                    return false;
            outputIds.insert(cons.outputs[0].id);
            if (cons.outputs[0].id > maxOutputId)
                maxOutputId = cons.outputs[0].id;
                     
            break;
        }

        default:
            return false;

        }
    }

    // check entry points
    for (auto& ep : entryPoints) {
        if (blocks.find(ep) == blocks.end())
            return false;
    }
    
    return true;
}

void ModelManipulator::reassignIds()
{
    // to be implemented
}