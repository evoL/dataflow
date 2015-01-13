#include "DiagramConstructor.h"
#include <QGraphicsProxyWidget>
#include <QLineEdit>

DiagramConstructor::DiagramConstructor(const std::shared_ptr<Block> blockPointer, QMenu * contextMenu, QGraphicsItem * parent)
    : constructorPointer(std::static_pointer_cast<Constructor>(blockPointer))
{
    myContextMenu = contextMenu;

    width = 170;
    height = 100;

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
    proxy->setPos(5,40);

    // Output circles
    for (int i = 0; i < constructorPointer->outputs.size(); i++) {
        BlockOut * first_exit = new BlockOut(i, constructorPointer->outputs[i].id, this);
        Out.append(first_exit);
        first_exit->drawOut(QStringLiteral("(%1)").arg(constructorPointer->outputs[i].id));
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramConstructor::~DiagramConstructor()
{
}
