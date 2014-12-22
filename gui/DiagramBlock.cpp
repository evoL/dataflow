#include "DiagramBlock.h"
#include "Arrow.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include "BlockDescription.h"
#include "BlockIn.h"
#include "BlockOut.h"
#include <QPolygonF>
#include <QColor>

QColor DiagramBlock::myItemColor = QColor(169, 191, 215);
QColor DiagramBlock::myInputColor = QColor(255, 251, 160);
//QColor DiagramBlock::myOutputColor = QColor(140, 255, 151);



/*DiagramBlock::DiagramBlock(const Block *block, QMenu * contextMenu, QGraphicsItem * parent) :
    QGraphicsRectItem(parent), blockPtr(block)
{
    myContextMenu = contextMenu;

    width = 170;
    height = 100;

    blockName = new BlockDescription(blockPtr->name, this);
    if (blockName->Is_text_long()) {
        width = 200;
        blockName->setTextWidth(250);
    }

    height = height + (blockName->Scale());

    setRect(0, 0, width, height);
    blockName->setAlignCenter(blockPtr->getType());

    // IO circles

    if(blockPtr->getType()!=-1)//It's not IN block
    {
    for (int i = 0; i < blockPtr->In.size(); i++) {
        BlockIn * first_entry = new BlockIn(this);
        In.append(first_entry);
        first_entry->DrawIn(i,blockPtr->In[i]);
    }
    }

    if(blockPtr->getType()!=1)//It's not OUT block
    {
    for (int i = 0; i < blockPtr->Out.size(); i++) {
        BlockOut * first_exit = new BlockOut(this);
        Out.append(first_exit);
        first_exit->DrawOut(i,blockPtr->Out[i]);
    }
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}*/

BlockIn * DiagramBlock::findInputByIndex(int index)
{
	//std::for_each(In.begin(), In.end(), [&] (BlockIn &b)->BlockIn* { if (b.getIndex() == index) return &b; });
	auto it = In.begin();
	while (it != In.end())
	{
		if ( (*it)->getIndex() == index) return *it;
		it++;
	}
	return NULL;
}

BlockOut * DiagramBlock::findOutputById(int id)
{
	//std::for_each(In.begin(), In.end(), [&] (BlockIn &b)->BlockIn* { if (b.getIndex() == index) return &b; });
	auto it = Out.begin();
	while (it != Out.end())
	{
		if ((*it)->getId() == id) return *it;
		it++;
	}
	return NULL;
}

void DiagramBlock::removeArrow(Arrow * arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
void DiagramBlock::removeArrows()
{
    foreach (BlockIn * in, In) in->removeArrows();

    foreach (BlockOut * out, Out) out->removeArrows();
}
void DiagramBlock::addArrow(Arrow * arrow)
{
    arrows.append(arrow);
}
void DiagramBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
QVariant DiagramBlock::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow * arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
