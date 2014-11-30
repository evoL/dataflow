#include "Module.h"
Module::Module(QString name, QSize size, QVector <QString> id_in, QVector<QString> id_out)
{
this->name = name;
this->blockSize = size;
this->In=id_in;
this->Out=id_out;
}
Module::Module()
{
}
