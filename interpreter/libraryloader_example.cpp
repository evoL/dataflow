#include "LibraryLoader.h"
#include <iostream>
#include <cstdlib>

int main()
{
    LibraryLoader loader;

    if (!loader.load("basicmath")) {
        std::cout << loader.getLastError() << std::endl;
    } else {
        std::cout << "Module loaded: " << loader.getName() << std::endl;
        std::cout << "Types [size]:" << std::endl;

        for (auto it = loader.getTypes().begin(); it < loader.getTypes().end(); it++) {
            std::cout << " * " << *it << " [" << loader.getSizes().at(*it) << "]" << std::endl;
        }

        std::cout << "Operations:" << std::endl;

        for (auto it = loader.getOperations().begin(); it < loader.getOperations().end(); it++) {
            const std::vector<std::string> & inputs = loader.getInputs().at(*it);
            const std::vector<std::string> & outputs = loader.getOutputs().at(*it);

            std::cout << " * " << *it << " [ ";

            for (auto it2 = inputs.begin(); it2 < inputs.end(); it2++)
                std::cout << *it2 << " ";

            std::cout << "-> ";

            for (auto it2 = outputs.begin(); it2 < outputs.end(); it2++)
                std::cout << *it2 << " ";

            std::cout << "]" << std::endl;
        }

        unsigned int integer_size = loader.getSizes().at("Integer");
        void * a = malloc(integer_size);
        void * b = malloc(integer_size);
        void * c = malloc(integer_size);
        loader.constructType("Integer", "42", a);
        loader.constructType("Integer", "123", b);

        std::vector<void *> inputs;
        std::vector<void *> outputs;

        inputs.push_back(a);
        inputs.push_back(b);
        outputs.push_back(c);

        loader.execute("AddIntegers", inputs, outputs);

        std::cout << "Adding integers: " << *(int *)a << " + " << *(int *)b << " = " << *(int *)c << std::endl;

        free(a);
        free(b);
        free(c);
        inputs.clear();
        outputs.clear();

        // ***************

        unsigned int real_size = loader.getSizes().at("Real");
        void * d = malloc(real_size);
        void * e = malloc(real_size);
        void * f = malloc(real_size);
        loader.constructType("Real", "3.1415", d);
        loader.constructType("Real", "2.7183", e);

        inputs.push_back(d);
        inputs.push_back(e);
        outputs.push_back(f);

        loader.execute("AddReals", inputs, outputs);

        std::cout << "Adding reals: " << *(double *)d << " + " << *(double *)e << " = " << *(double *)f << std::endl;

        free(d);
        free(e);
        free(f);
        inputs.clear();
        outputs.clear();

    }

    std::cin.get();

    return 0;
}
