#ifndef DATAFLOW_MODEL
#define DATAFLOW_MODEL

#include <list>
#include <unordered_map>
#include <memory>

struct Block;

typedef std::unordered_map<int, std::unique_ptr<Block>> BlocksMap;

struct Position
{
    float x;
    float y;
};

struct InputTransition
{
    int id;
    int output_id;
    Block *outputBlock;
};

struct OutputTransition
{
    int id;
};

struct Block
{
    // constructor
    Block(int id, std::string module = "", Position position = Position {0.0, 0.0}, const std::list<OutputTransition>& outputs = std::list<OutputTransition>())
    : id(id), module(module), position(position), outputs(outputs) {}
    
    // fields
    int id;
    std::string module;
    Position position;
    std::list<OutputTransition> outputs;
};

struct Constructor : Block
{
    // constructor
    Constructor(int id, std::string module = "", Position position = Position {0.0, 0.0}, const std::list<OutputTransition>& outputs = std::list<OutputTransition>(), std::string type = "", const char* data = "")
    : Block(id, module, position, outputs), type(type), data(data) {}
    
    // fields
    std::string type;
    const char* data;
};

struct Operation : Block
{
    // constructor
    Operation(int id, std::string module = "", Position position = Position {0.0, 0.0}, const std::list<OutputTransition>& outputs = std::list<OutputTransition>(), const std::list<InputTransition> inputs = std::list<InputTransition>(), std::string name = "")
    : Block(id, module, position, outputs), inputs(inputs), name(name) {}
    
    // fields
    std::list<InputTransition> inputs;
    std::string name;
};

class DataflowProjectModel
{
public:
    // getters
    const std::string& getName() { return name; }
    const std::list<std::string>& getImports() { return imports; }
    const Block& getBlock(int id) { return *blocks[id]; }
    
private:
    std::string name;
    std::list<std::string> imports;
    BlocksMap blocks;
};

#endif