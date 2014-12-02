#ifndef MODULEOUT_H
#define MODULEOUT_H

#include <QGraphicsPolygonItem>
#include <QStyleOptionGraphicsItem>
#include <QList>
#include <QPointF>

class Arrow;
class ModuleOut : public QGraphicsEllipseItem
{
public:

    enum { Type = UserType + 2 };
    ModuleOut(QGraphicsItem * parent = 0);
    void DrawOut(int i);
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

#endif // MODULEOUT_H
