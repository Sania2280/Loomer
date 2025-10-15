#ifndef KEYEXCHANGE_H
#define KEYEXCHANGE_H

#include <QObject>
#include <QWidget>
#include "message.h"


class KeyExchange
{
public:
    KeyExchange();
    Message GetOpenKey(std::string resId);
};

#endif // KEYEXCHANGE_H
