#ifndef DATAFLOW_INTERPRETER
#define DATAFLOW_INTERPRETER
#include "DataflowModel.h"
#include "Datastore.h"

class InterpreterError : public std::runtime_error
{
public:
	InterpreterError(const std::string & what) : std::runtime_error(what) {}
};

class Interpreter : private BlockVisitor {
public:
	Interpreter(ProjectModel & model);
	void interpret();
private:
	Datastore datastore;
	ProjectModel & model;

	virtual void visit(Constructor & constructor);
	virtual void visit(Operation & constructor);
	
	void ensureAllInputsAreComputed(Operation &operation);
	void allocateOutputs(Operation & operation);
	void executeOperation(Operation &operation);
};

#endif