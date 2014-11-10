#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "diagrammoduleitem.h"

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QColor;
QT_END_NAMESPACE

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, MoveItem };

    explicit DiagramScene(QMenu *itemMenu, QObject *parent = 0);

public slots:
    void setMode(Mode mode);

signals:
    void itemInserted();
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QMenu *myItemMenu;
    Mode myMode;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
};

#endif // DIAGRAMSCENE_H
