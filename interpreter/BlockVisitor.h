#ifndef DATAFLOW_BLOCK_VISITOR
#define DATAFLOW_BLOCK_VISITOR

#include "interpreter_export.h"

struct Constructor;
struct Operation;

class INTERPRETER_NO_EXPORT BlockVisitor
{
public:
    virtual void visit(const Constructor & constructor) = 0;
    virtual void visit(const Operation & constructor) = 0;
};

#endif