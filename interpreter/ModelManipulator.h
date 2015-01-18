#ifndef MODELMANIPULATOR_H
#define MODELMANIPULATOR_H

#include "DataflowModel.h"
#include <stdexcept>
#include "interpreter_export.h"

class INTERPRETER_EXPORT ModelManipulatorError : public std::runtime_error
{
public:
    ModelManipulatorError(const std::string & what): std::runtime_error(what) {}
};

class INTERPRETER_EXPORT ModelManipulator
{
private:
    ProjectModel& model;
    int maxOutputId;
    int maxBlockId;

public:
    ModelManipulator(ProjectModel& model);
    ModelManipulator(const ModelManipulator&) = delete;
    const ModelManipulator& operator=(const ModelManipulator&) = delete;

    void setProjectName(const std::string & name);

    void addLibrary(const std::string & name);
    void deleteLibrary(const std::string & name);

    void setEntryPoint(int blockId);
    void unsetEntryPoint(int blockId);

    int addOperation(const std::string & module, const std::string & name, Position position = Position {0.0, 0.0});
    int addConstructor(const std::string & module, const std::string & type, Position position = Position {0.0, 0.0});
    void deleteBlock(int blockId);

    void setConstructorData(int blockId, const std::string & data);
    void setBlockPosition(int blockId, Position position);

    void addConnection(int outputBlockId, int outputIndex, int inputBlockId, int inputIndex);
    void deleteConnection(int blockId, int inputIndex);


private:
    void ensureModelCorrectness();
    void reassignIds();
};

#endif
