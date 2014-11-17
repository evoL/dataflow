#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QSize>

class Module
{
public:
    Module(QString name, QSize size);

    QString toString() const { return name; }
    QSize size() const { return blockSize; }
    void setName(QString newName) { name = newName; }

private:
    Module();

    QString name;
    QSize blockSize;
};

#endif // MODULE_H
