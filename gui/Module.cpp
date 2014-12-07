#include "Module.h"
Module::Module(QString name, QSize size, QVector <QString> id_in, QVector<QString> id_out, int t)
{
    this->name = name;
    this->blockSize = size;
    this->In = id_in;
    this->Out = id_out;
    this->blockType=t;
}
Module::Module()
{
}

int Module::getType()
{
return blockType;            
}

