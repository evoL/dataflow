#include "BlockIn.h"
#include "Arrow.h"
#include <QPolygonF>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsTextItem>

BlockIn::BlockIn(int index, QGraphicsItem * parent) : QGraphicsEllipseItem(parent)
{
	this->index = index;
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void BlockIn::DrawIn(QString text)
{
    setBrush(Qt::red);
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    setRect(-7, moduleNameRectHeight + 5 + index*20, 15, 15);

    QGraphicsTextItem *inDesc=new QGraphicsTextItem(this);
    inDesc->setPlainText(text.mid(0,10));
    inDesc->setX(7);
    inDesc->setY(3+index*20+moduleNameRectHeight);
}

void BlockIn::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //QStyleOptionGraphicsItem * style = const_cast<QStyleOptionGraphicsItem *>(option);

    bool is_selected = option->state & QStyle::State_Selected;
    highlight(painter, is_selected);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void BlockIn::highlight(QPainter * painter, bool is_selected)
{
    if (is_selected) {
        setBrush(Qt::green);
    } else {
        setBrush(Qt::red);
    }
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    setRect(-7, moduleNameRectHeight + 5 + index*20, 15, 15);
}

void BlockIn::removeArrow(Arrow * arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
void BlockIn::removeArrows()
{
    foreach (Arrow * arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
void BlockIn::addArrow(Arrow * arrow)
{
    arrows.append(arrow);
}

QPointF BlockIn::pos() const
{
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    QPoint q(0, moduleNameRectHeight + 12 + 20 * index); // y = 15+7+20*index
    return parentItem()->pos() + q;
}
