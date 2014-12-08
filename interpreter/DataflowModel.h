#ifndef DATAFLOW_MODEL
#define DATAFLOW_MODEL

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#include "Library.h"
#include "BlockVisitor.h"

struct Block;

typedef std::unordered_map<int, std::shared_ptr<Block>> BlocksMap;
typedef std::unordered_map<std::string, Library> LibraryMap;

enum class BlockType
{
    Operation,
    Constructor
};

struct Position
{
    float x;
    float y;
};

struct InputTransition
{
    int outputId;
    std::shared_ptr<Block> outputBlock;
};

struct OutputTransition
{
    int id;
};

typedef std::unordered_map<int, InputTransition> InputTransitionMap;

struct Block
{
    // constructor
    Block(int id, std::string module = "", Position position = Position {0.0, 0.0}, const std::vector<OutputTransition> & outputs = std::vector<OutputTransition>())
        : id(id), module(module), position(position), outputs(outputs) {}

    // methods
    virtual BlockType blockType() const = 0;
	virtual void accept(BlockVisitor & visitor) = 0;

    // fields
    int id;
    std::string module;
    Position position;
    std::vector<OutputTransition> outputs;
};

struct Constructor : Block
{
    // constructor
    Constructor(int id, std::string module = "", std::string type = "", Position position = Position {0.0, 0.0}, const std::vector<OutputTransition> & outputs = std::vector<OutputTransition>(), const char * data = "")
    :Block(id, module, position, outputs)
	,type(type)
	,data(data) {}

    // methods
	virtual BlockType blockType() const { return BlockType::Constructor; }
	void accept(BlockVisitor & visitor) { visitor.visit(*this); };

    // fields
    std::string type;
    std::string data;
};

struct Operation : Block
{
    // constructor
    Operation(int id, std::string module = "",  std::string name = "", Position position = Position {0.0, 0.0}, const std::vector<OutputTransition> & outputs = std::vector<OutputTransition>(), const InputTransitionMap inputs = InputTransitionMap())
    :Block(id, module, position, outputs)
	,inputs(inputs)
	,name(name) {}

    // methods
    virtual BlockType blockType() const { return BlockType::Operation; }
	void accept(BlockVisitor & visitor) { visitor.visit(*this); };

    // fields
    InputTransitionMap inputs;
    std::string name;
};

class ProjectModel
{
    friend class XMLParser;
	friend class ModelManipulator;

public:
    // getters
    const std::string & getName() { return name; }
    const LibraryMap & getLibraries() { return libraries; }
    const Block & getBlock(int id) { return *blocks[id]; }

private:
    std::string name;
    LibraryMap libraries;
    std::vector<int> entryPoints;
    BlocksMap blocks;
};

#endif