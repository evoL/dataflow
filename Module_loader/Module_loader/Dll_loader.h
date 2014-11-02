#include "Module_loader.h"

typedef const char * (__cdecl * fun_module_type)();
typedef const char * (__cdecl * fun_types_type)();
typedef const char * (__cdecl * fun_operations_type)();

class Dll_loader : public Module_loader
{
private:
	bool						loaded;
	std::string					name;
	std::vector<std::string>	types;
	std::vector<std::string>	operations;
	std::string					error;

public:
	Dll_loader();

	bool							load(const std::string module_name);
	void							clear();
	const std::string&				get_last_error() const;

	const std::string&				get_name() const;
	const std::vector<std::string>&	get_types() const;
	const std::vector<std::string>&	get_operations() const;
};