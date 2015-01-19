#ifndef DIAGRAMOPERATION_H
#define DIAGRAMOPERATION_H

#include "DataflowModel.h"
#include "DiagramBlock.h"
#include "BlockDescription.h"
#include "BlockIn.h"
#include "BlockOut.h"

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector>

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

class DiagramOperation : public DiagramBlock
{
public:
    enum { Type = UserType + 32 };
    DiagramOperation();
    DiagramOperation(const ProjectModel * model, const std::shared_ptr<Block> operationPointer, QMenu * contextMenu, QGraphicsItem * parent = 0);
    ~DiagramOperation();
    int type() const { return Type; }

    virtual int getId() { return operationPointer->id; }
    virtual qreal getHeight() { return height; }
    virtual qreal getWidth() { return width; }
    virtual float getX() { return operationPointer->position.x;  }
    virtual float getY() { return operationPointer->position.y; }

    virtual QString moduleName();

    bool isEntryPoint();
    void updateAppearance();

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
    const ProjectModel * model;
    const std::shared_ptr<Operation> operationPointer;
};

#endif
