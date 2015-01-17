#ifndef DIAGRAMCONSTRUCTOR_H
#define DIAGRAMCONSTRUCTOR_H

#include "DataflowModel.h"
#include "DiagramBlock.h"
#include "BlockDescription.h"
#include "BlockIn.h"
#include "BlockOut.h"

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector>
#include <QGraphicsProxyWidget>
#include <QLineEdit>

#include <iostream>

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

class DiagramConstructor : public DiagramBlock
{
public:
    enum { Type = UserType + 31 };
    DiagramConstructor() = delete;
    DiagramConstructor(const std::shared_ptr<Block> blockPointer, const LibraryMap & libraries, QMenu * contextMenu, QGraphicsItem * parent = 0);
    ~DiagramConstructor();
    int type() const { return Type; }

    virtual int getId() { return constructorPointer->id;  }
    virtual qreal getHeight() { return height; }
    virtual qreal getWidth() { return width; }
    virtual float getX() { return constructorPointer->position.x; }
    virtual float getY() { return constructorPointer->position.y; }

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
    const std::shared_ptr<Constructor> constructorPointer;
};

#endif
