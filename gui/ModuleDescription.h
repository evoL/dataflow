#ifndef MODULEDESCRIPTION_H
#define MODULEDESCRIPTION_H

#include <QGraphicsTextItem>
#include <QString>

class ModuleDescription : public QGraphicsTextItem
{
public:
    ModuleDescription(QGraphicsItem * parent = 0);
    ModuleDescription(QString s, QGraphicsItem * parent = 0);
    void SetDescription(const QString desc);
    void setAlignCenter();
    qreal Scale();
    bool Is_text_long();
private:
    QString text_description;
};

#endif // MODULEDESCRIPTION_H
