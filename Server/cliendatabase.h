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

    struct LogInStruct
    {
        MesageIdentifiers mesID;
        QString clientID;
    };

    void CreateateDB();
    LogInStruct LogIn(QString nick, QString pass);
    MesageIdentifiers SingUp(QString nick, QString pass, int descriptor);

    int ClientID(QJsonObject database);

    QString GetId(QString desk);
    QString GetNick(std::string id);
    QString GetDesk(QString id);
    void RewriteDesk(std::string id, QString desk);

    QJsonObject ReadFile(QString name);


private:


signals:
};

#endif // CLIENDATABASE_H
