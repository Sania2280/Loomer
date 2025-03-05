#pragma once

#include <QString>

class M_pack{
public:
    QString unpack(QByteArray rawData);
    std::string puck(QString rawData);
};

