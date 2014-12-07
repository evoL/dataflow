#ifndef MODULE_H
#define MODULE_H
#include <QString>
#include <QSize>
#include <QVector>
class Module
{
public:
    Module(QString name, QSize size, QVector<QString> id_in, QVector<QString> id_out,int t=0);
    QString toString() const { return name; }
    QSize size() const { return blockSize; }
    void setName(QString newName) { name = newName; }
    Module();
    QVector<QString> In;
    QVector<QString> Out;
    QString name;
    int getType() const;
private:
    QSize blockSize;
    int blockType; //-1=input block, 1=output block, 0=operation block

};
#endif // MODULE_H
