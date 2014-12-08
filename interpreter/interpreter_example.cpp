#include <iostream>
#include "DataflowModel.h"
#include "XMLParser.h"
#include "Interpreter.h"

int main() {
	XMLParser parser;
	std::unique_ptr<ProjectModel> model{ parser.loadModelFromFile("sample.xml") };

	Interpreter interpreter(*model);
	interpreter.interpret();
}



