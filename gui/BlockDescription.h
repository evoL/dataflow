#ifndef BlockDescription_H
#define BlockDescription_H

#include <QGraphicsTextItem>
#include <QString>

class BlockDescription : public QGraphicsTextItem
{
public:
    BlockDescription(QGraphicsItem * parent = 0);
    BlockDescription(QString s, QGraphicsItem * parent = 0);
    void SetDescription(const QString desc);
    void setAlignCenter(int i);
    qreal getBlockHeight() { return blockHeight; }
    qreal Scale();
    bool Is_text_long();
private:
    QString text_description;
    qreal blockHeight;
};

#endif // BlockDescription_H
