#include "keyexchange.h"
#include "message.h"

KeyExchange::KeyExchange() {}

Message KeyExchange::GetOpenKey(std::string resId)
{
    Message mesage;
    mesage.openData.resId = resId;
    return mesage;
}

