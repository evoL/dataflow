#ifndef BlockDescription_H
#define BlockDescription_H

#include <QTextBlockFormat>
#include <QTextCursor>
#include <QGraphicsTextItem>
#include <QString>

class BlockDescription : public QGraphicsTextItem
{
public:
    BlockDescription(QGraphicsItem * parent = 0);
    BlockDescription(QString s, QGraphicsItem * parent = 0);
    void setDescription(const QString desc);
    void setAlignCenter(int i);
    qreal getBlockHeight() { return blockHeight; }
    qreal scale();
    bool isLong();
private:
    QString textDescription;
    qreal blockHeight;
};

#endif // BlockDescription_H
