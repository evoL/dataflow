#include <iostream>
#include "DataflowModel.h"
#include "XMLParser.h"
#include "Interpreter.h"
#include "ModelManipulator.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " file.xml" << std::endl;
        return 1;
    }

    try {
        XMLParser parser;
        std::unique_ptr<ProjectModel> model{ parser.loadModelFromFile(argv[1]) };

        ModelManipulator manipulator(*model);

        Interpreter interpreter(*model);
        interpreter.interpret();
    } catch (XMLParserError e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}



