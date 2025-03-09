#pragma once

#include <QString>

class Mpack{
private:
    Mpack() = delete;
    ~Mpack() = delete;
public:
    static QString unpack(QByteArray rawData);
    static std::string puck(QString rawData);
};
