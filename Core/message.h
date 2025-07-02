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
    std::string senderId;
    std::string resivId;
    std::string resivIpAddress;
    std::string message;
    MSGPACK_DEFINE(message, senderId, resivId, resivIpAddress);

};

struct RegistrationData
{   std::string id;
    std::string nick;
    std::string pass;
    std::string desckriptor;
    MSGPACK_DEFINE(id, nick, pass, desckriptor );
};

struct NewOrDeleteClientInNet
{
    std::string descriptor;
    std::string id;
    std::string nick;
    MSGPACK_DEFINE(descriptor, id, nick);
};

struct Reconnect
{
    std::string id;
    std::string desck;
    std::string nick;
    MSGPACK_DEFINE(desck, id, nick);

};

struct Message
{
    Message() {}

    MesageIdentifiers id = MesageIdentifiers::NONE;
    std::string dbID;

    MessageData messageData ;
    Reconnect reconnect;
    RegistrationData registrationData;
    NewOrDeleteClientInNet newOrDeleteClientInNet;

    MSGPACK_DEFINE(id, dbID, messageData, registrationData, newOrDeleteClientInNet, reconnect);
};


#endif // MESSAGE_H
