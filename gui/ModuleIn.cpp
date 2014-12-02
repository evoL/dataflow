#include "modulein.h"
#include "Arrow.h"
#include <QPolygonF>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

ModuleIn::ModuleIn(QGraphicsItem * parent) : QGraphicsEllipseItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void ModuleIn::DrawIn(int i)
{
    posi = i;
    setBrush(Qt::red);
    setRect(-7, 15 + posi*20, 15, 15);
}

void ModuleIn::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //QStyleOptionGraphicsItem * style = const_cast<QStyleOptionGraphicsItem *>(option);

    bool is_selected = option->state & QStyle::State_Selected;
    highlight(painter, is_selected);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void ModuleIn::highlight(QPainter * painter, bool is_selected)
{
    if (is_selected) {
        setBrush(Qt::green);
    } else {
        setBrush(Qt::red);
    }
    setRect(-7, 15 + posi*20, 15, 15);
}

void ModuleIn::removeArrow(Arrow * arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
void ModuleIn::removeArrows()
{
    foreach (Arrow * arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
void ModuleIn::addArrow(Arrow * arrow)
{
    arrows.append(arrow);
}

QPointF ModuleIn::pos() const
{
    QPoint q(0, 22 + 20 * posi); // y = 15+7+20*posi
    return parentItem()->pos() + q;
}
