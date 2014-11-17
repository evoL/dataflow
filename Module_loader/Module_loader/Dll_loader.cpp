#include "Dll_loader.h"
#include <cstring>

Dll_loader::Dll_loader()
:loaded(false)
,dll_id(0)
{

}

bool Dll_loader::load(const std::string module_name)
{
	dll_id = LoadLibrary((module_name + ".dll").c_str());

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
	
	while (*types_data)
	{
		if (!get_type(types_data))
			return false;

		types_data += strlen(types_data) + 1;
	}


	// Operations

	fun_operations_type operations_fun = (fun_operations_type)GetProcAddress(dll_id, "operations");
	if (!operations_fun)
	{
		error = "Function 'operations' not found";
		return false;
	}
	const char * operations_data = (*operations_fun)();

	while (*operations_data)
	{
		if (!get_operation(operations_data))
			return false;

		operations_data += strlen(operations_data) + 1;
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

const std::unordered_map<std::string, unsigned int>& Dll_loader::get_sizes()
{
	return type_sizes;
}

const std::unordered_map<std::string, std::vector<std::string> >& Dll_loader::get_inputs()
{
	return inputs;
}

const std::unordered_map<std::string, std::vector<std::string> >& Dll_loader::get_outputs()
{
	return outputs;
}

bool Dll_loader::construct_type(const std::string& type_name, const std::string& data, void * out)
{
	auto res = constructors.find(type_name);
	if (res == constructors.end())
	{
		error = "Unknown type";
		return false;
	}

	bool succeeded = (*res->second)(data.c_str(), out);
	if (!succeeded)
	{
		error = "Runtime error: constructor '" + type_name + "'";
		return false;
	}

	return true;
}

bool Dll_loader::execute(const std::string& operation_name, const std::vector<void *>& in, const std::vector<void *>& out)
{
	auto res = executes.find(operation_name);
	if (res == executes.end())
	{
		error = "Unknown operation";
		return false;
	}

	bool succeeded = (*res->second)(in.data(), out.data());
	if (!succeeded)
	{
		error = "Runtime error: operation '" + operation_name + "'";
		return false;
	}

	return true;
}

bool Dll_loader::get_type(const char* type_name)
{	
	std::string type_name_str = type_name;
	if (type_sizes.find(type_name_str) != type_sizes.end())
		return true;

	// type size

	std::string type_size_str = type_name_str + "_size";
	fun_size_type size_fun = (fun_size_type)GetProcAddress(dll_id, type_size_str.c_str());
	if (!size_fun)
	{
		error = "Function '" + type_size_str + "' not found";
		return false;
	}
	unsigned int size = (*size_fun)();

	// construct function

	std::string type_construct_str = type_name_str + "_construct";
	fun_constructor_type construct_fun = (fun_constructor_type)GetProcAddress(dll_id, type_construct_str.c_str());
	if (!construct_fun)
	{
		error = "Function '" + type_construct_str + "' not found";
		return false;
	}

	types.push_back(type_name);
	type_sizes[type_name_str] = size;
	constructors[type_name_str] = construct_fun;

	return true;
}

bool Dll_loader::get_operation(const char* operation_name)
{
	std::string operation_name_str = operation_name;
	if (inputs.find(operation_name_str) != inputs.end())
		return true;

	// input arguments

	std::string operation_inputs_str = operation_name_str + "_inputs";
	fun_types_type inputs_fun = (fun_types_type)GetProcAddress(dll_id, operation_inputs_str.c_str());
	if (!inputs_fun)
	{
		error = "Function '" + operation_inputs_str + "' not found";
		return false;
	}
	const char * inputs_data = (*inputs_fun)();

	std::vector<std::string> inputs_temp;
	while (*inputs_data)
	{
		inputs_temp.push_back(inputs_data);
		inputs_data += strlen(inputs_data) + 1;
	}


	// output arguments

	std::string operation_outputs_str = operation_name_str + "_outputs";
	fun_types_type outputs_fun = (fun_types_type)GetProcAddress(dll_id, operation_outputs_str.c_str());
	if (!outputs_fun)
	{
		error = "Function '" + operation_outputs_str + "' not found";
		return false;
	}
	const char * outputs_data = (*outputs_fun)();

	std::vector<std::string> outputs_temp;
	while (*outputs_data)
	{
		outputs_temp.push_back(outputs_data);
		outputs_data += strlen(outputs_data) + 1;
	}

	
	// execute function
	
	std::string execute_str = operation_name_str + "_execute";
	fun_execute_type execute_fun = (fun_execute_type)GetProcAddress(dll_id, execute_str.c_str());
	if (!execute_fun)
	{
		error = "Function '" + execute_str + "' not found";
		return false;
	}

	operations.push_back(operation_name);
	inputs[operation_name_str] = inputs_temp;
	outputs[operation_name_str] = outputs_temp;
	executes[operation_name_str] = execute_fun;

	return true;
}