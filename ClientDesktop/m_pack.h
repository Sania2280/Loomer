#ifndef M_PACK_H
#define M_PACK_H
#include <QString>
#include "message.h"

class M_pack{
public:
    std::string puck(Message& rawData);
    Message unpack(std::string rawData);
};

#endif // M_PACK_H
