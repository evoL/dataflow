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
	}

	system("PAUSE");
	
	return 0;
}