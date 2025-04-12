#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include "enums.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wshadow"

#include <msgpack.hpp>

#pragma GCC diagnostic pop



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
    std::string name;
    MSGPACK_DEFINE(descriptor, ip, name);
};

struct Reconnect
{
    std::string ip;
    std::string desck;
    MSGPACK_DEFINE(desck, ip);

};

struct Message
{
    Message() {}

    MesageIdentifiers id = MesageIdentifiers::NONE;

    MessageData messageData ;
    Reconnect reconnect;
    RegistrationData registrationData;
    NewOrDeleteClientInNet newOrDeleteClientInNet;

    MSGPACK_DEFINE(id, messageData, registrationData, newOrDeleteClientInNet, reconnect);
};


#endif // MESSAGE_H
