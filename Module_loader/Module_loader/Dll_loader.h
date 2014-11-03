#include "Module_loader.h"
#include <Windows.h>

typedef const char * (__cdecl * fun_module_type)();
typedef const char * (__cdecl * fun_types_type)();
typedef const char * (__cdecl * fun_operations_type)();
typedef bool (__cdecl * fun_constructor_type)(const char *, void *);
typedef bool (__cdecl * fun_execute_type)(void * const *, void * const *);
typedef unsigned int (__cdecl * fun_size_type)();

class Dll_loader : public Module_loader
{
private:
	bool														loaded;
	HINSTANCE													dll_id;
	std::string													name;
	std::vector<std::string>									types;
	std::vector<std::string>									operations;
	std::string													error;
	std::unordered_map<std::string, unsigned int>				type_sizes;
	std::unordered_map<std::string, fun_constructor_type>		constructors;
	std::unordered_map<std::string, std::vector<std::string> >	inputs;
	std::unordered_map<std::string, std::vector<std::string> >	outputs;
	std::unordered_map<std::string, fun_execute_type>			executes;

public:
	Dll_loader();

	bool																load(const std::string module_name);
	void																clear();
	const std::string&													get_last_error();

	const std::string&													get_name();
	const std::vector<std::string>&										get_types();
	const std::vector<std::string>&										get_operations();
	const std::unordered_map<std::string, unsigned int>&				get_sizes();
	const std::unordered_map<std::string, std::vector<std::string> >&	get_inputs();
	const std::unordered_map<std::string, std::vector<std::string> >&	get_outputs();

	bool																construct_type(const std::string& type_name, const std::string& data, void * out);
	bool																execute(const std::string& operation_name, const std::vector<void *>& inputs, const std::vector<void *>& outputs);

private:
	bool																get_type(const char * type_name);
	bool																get_operation(const char * operation_name);
};