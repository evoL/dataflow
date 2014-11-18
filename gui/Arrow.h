#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "DiagramModuleItem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class DiagramModuleItem;

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(DiagramModuleItem *startItem, DiagramModuleItem *endItem,
      QGraphicsItem *parent = 0);

    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color) { myColor = color; }
    DiagramModuleItem *startItem() const { return myStartItem; }
    DiagramModuleItem *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    DiagramModuleItem *myStartItem;
    DiagramModuleItem *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};

#endif // ARROW_H
