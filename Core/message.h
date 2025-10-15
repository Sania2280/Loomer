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
    std::string message;
    MSGPACK_DEFINE(message, senderId, resivId);

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

struct CrypKey
{
    std::vector<unsigned char> openKey;
    MSGPACK_DEFINE( openKey);
};

struct OpenData
{
    std::vector<unsigned char> p;
    std::vector<unsigned char> q;
    std::string resId;
    MSGPACK_DEFINE(p, q, resId);
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

    CrypKey cryptoKey;
    OpenData openData;

    MSGPACK_DEFINE(id, dbID, messageData, registrationData, newOrDeleteClientInNet, reconnect, cryptoKey, openData);
};


#endif // MESSAGE_H
