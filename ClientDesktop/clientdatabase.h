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

    QVector<QString> GetAllUsersNicks();
    QJsonObject ReadFile(QString name);

    QString GetNick(QString id);
    QString GetId(QString Nick);

};

#endif // CLIENTDATABASE_H
