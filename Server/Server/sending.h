#ifndef SENDING_H
#define SENDING_H

#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include "server.h"



class Sending : public QThread {
    Q_OBJECT

public:
    explicit Sending(server *srv, QObject *parent = nullptr);

private:
    server *m_server;
    QList<QTcpSocket *> Sockets;
    QString String_to_Send(QString ID, QString IP, QString DESCK);


public slots:
    void Get_New_Client(QTcpSocket *socet, QList<QTcpSocket *> Sockets_reciverd);
    void Get_Disconnected_Client(qintptr socket, QString IP);
    void sendToSocket(QTcpSocket *socket, const QString &message);
};

#endif // SENDING_H
