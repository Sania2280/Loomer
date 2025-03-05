#pragma once

#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

#include "qmutex.h"
#include "Config.hpp"

class Sending;

class server : public QTcpServer {
    Q_OBJECT

public:
    server(const Config::Settings& aSettings);
    void setSending(Sending &sending);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();

public:
    static QList<QTcpSocket *> Sockets;
private:
    static QMutex mutex;

    Sending *sendingPtr = nullptr;
    qint16 server_port;
    QHostAddress::SpecialAddress addressEnum;

signals:
    void newClientConnected(QTcpSocket *socet, QList<QTcpSocket *> &Sockets);
    void disconnectedClient(qintptr socet, QString IP);
    void sendingMesage(QTcpSocket *socket, const QString &message);
};



class Loger{
public:
    static void myLogMessageHandler(const QtMsgType type, const QMessageLogContext& context, const QString& msg);

};


