#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include <QObject>
#include "message.h"

class ClientDataBase
{
public:
    ClientDataBase();

    void CreateDB();
    void AddUser(Message mesege);
    void ChangeUserStat(QString id);

    QVector<QString> GetAllUsersNicks();
    QJsonObject ReadFile(QString name);

    QString GetNick(QString id);
    QString GetId(QString Nick);

};

#endif // CLIENTDATABASE_H
