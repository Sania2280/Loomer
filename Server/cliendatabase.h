#ifndef CLIENDATABASE_H
#define CLIENDATABASE_H
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
    QString LogIn(QString nick, QString pass);
    QString SingUp(QString nick, QString pass, QTcpSocket* socket);

    int ClientID(QJsonObject database);

private:


signals:
};

#endif // CLIENDATABASE_H
