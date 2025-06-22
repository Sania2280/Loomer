#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include <QObject>
#include "message.h"

class ClientDataBase
{
public:
    ClientDataBase();

    void CreateDB();
    void CreateDBfirst(QString desk);
    void AddUser(Message mesege);

    QString GetNick();

};

#endif // CLIENTDATABASE_H
