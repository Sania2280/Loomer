#ifndef SERVER_H
#define SERVER_H

#include "message.h"
#include "qmutex.h"
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>
#include "config.hpp"

class Sending;

class server : public QTcpServer {
    Q_OBJECT

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();

public:
    server(const Config::Settings& aSettings);
    void setSending(Sending &sending);
    struct ClientInfo
    {
        QTcpSocket* desk;
        QString nick;
    };


    static QList <QTcpSocket*> Sockets;

    QMap<QString , ClientInfo> ClientsData;
    static QList<QTcpSocket *> TempSockets;
private:
    static QMutex mutex;

    Sending *sendingPtr = nullptr;
    qint16 server_port;
    QHostAddress::SpecialAddress addressEnum;

signals:
    void newClientConnected(QString id, QMap<QString , ClientInfo> &ClientsData);
    void disconnectedClient(qintptr socet);
    void sendingMesage(QTcpSocket *socket, Message &message);
};

#endif // SERVER_H
