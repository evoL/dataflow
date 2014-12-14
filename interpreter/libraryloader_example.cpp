#include "LibraryLoader.h"
#include <iostream>

int main()
{
    try {
        Library basicmath{LibraryLoader::load("basicmath")};

        std::cout << "Module loaded: " << basicmath.getName() << std::endl;
        std::cout << "Types [size]:" << std::endl;

        for (auto it = basicmath.getTypes().begin(); it < basicmath.getTypes().end(); it++) {
            std::cout << " * " << *it << " [" << basicmath.getSizes().at(*it) << "]" << std::endl;
        }

        std::cout << "Operations:" << std::endl;

        for (auto it = basicmath.getOperations().begin(); it < basicmath.getOperations().end(); it++) {
            const std::vector<std::string> & inputs = basicmath.getInputs().at(*it);
            const std::vector<std::string> & outputs = basicmath.getOutputs().at(*it);

            std::cout << " * " << *it << " [ ";

            for (auto it2 = inputs.begin(); it2 < inputs.end(); it2++)
                std::cout << *it2 << " ";

            std::cout << "-> ";

            for (auto it2 = outputs.begin(); it2 < outputs.end(); it2++)
                std::cout << *it2 << " ";

            std::cout << "]" << std::endl;
        }

        unsigned int integerSize = basicmath.getSizes().at("Integer");
        void * a = malloc(integerSize);
        void * b = malloc(integerSize);
        void * c = malloc(integerSize);
        basicmath.constructType("Integer", "42", a);
        basicmath.constructType("Integer", "123", b);

        std::vector<void *> inputs;
        std::vector<void *> outputs;

        inputs.push_back(a);
        inputs.push_back(b);
        outputs.push_back(c);

        basicmath.execute("AddIntegers", inputs, outputs);

        std::cout << "Adding integers: " << *(int *)a << " + " << *(int *)b << " = " << *(int *)c << std::endl;

        free(a);
        free(b);
        free(c);
        inputs.clear();
        outputs.clear();

        // ***************

        unsigned int realSize = basicmath.getSizes().at("Real");
        void * d = malloc(realSize);
        void * e = malloc(realSize);
        void * f = malloc(realSize);
        basicmath.constructType("Real", "3.1415", d);
        basicmath.constructType("Real", "2.7183", e);

        inputs.push_back(d);
        inputs.push_back(e);
        outputs.push_back(f);

        basicmath.execute("AddReals", inputs, outputs);

        std::cout << "Adding reals: " << *(double *)d << " + " << *(double *)e << " = " << *(double *)f << std::endl;

        free(d);
        free(e);
        free(f);
        inputs.clear();
        outputs.clear();

    } catch (LibraryLoadError & error) {
        std::cerr << error.what() << std::endl;
    }

    std::cin.get();

    return 0;
}
