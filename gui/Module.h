#ifndef MODULE_H
#define MODULE_H
#include <QString>
#include <QSize>
#include <QVector>
class Module
{
public:
    Module(QString name, QSize size, QVector<QString> id_in, QVector<QString> id_out);
    QString toString() const { return name; }
    QSize size() const { return blockSize; }
    void setName(QString newName) { name = newName; }
    Module();
    QVector<QString> In;
    QVector<QString> Out;
    QString name;
private:
    QSize blockSize;

};
#endif // MODULE_H
