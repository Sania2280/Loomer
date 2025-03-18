#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "enums.h"
#include <msgpack.hpp>

MSGPACK_ADD_ENUM(MesageIdentifiers);

struct MessageData
{
    std::string senderDesk;
    std::string senderIpAddress;
    std::string resivDesk;
    std::string resivIpAddress;
    std::string message;
    MSGPACK_DEFINE(message, senderDesk, senderIpAddress, resivDesk, resivIpAddress);

};

struct RegistrationData
{
    std::string nickName;
    std::string pass;
    std::string desckriptor;
    MSGPACK_DEFINE(nickName, pass, desckriptor );
};

struct NewOrDeleteClientInNet
{
    std::string descriptor;
    std::string ip;
    MSGPACK_DEFINE(descriptor, ip);
};

struct Message
{
    Message() {}

    MesageIdentifiers id = MesageIdentifiers::NONE;

    MessageData messageData ;
    RegistrationData registrationData;
    NewOrDeleteClientInNet newOrDeleteClientInNet;

    MSGPACK_DEFINE(id, messageData, registrationData, newOrDeleteClientInNet);
};


#endif // MESSAGE_H
