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
    MSGPACK_DEFINE(id, nick, pass );
};

struct NewOrDeleteClientInNet
{
    std::string id;
    std::string nick;
    bool stat;
    MSGPACK_DEFINE(id, nick, stat);
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


struct Key
{
    std::string id;
    std::vector<unsigned char> key;
    MSGPACK_DEFINE(id, key);
};
struct KeySending
{
    CryptoEnum id = CryptoEnum::NON;
    Key key;

    MSGPACK_DEFINE(id, key);
};

#endif // MESSAGE_H
