#ifndef DATAFLOW_BLOCK_VISITOR
#define DATAFLOW_BLOCK_VISITOR

struct Constructor;
struct Operation;

class BlockVisitor {
public:
	virtual void visit(Constructor & constructor) = 0;
	virtual void visit(Operation & constructor) = 0;
};

#endif