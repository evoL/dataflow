#ifndef DIAGRAMMODULEITEM_H
#define DIAGRAMMODULEITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>

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
    enum { Type = UserType + 30 };
    DiagramModuleItem() {}
    DiagramModuleItem(QMenu *contextMenu, QGraphicsItem *parent = 0);

    void addArrow(Arrow *arrow);
    void removeArrow(Arrow *arrow);
    void removeArrows();
    int type() const { return Type;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QMenu *myContextMenu;
    QList<Arrow *> arrows;
};

#endif // DIAGRAMMODULEITEM_H
