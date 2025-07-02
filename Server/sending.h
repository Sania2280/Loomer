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
    QString MesType(MesageIdentifiers id);
    QMap<QString , server::ClientInfo> ClientsData;
    Message ObjectToSend(MesageIdentifiers MESID, QString IP, QString ID);

public slots:
    void GetDisconnectedClient(qintptr socket);
    void SendToSocket(QString socket, Message &message);
    void SendToSocketLogInData(QTcpSocket* socket, Message &message);
    void MessagePacker(MesageIdentifiers mesId, QString newId,  QMap<QString , server::ClientInfo> &clientsData);
};

#endif // SENDING_H
