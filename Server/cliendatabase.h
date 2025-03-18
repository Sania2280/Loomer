#ifndef CLIENDATABASE_H
#define CLIENDATABASE_H
#include "enums.h"
#include <qobject.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTcpSocket>

class ClienDataBase : public QObject
{
public:
    explicit ClienDataBase(QObject *parent = nullptr);

    void CreateateDB();
    MesageIdentifiers LogIn(QString nick, QString pass);
    MesageIdentifiers SingUp(QString nick, QString pass, int descriptor);

    int ClientID(QJsonObject database);

private:


signals:
};

#endif // CLIENDATABASE_H
