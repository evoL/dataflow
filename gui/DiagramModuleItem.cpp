#include "DiagramModuleItem.h"
#include "DiagramModuleItem.h"
#include "Arrow.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include "ModuleDescription.h"
#include "ModuleIn.h"
#include "ModuleOut.h"
#include <QPolygonF>
#include <QColor>
#include <QGraphicsProxyWidget>


DiagramModuleItem::DiagramModuleItem(const Module * module, QMenu * contextMenu, QGraphicsItem * parent) :
    QGraphicsRectItem(parent), modulePtr(module)
{
    myContextMenu = contextMenu;

    width = 170;
    height = 100;

    moduleName = new ModuleDescription(modulePtr->name, this);
    if (moduleName->Is_text_long()) {
        width = 200;
        moduleName->setTextWidth(250);
    }

    height = height + (moduleName->Scale());

    setRect(0, 0, width, height);
    moduleName->setAlignCenter(modulePtr->getType());
    
    // input

    if(modulePtr->getType()==-1)
    {
        block_input=new QLineEdit();
        QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);
        block_input->setFixedWidth(100);
        block_input->setPlaceholderText("Input");
        proxy->setWidget(block_input);
        proxy->setPos(5,40);
    }

    // IO circles

    if(modulePtr->getType()!=-1)//It's not IN block
    {
    for (int i = 0; i < modulePtr->In.size(); i++) {
        ModuleIn * first_entry = new ModuleIn(this);
        In.append(first_entry);
        first_entry->DrawIn(i,modulePtr->In[i]);
    }
    }

    if(modulePtr->getType()!=1)//It's not OUT block
    {
    for (int i = 0; i < modulePtr->Out.size(); i++) {
        ModuleOut * first_exit = new ModuleOut(this);
        Out.append(first_exit);
        first_exit->DrawOut(i,modulePtr->Out[i]);
    }
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramModuleItem::removeArrow(Arrow * arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
void DiagramModuleItem::removeArrows()
{
    foreach (ModuleIn * in, In) in->removeArrows();

    foreach (ModuleOut * out, Out) out->removeArrows();
}
void DiagramModuleItem::addArrow(Arrow * arrow)
{
    arrows.append(arrow);
}
void DiagramModuleItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
QVariant DiagramModuleItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow * arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
