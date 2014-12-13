#ifndef DATAFLOW_BLOCK_VISITOR
#define DATAFLOW_BLOCK_VISITOR

struct Constructor;
struct Operation;

class BlockVisitor
{
public:
    virtual void visit(const Constructor & constructor) = 0;
    virtual void visit(const Operation & constructor) = 0;
};

#endif