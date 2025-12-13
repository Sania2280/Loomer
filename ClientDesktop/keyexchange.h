#ifndef KEYEXCHANGE_H
#define KEYEXCHANGE_H

#include <QObject>
#include <QWidget>
#include "message.h"


class KeyExchange
{
public:
    KeyExchange();
    Message GetOpenDataMessage(std::string resId, std::string sendId);
    Message GetOpenKeyMessage(std::string resId, std::string sendId, std::string openKey);
};

#endif // KEYEXCHANGE_H
