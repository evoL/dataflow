#include "BlockDescription.h"
#include "DiagramBlock.h"
#include "DiagramConstructor.h"
#include "DiagramOperation.h"
#include <QTextBlockFormat>
#include <QTextCursor>

BlockDescription::BlockDescription(QString s, QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    text_description = s;
    setPlainText(text_description);
    blockHeight = this->boundingRect().height();
    //setFlag(QGraphicsItem::ItemIsSelectable, true);

	
}
BlockDescription::BlockDescription(QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    text_description = "";
    setPlainText(text_description);
    blockHeight = this->boundingRect().height();
}

void BlockDescription::SetDescription(const QString desc)
{
    setPlainText(desc);
}

void BlockDescription::setAlignCenter(int i)
{
	DiagramBlock *parr = static_cast<DiagramBlock*> (parentItem());
    setTextWidth(parr->getWidth() - 1);
    blockHeight = this->boundingRect().height();

    if(i==0)this->setHtml("<body style='background-color:#4C6BB2;'><center>" + text_description + "</center></body>");
    if(i==1)this->setHtml("<body style='background-color:#1FFC52;'><center>" + text_description + "</center></body>");
    if(i==-1)this->setHtml("<body style='background-color:#D0FC1F;'><center>" + text_description + "</center></body>");

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

bool BlockDescription::Is_text_long()
{
    if (text_description.length() > 30) return true;
    else return false;
}

qreal BlockDescription::Scale()
{
    qreal height = 1;
    qreal text_width;
    text_width = text_description.length();

    while (text_width > 20) {
        text_width = text_width - 20;
        height = height + 1;
    }

    return 10 * height;
}

