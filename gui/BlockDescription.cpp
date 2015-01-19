#include "BlockDescription.h"

#include "DiagramBlock.h"
#include "DiagramConstructor.h"
#include "DiagramOperation.h"

BlockDescription::BlockDescription(QString s, QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    textDescription = s;
    setPlainText(textDescription);
    blockHeight = this->boundingRect().height();
}

BlockDescription::BlockDescription(QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    textDescription = "";
    setPlainText(textDescription);
    blockHeight = this->boundingRect().height();
}

void BlockDescription::setDescription(const QString desc)
{
    setPlainText(desc);
}

void BlockDescription::setAlignCenter(int i)
{
    setTextWidth(static_cast<DiagramBlock *>(parentItem())->getWidth() - 1);
    blockHeight = this->boundingRect().height();

    if (i == 0)this->setHtml("<body style='background-color:#4C6BB2;'><center>" + textDescription + "</center></body>");

    if (i == 1)this->setHtml("<body style='background-color:#1FFC52;'><center>" + textDescription + "</center></body>");

    if (i == -1)this->setHtml("<body style='background-color:#D0FC1F;'><center>" + textDescription + "</center></body>");

    //748DC8
    //Setting align
    /*QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = this->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    this->setTextCursor(cursor);*/

    setPos(1, 1);
}

bool BlockDescription::isLong()
{
    return textDescription.length() > 30;
}

qreal BlockDescription::scale()
{
    qreal height = 1;
    qreal text_width;
    text_width = textDescription.length();

    while (text_width > 20) {
        text_width = text_width - 20;
        height = height + 1;
    }

    return 10 * height;
}

