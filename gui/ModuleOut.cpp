#include "moduleout.h"
#include "Arrow.h"
#include <QPolygonF>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

ModuleOut::ModuleOut(QGraphicsItem * parent) : QGraphicsPolygonItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void ModuleOut::DrawOut(int i)
{
    posi = i;
    setBrush(Qt::red);
    QPolygonF triangle;
    triangle << QPointF(100, -15 + i * 20) << QPointF(100, i * 20) << QPoint(115, -7 + i * 20);
    setPolygon(triangle);
}

void ModuleOut::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QStyleOptionGraphicsItem * style = const_cast<QStyleOptionGraphicsItem *>(option);

    bool is_selected = option->state & QStyle::State_Selected;
    highlight(painter, is_selected);
    QGraphicsPolygonItem::paint(painter, option, widget);
}

void ModuleOut::highlight(QPainter * painter, bool is_selected)
{
    if (is_selected) {
        setBrush(Qt::green);
        QPolygonF triangle;
        triangle << QPointF(100, -15 + posi * 20) << QPointF(100, posi * 20) << QPoint(115, -7 + posi * 20);
        setPolygon(triangle);
    } else {
        setBrush(Qt::red);
        QPolygonF triangle;
        triangle << QPointF(100, -15 + posi * 20) << QPointF(100, posi * 20) << QPoint(115, -7 + posi * 20);
        setPolygon(triangle);
    }

}

void ModuleOut::removeArrow(Arrow * arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
void ModuleOut::removeArrows()
{
    foreach (Arrow * arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
void ModuleOut::addArrow(Arrow * arrow)
{
    arrows.append(arrow);
}

QPointF ModuleOut::pos() const
{
    QPoint q(100, -15 + 20 * posi);
    QPointF p = parentItem()->pos();
    p += q;
    return p;
}
