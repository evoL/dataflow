#include "ModuleOut.h"
#include "Arrow.h"
#include <QPolygonF>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QTextBlockFormat>
#include <QTextCursor>

ModuleOut::ModuleOut(QGraphicsItem * parent) : QGraphicsEllipseItem(parent)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void ModuleOut::DrawOut(int i, QString text)
{
    posi = i;
    setBrush(Qt::red);
    qreal moduleNameRectHeight = qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->moduleName->getBlockHeight();
    setRect( qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->getWidth() - 8 , moduleNameRectHeight + 5 + posi*20, 15, 15);

    QGraphicsTextItem *outDesc=new QGraphicsTextItem(this);
    outDesc->setPlainText(text.mid(0,10));
    outDesc->setX(qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->getWidth() - 80);
    outDesc->setY(3+posi*20+moduleNameRectHeight);
    outDesc->setTextWidth(70);
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignRight);
    QTextCursor cursor = outDesc->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    outDesc->setTextCursor(cursor);
}

void ModuleOut::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    //QStyleOptionGraphicsItem * style = const_cast<QStyleOptionGraphicsItem *>(option);

    bool is_selected = option->state & QStyle::State_Selected;
    highlight(painter, is_selected);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void ModuleOut::highlight(QPainter * painter, bool is_selected)
{
    if (is_selected) {
        setBrush(Qt::green);
    } else {
        setBrush(Qt::red);
    }
    qreal moduleNameRectHeight = qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->moduleName->getBlockHeight();
    setRect( qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->getWidth() - 8 , moduleNameRectHeight + 5 + posi*20, 15, 15);
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
    qreal moduleNameRectHeight = qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->moduleName->getBlockHeight();
    QPoint q(qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->getWidth() , moduleNameRectHeight + 12 + 20 * posi);
    return parentItem()->pos() + q;
}

