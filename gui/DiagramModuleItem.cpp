#include "DiagramModuleItem.h"
#include "Arrow.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include "moduledescription.h"
#include "modulein.h"
#include "moduleout.h"
#include <QPolygonF>
#include <QColor>



DiagramModuleItem::DiagramModuleItem(const Module *module, QMenu *contextMenu, QGraphicsItem *parent) :
QGraphicsRectItem(parent), modulePtr(module)
{

    for(int i=0;i< modulePtr->In.size();i++)
    {
        ModuleIn *first_entry=new ModuleIn(this);
        In.append(first_entry);
        first_entry->DrawIn(i);
    }

    for(int i=0;i< modulePtr->Out.size();i++)
    {
        ModuleOut *first_exit=new ModuleOut(this);
        Out.append(first_exit);
        first_exit->DrawOut(i);
    }

myContextMenu = contextMenu;

ModuleDescription *text=new ModuleDescription(modulePtr->name,this);


qreal width=100;
qreal height=100;

if(text->Is_text_long())
{
   width=150;
   text->setTextWidth(150);
}

height=height+(text->Scale());

setRect( -20,-20,width+40,height+20);





setFlag(QGraphicsItem::ItemIsMovable, true);
setFlag(QGraphicsItem::ItemIsSelectable, true);
setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
void DiagramModuleItem::removeArrow(Arrow *arrow)
{
int index = arrows.indexOf(arrow);
if (index != -1)
arrows.removeAt(index);
}
void DiagramModuleItem::removeArrows()
{
foreach (Arrow *arrow, arrows) {
arrow->startItem()->removeArrow(arrow);
arrow->endItem()->removeArrow(arrow);
scene()->removeItem(arrow);
delete arrow;
}
}
void DiagramModuleItem::addArrow(Arrow *arrow)
{
arrows.append(arrow);
}
void DiagramModuleItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
scene()->clearSelection();
setSelected(true);
myContextMenu->exec(event->screenPos());
}
QVariant DiagramModuleItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
if (change == QGraphicsItem::ItemPositionChange) {
foreach (Arrow *arrow, arrows) {
arrow->updatePosition();
}
}
return value;
}
