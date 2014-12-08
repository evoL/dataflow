#ifndef DATAFLOW_INTERPRETER
#define DATAFLOW_INTERPRETER
#include "DataflowModel.h"
#include "Datastore.h"

class InterpreterError : public std::runtime_error
{
public:
	InterpreterError(const std::string & what) : std::runtime_error(what) {}
};

class Interpreter {
public:
	Interpreter(ProjectModel & model);
	void interpret();
private:
	Datastore datastore;
	ProjectModel & model;
};

#endif