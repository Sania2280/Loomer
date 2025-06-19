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

};

#endif // CLIENTDATABASE_H
