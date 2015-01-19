#include "BlockOut.h"
#include "Arrow.h"

BlockOut::BlockOut(int index, int outputId, QGraphicsItem * parent) : QGraphicsEllipseItem(parent)
{
    this->index = index;
    this->outputId = outputId;
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void BlockOut::drawOut(QString text)
{
    //index = i;
    setBrush(Qt::red);
    qreal moduleNameRectHeight = static_cast< DiagramBlock * >(parentItem())->blockName->getHeight();
    setRect(static_cast< DiagramBlock * >(parentItem())->getWidth() - 8 , moduleNameRectHeight + 5 + index * 20, 15, 15);

    if (text == "") return;

    QGraphicsTextItem * outDesc = new QGraphicsTextItem(this);
    outDesc->setPlainText(text);
    outDesc->setX(static_cast< DiagramBlock * >(parentItem())->getWidth() - 80);
    outDesc->setY(3 + index * 20 + moduleNameRectHeight);
    outDesc->setTextWidth(70);
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignRight);
    QTextCursor cursor = outDesc->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    outDesc->setTextCursor(cursor);
}

/*void BlockOut::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //QStyleOptionGraphicsItem * style = const_cast<QStyleOptionGraphicsItem *>(option);

    bool is_selected = option->state & QStyle::State_Selected;
    //highlight(painter, is_selected);
    setBrush(Qt::red);
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    setRect(static_cast< DiagramBlock* >(parentItem())->getWidth() - 8, moduleNameRectHeight + 10 + index * 20, 15, 15);

    QGraphicsEllipseItem::paint(painter, option, widget);
}

void BlockOut::highlight(QPainter * painter, bool isSelected)
{
    if (isSelected) {
        setBrush(Qt::green);
    } else {
        setBrush(Qt::red);
    }
    qreal moduleNameRectHeight = static_cast< DiagramBlock* >(parentItem())->blockName->getBlockHeight();
    setRect( static_cast< DiagramBlock* >(parentItem())->getWidth() - 8 , moduleNameRectHeight + 5 + index*20, 15, 15);
}*/

void BlockOut::removeArrow(Arrow * arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void BlockOut::removeArrows()
{
    foreach (Arrow * arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void BlockOut::addArrow(Arrow * arrow)
{
    arrows.append(arrow);
}

QPointF BlockOut::pos() const
{
    qreal moduleNameRectHeight = static_cast< DiagramBlock * >(parentItem())->blockName->getHeight();
    QPoint q(static_cast< DiagramBlock * >(parentItem())->getWidth() , moduleNameRectHeight + 12 + 20 * index);
    return parentItem()->pos() + q;
}

