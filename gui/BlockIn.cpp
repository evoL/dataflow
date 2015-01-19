#include "BlockIn.h"
#include "Arrow.h"

BlockIn::BlockIn(int index, QGraphicsItem * parent) : QGraphicsEllipseItem(parent)
{
    this->index = index;
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void BlockIn::drawIn(QString text)
{
    setBrush(Qt::red);
    qreal moduleNameRectHeight = static_cast< DiagramBlock * >(parentItem())->blockName->getHeight();
    setRect(-7, moduleNameRectHeight + 5 + index * 20, 15, 15);

    QGraphicsTextItem * inDesc = new QGraphicsTextItem(this);
    inDesc->setPlainText(text);
    inDesc->setX(7);
    inDesc->setY(3 + index * 20 + moduleNameRectHeight);
}

/*void BlockIn::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //QStyleOptionGraphicsItem * style = const_cast<QStyleOptionGraphicsItem *>(option);

    bool is_selected = option->state & QStyle::State_Selected;
    //highlight(painter, is_selected);
    setBrush(Qt::red);
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    setRect(-7, moduleNameRectHeight + 10 + index * 20, 15, 15);

    QGraphicsEllipseItem::paint(painter, option, widget);
}*/

/*void BlockIn::highlight(QPainter * painter, bool isSelected)
{
    if (isSelected) {
        setBrush(Qt::green);
    } else {
        setBrush(Qt::red);
    }
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    setRect(-7, moduleNameRectHeight + 5 + index*20, 15, 15);
}*/

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
    qreal moduleNameRectHeight = static_cast< DiagramBlock * >(parentItem())->blockName->getHeight();
    QPoint q(0, moduleNameRectHeight + 12 + 20 * index); // y = moduleNameRectHeight + 5 + 7 + 20*index (centre of the circle)
    return parentItem()->pos() + q;
}
