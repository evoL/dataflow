#ifndef BlockIn_H
#define BlockIn_H

#include <QPolygonF>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>
#include <QStyleOptionGraphicsItem>
#include <QList>
#include <QPointF>

class Arrow;

class BlockIn : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 1 };
    BlockIn(int index, QGraphicsItem * parent = 0);
	int getIndex() { return index; }
    void drawIn(QString text);
    //void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    //void highlight(QPainter * painter, bool isSelected);
    int type() const { return Type;}
    void addArrow(Arrow * arrow);
    void removeArrow(Arrow * arrow);
    void removeArrows();
    QPointF pos() const;

private:
    int index;
    QList<Arrow *> arrows;
};

#endif // BlockIn_H
