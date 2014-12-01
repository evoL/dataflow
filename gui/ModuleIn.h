#ifndef MODULEIN_H
#define MODULEIN_H

#include <QGraphicsPolygonItem>
#include <QStyleOptionGraphicsItem>
#include <QList>
#include <QPointF>

class Arrow;
class ModuleIn : public QGraphicsPolygonItem
{
public:

    enum { Type = UserType + 1 };
    ModuleIn(QGraphicsItem * parent = 0);
    void DrawIn(int i);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void highlight(QPainter * painter, bool is_selected);
    int type() const { return Type;}
    void addArrow(Arrow * arrow);
    void removeArrow(Arrow * arrow);
    void removeArrows();
    QPointF pos() const;

private:
    int posi;
    QList<Arrow *> arrows;

};

#endif // MODULEIN_H
