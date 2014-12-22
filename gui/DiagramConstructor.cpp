#include "DiagramConstructor.h"


DiagramConstructor::DiagramConstructor()
{
}

DiagramConstructor::DiagramConstructor(const std::shared_ptr<Block> blockPointer, QMenu * contextMenu, QGraphicsItem * parent) 
	: blockPtr(std::static_pointer_cast<Constructor>(blockPointer))
{
    myContextMenu = contextMenu;

    width = 170;
    height = 100;

    blockName = new BlockDescription(QString::fromStdString(blockPtr->type), this);
    if (blockName->Is_text_long()) {
        width = 200;
        blockName->setTextWidth(250);
    }

    height = height + (blockName->Scale());

    setRect(0, 0, width, height);
    blockName->setAlignCenter(-1); //to do: block types, have to do sth with argument
	setBrush(myInputColor);

    // Output circles
    for (int i = 0; i < blockPtr->outputs.size(); i++) {
        BlockOut * first_exit = new BlockOut(i, blockPtr->outputs[i].id, this);
        Out.append(first_exit);
        //first_exit->DrawOut(i,blockPtr->Out[i]);
		first_exit->DrawOut(QString::number(blockPtr->outputs[i].id));
    }

    // Load arrows

    //here

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramConstructor::~DiagramConstructor()
{
}
