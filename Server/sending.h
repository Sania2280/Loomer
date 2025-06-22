#ifndef SENDING_H
#define SENDING_H

#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include "server.h"
#include "message.h"



class Sending : public QThread {
    Q_OBJECT

public:
    explicit Sending(server *srv, QObject *parent = nullptr);

private:
    server *m_server;
    QList<QTcpSocket *> Sockets;
    QMap<QString , server::ClientInfo> ClientsData;
    Message ObjectToSend(MesageIdentifiers MESID, QString IP, QString DESCK, QString NICK, QString ID);

public slots:
    void Get_New_Client(QString new_id, QMap<QString , server::ClientInfo> &clientsData );
    void Get_Disconnected_Client(qintptr socket);
    void sendToSocket(QTcpSocket *socket, Message message);
};

#endif // SENDING_H
