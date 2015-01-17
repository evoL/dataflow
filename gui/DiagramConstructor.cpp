#include "DiagramConstructor.h"

DiagramConstructor::DiagramConstructor(const std::shared_ptr<Block> blockPointer, const LibraryMap & libraries, QMenu * contextMenu, QGraphicsItem * parent)
    : constructorPointer(std::static_pointer_cast<Constructor>(blockPointer))
{
    myContextMenu = contextMenu;

	width = 150;
	height = 46;

	blockName = new BlockDescription(QString::fromStdString(constructorPointer->type), this);
    blockName->setAlignCenter(-1); //to do: block types, have to do sth with argument
    setBrush(myInputColor);

    QLineEdit *block_input1;
    block_input1=new QLineEdit();
    QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);
    block_input1->setFixedWidth(this->width - 15);
    block_input1->setPlaceholderText("Input");

    if (!constructorPointer->data.empty()) {
        block_input1->setText(QString::fromStdString(constructorPointer->data));
    }

    proxy->setWidget(block_input1);
    proxy->setPos(3, blockName->getHeight() + 3);

    // Output circle
    // Assumption: Constructors have only one output
    BlockOut * exit = new BlockOut(0, constructorPointer->outputs[0].id, this);
    Out.append(exit);
	exit->drawOut(); 

	/*if (blockName->isLong()) {
		width = 200;
		blockName->setTextWidth(250);
	}*/

	height = blockName->getHeight() + proxy->boundingRect().height() + 6; // 6 = 3 + 3 spacing for input
	setRect(0, 0, width, height);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


DiagramConstructor::~DiagramConstructor()
{
}

void DiagramConstructor::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // See the comment in DiagramOperation.cpp
    QAction *firstAction = myContextMenu->actions().first();
    firstAction->setChecked(false);
    firstAction->setEnabled(false);

    DiagramBlock::contextMenuEvent(event);
}
