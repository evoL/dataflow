#ifndef ARROW_H
#define ARROW_H
#include <QGraphicsLineItem>
#include "DiagramBlock.h"
#include "BlockIn.h"
#include "BlockOut.h"
QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QPolygonF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE
class DiagramBlock;
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };
    Arrow(BlockIn * startItem, BlockOut * endItem,
          QGraphicsItem * parent = 0);
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor & color) { myColor = color; }
    BlockIn * startItem() const { return myStartItem; }
    BlockOut * endItem() const { return myEndItem; }
    void updatePosition();
protected:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private:
    BlockIn * myStartItem;
    BlockOut * myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};
#endif // ARROW_H
