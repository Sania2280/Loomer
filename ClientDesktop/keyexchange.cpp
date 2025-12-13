#include "keyexchange.h"
#include "message.h"

KeyExchange::KeyExchange() {}

Message KeyExchange::GetOpenDataMessage(std::string resId, std::string sendId)
{
    Message mesage;
    mesage.id = MesageIdentifiers::OPENDATA;
    mesage.openData.resId = resId;
    mesage.openData.sendId = sendId;
    return mesage;
}

Message KeyExchange::GetOpenKeyMessage(std::string resId, std::string sendId, std::string openKey)
{
    Message message;
    message.id = MesageIdentifiers::OPENKEY;
    message.openKeyPac.openKey = openKey;
    message.openKeyPac.resId = resId;
    message.openKeyPac.sendId = sendId;

    return message;
}

