#pragma once

#include <QString>
#include "message.h"

class Mpack{
private:
    Mpack() = delete;
    ~Mpack() = delete;
public:
    static Message unpack(std::string rawData);
    static std::string puck(Message &rawData);
};
