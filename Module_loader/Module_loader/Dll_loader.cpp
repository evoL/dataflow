#include "Dll_loader.h"
#include <cstring>

Dll_loader::Dll_loader()
:loaded(false)
,dll_id(0)
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
		if (!get_type(types_data))
			return false;

		types_data += strlen(types_data) + 1;
	}



	return true;
}

void Dll_loader::clear()
{
	loaded = false;
	dll_id = 0;
	name.clear();
	types.clear();
	operations.clear();
	error.clear();
	type_sizes.clear();
	constructors.clear();
	inputs.clear();
	outputs.clear();
	executes.clear();
}

const std::string& Dll_loader::get_last_error()
{
	return error;
}

const std::string& Dll_loader::get_name()
{
	return name;
}

const std::vector<std::string>& Dll_loader::get_types()
{
	return types;
}

const std::vector<std::string>& Dll_loader::get_operations()
{
	return operations;
}

unsigned int Dll_loader::size_of(const std::string& type_name)
{
	auto res = type_sizes.find(type_name);
	if (res == type_sizes.end())
		return 0;
	return res->second;
}

bool Dll_loader::construct_type(const std::string& type_name, const std::string& data, void * out)
{
	auto res = constructors.find(type_name);
	if (res == constructors.end())
	{
		error = "Unknown type";
		return false;
	}

	return (*res->second)(data.c_str(), out);
}

const std::vector<std::string>& Dll_loader::get_inputs(const std::string& operation_name)
{

}

const std::vector<std::string>& Dll_loader::get_outputs(const std::string& operation_name)
{

}

bool Dll_loader::execute(const std::string& operation_name, const std::vector<void *>& inputs, const std::vector<void *>& outputs)
{
	
}

bool Dll_loader::get_type(const char* type_name)
{
	// type size
}