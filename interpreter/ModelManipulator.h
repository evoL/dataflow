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

    void setProjectName(std::string& name);
    
    void addLibrary(std::string& name);
    void deleteLibrary(std::string& name);
    
    void setEntryPoint(int blockId);
    void unsetEntryPoint(int blockId);

    void addOperation(std::string& module, std::string& name, Position position = Position {0.0, 0.0});
    void addConstructor(std::string& module, std::string& type, Position position = Position {0.0, 0.0});
    void deleteBlock(int blockId);

    void setConstructorData(int blockId, std::string& data);
    void setBlockPosition(int blockId, Position position);

    void addConnection(int outputBlockId, int outputIndex, int inputBlockId, int inputIndex);
    void deleteConnection(int blockId, int inputIndex);


private:
    bool checkModelCorrectness();
    void reassignIds();
};

#endif