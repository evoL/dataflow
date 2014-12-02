#include "ModuleDescription.h"
#include "DiagramModuleItem.h"
#include <QTextBlockFormat>
#include <QTextCursor>

ModuleDescription::ModuleDescription(QString s, QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    text_description = s;
    setPlainText(text_description);
}
ModuleDescription::ModuleDescription(QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    text_description = "";
    setPlainText(text_description);
}

void ModuleDescription::SetDescription(const QString desc)
{
    setPlainText(desc);
}

void ModuleDescription::setAlignCenter()
{
    setTextWidth(qgraphicsitem_cast< DiagramModuleItem* >(parentItem())->getWidth() - 30);

    //Setting align
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = this->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    this->setTextCursor(cursor);

    setPos(15, 0);
}

bool ModuleDescription::Is_text_long()
{
    if (text_description.length() > 30) return true;
    else return false;
}

qreal ModuleDescription::Scale()
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
