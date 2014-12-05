#ifndef DIAGRAMMODULEITEM_H
#define DIAGRAMMODULEITEM_H
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector>
#include "Module.h"
#include "ModuleDescription.h"
#include "ModuleIn.h"
#include "ModuleOut.h"

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
class DiagramModuleItem : public QGraphicsRectItem
{
public:

    ModuleDescription * moduleName;
    enum { Type = UserType + 30 };
    DiagramModuleItem() {}
    DiagramModuleItem(const Module * module, QMenu * contextMenu, QGraphicsItem * parent = 0);
    qreal getHeight() { return height; }
    qreal getWidth() { return width; }
    void addArrow(Arrow * arrow);
    void removeArrow(Arrow * arrow);
    void removeArrows();
    int type() const { return Type;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

private:
    qreal height;
    qreal width;
    QVector <ModuleIn *> In;
    QVector <ModuleOut *> Out;
    QMenu * myContextMenu;
    QList<Arrow *> arrows;
    const Module * modulePtr;
};
#endif // DIAGRAMMODULEITEM_H
