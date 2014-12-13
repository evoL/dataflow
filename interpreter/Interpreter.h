#ifndef DATAFLOW_INTERPRETER
#define DATAFLOW_INTERPRETER
#include "DataflowModel.h"
#include "Datastore.h"

class InterpreterError : public std::runtime_error
{
public:
    InterpreterError(const std::string & what) : std::runtime_error(what) {}
};

class Interpreter : private BlockVisitor
{
public:
    Interpreter(ProjectModel & model);
    void interpret();
private:
    Datastore datastore;
    ProjectModel & model;

    virtual void visit(const Constructor & constructor);
    virtual void visit(const Operation & constructor);

    void ensureAllInputsAreComputed(const Operation & operation);
    void allocateOutputs(const Operation & operation);
    void executeOperation(const Operation & operation);
};

#endif