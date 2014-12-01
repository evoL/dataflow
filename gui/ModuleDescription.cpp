#include "moduledescription.h"


ModuleDescription::ModuleDescription(QString s, QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    text_description = s;
    setPlainText(s);
}
ModuleDescription::ModuleDescription(QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
    text_description = "";

}

void ModuleDescription::SetDescription(const QString desc)
{
    setPlainText(desc);
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
