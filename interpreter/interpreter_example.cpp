#include <iostream>
#include "DataflowModel.h"
#include "XMLParser.h"
#include "Interpreter.h"
#include "ModelManipulator.h"

int main()
{
    XMLParser parser;
    std::unique_ptr<ProjectModel> model{ parser.loadModelFromFile("C:\\Users\\Prymus\\Documents\\Uni\\Projekt zespolowy\\dataflow\\ppm_sample.xml") };

    ModelManipulator manipulator(*model);

    Interpreter interpreter(*model);
    interpreter.interpret();
}



