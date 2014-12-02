#ifndef DATAFLOW_MODEL
#define DATAFLOW_MODEL

#include <list>
#include <unordered_map>
#include <memory>
#include <string>

#include "XMLParser.h"

struct Block;

typedef std::unordered_map<int, std::shared_ptr<Block>> BlocksMap;

typedef enum {
    BlockTypeOperation,
    BlockTypeConstructor
} BlockType;

struct Position {
    float x;
    float y;
};

struct InputTransition {
    int index;
    int outputId;
    std::shared_ptr<Block> outputBlock;
};

struct OutputTransition {
    int id;
};

struct Block {
    // constructor
    Block(int id, std::string module = "", Position position = Position {0.0, 0.0}, const std::list<OutputTransition> & outputs = std::list<OutputTransition>())
        : id(id), module(module), position(position), outputs(outputs) {}

    // methods
    virtual BlockType blockType() const = 0;
    
    // fields
    int id;
    std::string module;
    Position position;
    std::list<OutputTransition> outputs;
};

struct Constructor : Block {
    // constructor
    Constructor(int id, std::string module = "", std::string type = "", Position position = Position {0.0, 0.0}, const std::list<OutputTransition> & outputs = std::list<OutputTransition>(), const char * data = "")
        : Block(id, module, position, outputs), type(type), data(data) {}

    // methods
    virtual BlockType blockType() const { return BlockTypeConstructor; }
    
    // fields
    std::string type;
    const char * data;
};

struct Operation : Block {
    // constructor
    Operation(int id, std::string module = "",  std::string name = "", Position position = Position {0.0, 0.0}, const std::list<OutputTransition> & outputs = std::list<OutputTransition>(), const std::list<InputTransition> inputs = std::list<InputTransition>())
        : Block(id, module, position, outputs), inputs(inputs), name(name) {}

    // methods
    virtual BlockType blockType() const { return BlockTypeOperation; }
    
    // fields
    std::list<InputTransition> inputs;
    std::string name;
};

class ProjectModel
{
    friend class XMLParser;
    
public:
    // getters
    const std::string & getName() { return name; }
    const std::list<std::string> & getImports() { return imports; }
    const std::list<int> & getEntryPoints() { return entryPoints; }
    const Block & getBlock(int id) { return *blocks[id]; }

//private:
public:
    std::string name;
    std::list<std::string> imports;
    std::list<int> entryPoints;
    BlocksMap blocks;
};

#endif