#pragma once

#include <QString>

class Mpack{
public:
    static QString unpack(QByteArray rawData);
    static std::string puck(QString rawData);
};

