#ifndef ARROW_H
#define ARROW_H
#include <QGraphicsLineItem>
#include "DiagramModuleItem.h"
#include "ModuleIn.h"
#include "ModuleOut.h"
QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QPolygonF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE
class DiagramModuleItem;
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };
    Arrow(ModuleIn * startItem, ModuleOut * endItem,
          QGraphicsItem * parent = 0);
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor & color) { myColor = color; }
    ModuleIn * startItem() const { return myStartItem; }
    ModuleOut * endItem() const { return myEndItem; }
    void updatePosition();
protected:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private:
    ModuleIn * myStartItem;
    ModuleOut * myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};
#endif // ARROW_H
