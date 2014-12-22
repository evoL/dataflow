#include "DiagramOperation.h"


DiagramOperation::DiagramOperation(const std::shared_ptr<Block> blockPointer, QMenu * contextMenu, QGraphicsItem * parent)
	: blockPtr(std::static_pointer_cast<Operation>(blockPointer))
{
    myContextMenu = contextMenu;
	//const std::shared_ptr<Operation> blockPtr = std::make_shared<Operation>(*blockPt);

    width = 170;
    height = 100;

    blockName = new BlockDescription(QString::fromStdString(blockPtr->name), this);
    if (blockName->Is_text_long()) {
        width = 200;
        blockName->setTextWidth(250);
    }

    height = height + (blockName->Scale());

    setRect(0, 0, width, height);
    blockName->setAlignCenter(0); //to do: block types, have to do sth with argument
	setBrush(myItemColor);

    // IO circles
    for (int i = 0; i < blockPtr->inputs.size(); i++) {
        BlockIn * entry = new BlockIn(i, this);
        In.append(entry);
        //first_entry->DrawIn(i,blockPtr->In[i]);
        entry->DrawIn(QString::number(blockPtr->inputs[i].outputId));
    }

    for (int i = 0; i < blockPtr->outputs.size(); i++) {
        BlockOut * exit = new BlockOut(i, blockPtr->outputs[i].id, this);
        Out.append(exit);
        //first_exit->DrawOut(i,blockPtr->Out[i]);
		exit->DrawOut(QString::number(blockPtr->outputs[i].id));
    }

    // Load arrows - when loading file

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramOperation::~DiagramOperation()
{
}
