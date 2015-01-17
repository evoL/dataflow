#ifndef DIAGRAMBLOCK_H
#define DIAGRAMBLOCK_H

#include "BlockDescription.h"
#include "BlockIn.h"
#include "BlockOut.h"

#include <QPolygonF>
#include <QColor>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

#include <memory>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
QT_END_NAMESPACE

class Arrow;

class DiagramBlock : public QGraphicsRectItem
{
public:
    DiagramBlock() {}
    virtual int getId() = 0;
    virtual qreal getHeight() = 0;
    virtual qreal getWidth() = 0;
    virtual float getX() = 0;
    virtual float getY() = 0;
    QVector <BlockIn *> * getBlockInputs() { return &In; }
    QVector <BlockOut *> * getBlockOutputs() { return &Out; }
    BlockIn * findInputByIndex(int index);
    BlockOut * findOutputById(int id);

    void addArrow(Arrow * arrow);
    void removeArrow(Arrow * arrow);
    void removeArrows();

    BlockDescription * blockName;

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    qreal height;
    qreal width;
    QVector <BlockIn *> In;
    QVector <BlockOut *> Out;
    QMenu * myContextMenu;
    QList<Arrow *> arrows;

    static QColor myItemColor;
    static QColor myInputColor;
};
#endif // DIAGRAMBLOCK_H
