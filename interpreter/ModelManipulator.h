#ifndef MODELMANIPULATOR_H
#define MODELMANIPULATOR_H

#include "DataflowModel.h"
#include <stdexcept>

class ModelManipulatorError : public std::runtime_error
{
public:
    ModelManipulatorError(const std::string & what): std::runtime_error(what) {}
};

class ModelManipulator
{
private:
	ProjectModel& model;
	int maxOutputId;
	int maxBlockId;

public:
	ModelManipulator(ProjectModel& model);
	ModelManipulator(const ModelManipulator&) = delete;
	const ModelManipulator& operator=(const ModelManipulator&) = delete;

private:
	bool checkModelCorrectness();
	void reassignIds();
};

#endif