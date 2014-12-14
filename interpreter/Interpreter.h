#ifndef DATAFLOW_INTERPRETER
#define DATAFLOW_INTERPRETER
#include "DataflowModel.h"
#include "Datastore.h"
#include "interpreter_export.h"

class INTERPRETER_EXPORT InterpreterError : public std::runtime_error
{
public:
    InterpreterError(const std::string & what) : std::runtime_error(what) {}
};

class INTERPRETER_EXPORT Interpreter : private BlockVisitor
{
public:
    Interpreter(ProjectModel & model);
    void interpret();
	Interpreter& operator=(const Interpreter&) = delete;

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