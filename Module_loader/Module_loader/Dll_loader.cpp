#include "Dll_loader.h"
#include <Windows.h>
#include <cstring>

Dll_loader::Dll_loader()
:loaded(false)
{

}

bool Dll_loader::load(const std::string module_name)
{
	HINSTANCE dll_id = LoadLibrary((module_name + ".dll").c_str());

	if (!dll_id)
	{
		error = "Library not found";
		return false;
	}

	// Module name

	fun_module_type module_fun = (fun_module_type)GetProcAddress(dll_id, "module");
	if (!module_fun)
	{
		error = "Function 'module' not found";
		return false;
	}
	name = (*module_fun)();

	
	// Types
	
	fun_types_type types_fun = (fun_types_type)GetProcAddress(dll_id, "types");
	if (!types_fun)
	{
		error = "Function 'types' not found";
		return false;
	}
	const char * types_data = (*types_fun)();
	
	while (!*types_data)
	{
		// ...

		types_data += strlen(types_data) + 1;
	}



	return true;
}

void Dll_loader::clear()
{
	loaded = false;
	name.clear();
	types.clear();
	operations.clear();
	error.clear();
}

const std::string& Dll_loader::get_last_error() const
{
	return error;
}

const std::string& Dll_loader::get_name() const
{
	return name;
}

const std::vector<std::string>& Dll_loader::get_types() const
{
	return types;
}

const std::vector<std::string>& Dll_loader::get_operations() const
{
	return operations;
}