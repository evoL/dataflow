#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <unordered_map>
#include <vector>
#include "library_loading.h"

#ifdef _MSC_VER
#   define DATAFLOW_CALLING_CONVENTION __cdecl
#else
#   define DATAFLOW_CALLING_CONVENTION
#endif

typedef const char * (DATAFLOW_CALLING_CONVENTION * module_list_fn)();
typedef const char * (DATAFLOW_CALLING_CONVENTION * type_list_fn)();
typedef const char * (DATAFLOW_CALLING_CONVENTION * operation_list_fn)();
typedef bool (DATAFLOW_CALLING_CONVENTION * constructor_fn)(const char *, void *);
typedef bool (DATAFLOW_CALLING_CONVENTION * destructor_fn)(void *);
typedef bool (DATAFLOW_CALLING_CONVENTION * execute_fn)(void * const *, void * const *);
typedef unsigned int (DATAFLOW_CALLING_CONVENTION * type_size_fn)();

class LibraryError : public std::runtime_error
{
public:
    LibraryError(const std::string & what): std::runtime_error(what) {}
};

class Library
{
    friend class LibraryLoader;

	DATAFLOW_LIBRARY dllId;
	bool ownsDll{ false };
    std::string name;
    std::vector<std::string> types;
    std::vector<std::string> operations;
    std::unordered_map<std::string, unsigned int> typeSizes;
    std::unordered_map<std::string, constructor_fn> constructors;
    std::unordered_map<std::string, destructor_fn> destructors;
    std::unordered_map<std::string, std::vector<std::string> > inputs;
    std::unordered_map<std::string, std::vector<std::string> > outputs;
    std::unordered_map<std::string, execute_fn> executes;

	void freeDll();
	void moveFields(Library && other);
public:
	Library() {}
	Library(const Library &) = delete;
	Library & operator=(const Library &) = delete;

	Library(Library && other);
	Library & operator=(Library && other);

    ~Library();

    const std::string & getName() const;
	const std::vector<std::string> & getTypes() const;
	const std::vector<std::string> & getOperations() const;
	const std::unordered_map<std::string, unsigned int> & getSizes() const;
	const std::unordered_map<std::string, std::vector<std::string> > & getInputs() const;
	const std::unordered_map<std::string, std::vector<std::string> > & getOutputs() const;

	void constructType(const std::string & typeName, const std::string & data, void * out) const;
	void destructType(const std::string & typeName, void * data) const;
	void execute(const std::string & operationName, const std::vector<void *> & inputs, const std::vector<void *> & outputs) const;
};

#endif
