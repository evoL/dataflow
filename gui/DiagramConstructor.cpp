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

    inputBox = new QLineEdit();
    QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);
    inputBox->setFixedWidth(this->width - 15);
    inputBox->setPlaceholderText("Input");
	inputBox->connect(inputBox, SIGNAL(textChanged(const QString &)), this, SLOT(inputChanged(const QString &)));

    if (!constructorPointer->data.empty()) {
        inputBox->setText(QString::fromStdString(constructorPointer->data));
    }

    proxy->setWidget(inputBox);
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

void DiagramConstructor::inputChanged(const QString & newText)
{
	constructorPointer->data = newText.toStdString();
}