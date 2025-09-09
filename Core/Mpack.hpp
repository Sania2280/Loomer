#pragma once

#include <QString>
#include "message.h"

class Mpack{
private:
    Mpack() = delete;
    ~Mpack() = delete;
public:
    static std::string puckMes(Message &rawData);
    static Message unpackMes(std::string rawData);

    static std::string puckKey(KeySending &rawData);
    static CryptoEnum unpackKey(std::string rawData);
};
