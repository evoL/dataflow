#include "ModelManipulator.h"
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
		case BlockTypeOperation:
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
			}

			auto inputs = lib.getInputs().find(op.name);
			if (inputs == lib.getInputs().end())
				return false;

			// check if there are specified redundant inputs
			if (inputs->second.size() < op.inputs.size())
				return false;

			for (auto& it2 : op.inputs) {
				if (it2.first >= inputs->second.size())
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
				const std::string& oName = (outputBlock->blockType() == BlockTypeOperation)
										 ? (dynamic_cast<Operation &>(*outputBlock).name)
										 : (dynamic_cast<Constructor &>(*outputBlock).type);
				auto found_output_list = found_output_lib->second.getOutputs().find(oName);
				if (found_output_list == found_output_lib->second.getOutputs().end())
					return false;

				// and if it is the correct one
				bool output_found = false;
				for (int i=0; i<block.outputs.size(); i++) {
					if (block.outputs[i].id == it2.first) {
						// ... and if the types match
						if (found_output_list->second[i] != input_type)
							return false;
						
						output_found = true;
						break;
					}
				}
				if (!output_found)
					return false;
			}

			break;
		}

		case BlockTypeConstructor:
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
			if (cons.outputs.size() == 1)
				return false;

			// check if output ids are unique in the context of whole model
			if (outputIds.find(cons.outputs[0].id) != outputIds.end())
					return false;
			outputIds.insert(cons.outputs[0].id);
					 
			break;
		}

		default:
			return false;

		}
	}

	// entry points
	
	return true;
}

void ModelManipulator::reassignIds()
{

}