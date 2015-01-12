#ifndef BlockOut_H
#define BlockOut_H

#include <QGraphicsPolygonItem>
#include <QStyleOptionGraphicsItem>
#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QTextBlockFormat>
#include <QTextCursor>

class Arrow;
class BlockOut : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 2 };
    BlockOut(int index, int outputId, QGraphicsItem * parent = 0);
	int getIndex() { return index; }
	int getId() { return outputId; }
    void drawOut(QString text);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void highlight(QPainter * painter, bool isSelected);
    int type() const { return Type;}
    void addArrow(Arrow * arrow);
    void removeArrow(Arrow * arrow);
    void removeArrows();
    QPointF pos() const;

private:
    int index;
	int outputId;
    QList<Arrow *> arrows;
};

#endif // BlockOut_H
