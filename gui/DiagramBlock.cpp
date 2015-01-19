#include "DiagramBlock.h"
#include "Arrow.h"
#include "BlockDescription.h"
#include "BlockIn.h"
#include "BlockOut.h"
#include "DataflowModel.h"

QColor DiagramBlock::myItemColor = QColor(169, 191, 215);
QColor DiagramBlock::myInputColor = QColor(255, 251, 160);

BlockIn * DiagramBlock::findInputByIndex(int index)
{
    auto it = In.begin();

    while (it != In.end()) {
        if ((*it)->getIndex() == index) return *it;

        it++;
    }

    return NULL;
}

BlockOut * DiagramBlock::findOutputById(int id)
{
    auto it = Out.begin();

    while (it != Out.end()) {
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
