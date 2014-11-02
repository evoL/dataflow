#include <string>
#include <vector>

class Module_loader
{
public:
	virtual bool							load(const std::string module_name) = 0;
	virtual void							clear() = 0;
	virtual const std::string&				get_last_error() const = 0;

	virtual const std::string&				get_name() const = 0;
	virtual const std::vector<std::string>&	get_types() const = 0;
	virtual const std::vector<std::string>&	get_operations() const = 0;

	virtual unsigned int					size_of(const std::string& type_name) const = 0;
	virtual bool							construct_type(const std::string& type_name, const std::string& data, void * out) const = 0;

	virtual const std::vector<std::string>& inputs(const std::string& operation_name) const = 0;
	virtual const std::vector<std::string>& outputs(const std::string& operation_name) const = 0;

	virtual bool							execute(const std::string& operation_name, const std::vector<void *>& inputs, const std::vector<void *>& outputs) const = 0;
};