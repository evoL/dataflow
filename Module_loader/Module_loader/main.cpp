#include "Dll_loader.h"
#include <iostream>
#include <cstdlib>

int main()
{
	Dll_loader loader;
	if (!loader.load("Basic_math"))
	{
		std::cout << loader.get_last_error() << std::endl;
	}
	else
	{
		std::cout << "Module loaded: " << loader.get_name() << std::endl;
		std::cout << "Types [size]:" << std::endl;
		for (auto& it = loader.get_types().begin(); it < loader.get_types().end(); it++)
		{
			std::cout << " * " << *it << " [" << loader.get_sizes().at(*it) << "]" << std::endl;
		}
		
		std::cout << "Operations:" << std::endl;
		for (auto& it = loader.get_operations().begin(); it < loader.get_operations().end(); it++)
		{
			const std::vector<std::string>& inputs = loader.get_inputs().at(*it);
			const std::vector<std::string>& outputs = loader.get_outputs().at(*it);
			
			std::cout << " * " << *it << " [ ";
			for (auto& it2 = inputs.begin(); it2 < inputs.end(); it2++)
				std::cout << *it2 << " ";

			std::cout << "-> ";
			for (auto& it2 = outputs.begin(); it2 < outputs.end(); it2++)
				std::cout << *it2 << " ";

			std::cout << "]" << std::endl;
		}

		unsigned int integer_size = loader.get_sizes().at("Integer");
		void * a = malloc(integer_size);
		void * b = malloc(integer_size);
		void * c = malloc(integer_size);
		loader.construct_type("Integer", "42", a);
		loader.construct_type("Integer", "123", b);

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

		unsigned int real_size = loader.get_sizes().at("Real");
		void * d = malloc(real_size);
		void * e = malloc(real_size);
		void * f = malloc(real_size);
		loader.construct_type("Real", "3.1415", d);
		loader.construct_type("Real", "2.7183", e);

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

	system("PAUSE");
	
	return 0;
}