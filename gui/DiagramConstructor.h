#ifndef DIAGRAMCONSTRUCTOR_H
#define DIAGRAMCONSTRUCTOR_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector>
#include "DataflowModel.h"
#include "DiagramBlock.h"
//#include "Module.h"
#include "BlockDescription.h"
#include "BlockIn.h"
#include "BlockOut.h"

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
	DiagramConstructor();
	DiagramConstructor(const std::shared_ptr<Block> blockPointer, QMenu * contextMenu, QGraphicsItem * parent = 0);
	~DiagramConstructor();
	int type() const { return Type; }

	virtual int getId() { return blockPtr->id;  }
    virtual qreal getHeight() { return height; }
    virtual qreal getWidth() { return width; }
	virtual float getX() { return blockPtr->position.x; }
	virtual float getY() { return blockPtr->position.y; }

private:
	const std::shared_ptr<Constructor> blockPtr;
};

#endif
