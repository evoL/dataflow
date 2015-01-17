#include "DiagramConstructor.h"

DiagramConstructor::DiagramConstructor(const std::shared_ptr<Block> blockPointer, const LibraryMap & libraries, QMenu * contextMenu, QGraphicsItem * parent)
    : constructorPointer(std::static_pointer_cast<Constructor>(blockPointer))
{
    myContextMenu = contextMenu;

    width = 170;
    height = 50;

    blockName = new BlockDescription(QString::fromStdString(constructorPointer->type), this);
    if (blockName->isLong()) {
        width = 200;
        blockName->setTextWidth(250);
    }

    height = height + (blockName->scale());

    setRect(0, 0, width, height);
    blockName->setAlignCenter(-1); //to do: block types, have to do sth with argument
    setBrush(myInputColor);

    QLineEdit *block_input1;
    block_input1=new QLineEdit();
    QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);
    block_input1->setFixedWidth(100);
    block_input1->setPlaceholderText("Input");

    if (!constructorPointer->data.empty()) {
        block_input1->setText(QString::fromStdString(constructorPointer->data));
    }

    proxy->setWidget(block_input1);
    proxy->setPos(5, 30);

    // Output circle
	// Assumption: Constructors have only one output
	BlockOut * exit = new BlockOut(0, constructorPointer->outputs[0].id, this); 
    Out.append(exit); 

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramConstructor::~DiagramConstructor()
{
}
