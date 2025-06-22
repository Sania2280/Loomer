#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <windows.h>

#include "sending.h"
#include "enums.h"
#include "cliendatabase.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wshadow"

#include "Mpack.hpp"

#pragma GCC diagnostic pop

QMutex mutex1;

Sending::Sending(server *srv, QObject *parent)
    : QThread(parent), m_server(srv) {}


void Sending::Get_New_Client(QString newId, QMap<QString , server::ClientInfo> &clientsData) {

    ClientsData = clientsData;

    QThread::msleep(100);

    for (auto orderdId = ClientsData.begin(); orderdId != ClientsData.end(); ++orderdId) {

        QString key = orderdId.key();
        server::ClientInfo orderdClient = orderdId.value();


        // Пропускаем отключенные сокеты
        if (orderdClient.desk->state() != QAbstractSocket::ConnectedState) {
            qWarning() << "Skipping disconnected socket:" << orderdClient.desk->socketDescriptor();
            continue;
        }

        // Пропускаем сам себя
        if (newId == key) {
            continue;
        }

        // Отправляем новому клиенту информацию о других сокетах

        ClienDataBase clientDB;


        Message messToSend1 = ObjectToSend(MesageIdentifiers::ID_CLIENT,
                                           orderdClient.desk->peerAddress().toString(),
                                           QString::number(orderdClient.desk->socketDescriptor()),
                                           orderdClient.nick,
                                           key);


        sendToSocket(ClientsData[newId].desk, messToSend1);

        // Отправляем другим сокетам информацию о новом клиенте


        Message messToSend2 = ObjectToSend(MesageIdentifiers::ID_CLIENT,
                                           ClientsData[newId].desk->peerAddress().toString(),
                                           QString::number(ClientsData[newId].desk->socketDescriptor()),
                                           ClientsData[newId].nick,
                                           newId);

        sendToSocket(orderdClient.desk, messToSend2);
    }

}

void Sending::Get_Disconnected_Client(qintptr socket) {

    ClienDataBase clientDB;

    QString clientIDtoRemuve = clientDB.GetId(QString::number(socket));
    QString clientDesktoRemuve = clientDB.GetDesk(clientIDtoRemuve);


    for (auto i = ClientsData.begin(); i != ClientsData.end(); i++) {

        QString key = i.key();
        server::ClientInfo orderdClient = i.value();

        if(key == clientIDtoRemuve){
            continue;
        }

        Message messToSend3 = ObjectToSend(MesageIdentifiers::ID_DELETE,
                                           ClientsData[clientIDtoRemuve].desk->peerAddress().toString(),
                                           clientDesktoRemuve,
                                           ClientsData[clientIDtoRemuve].nick,
                                           clientIDtoRemuve);

        sendToSocket(orderdClient.desk, messToSend3);
    }


    Sockets.removeAll(ClientsData[clientIDtoRemuve].desk);
    ClientsData.remove(clientIDtoRemuve);
}

void Sending::sendToSocket(QTcpSocket *socket, Message message) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Socket not connected:" << socket->socketDescriptor();
        return;
    }


    qDebug() << message.registrationData.desckriptor;

    socket->write(Mpack::puck(message).data());

    socket->flush();

    qDebug() << "Mess size: " << sizeof(message);



    // if (!socket->waitForBytesWritten(5000)) { // Timeout для предотвращения вечного ожидания
    //     qWarning() << "Error waiting for bytes to be written:"
    //                << socket->errorString();
    // }

}


Message Sending::ObjectToSend(MesageIdentifiers MESID, QString IP, QString DESCK, QString NICK, QString ID)
{
    Message messToSend;

    qDebug() << "ID: " << ID;

    messToSend.id = MESID;
    messToSend.newOrDeleteClientInNet.id = ID.toStdString();
    messToSend.newOrDeleteClientInNet.ip = IP.toStdString();
    messToSend.newOrDeleteClientInNet.descriptor = DESCK.toStdString();
    messToSend.newOrDeleteClientInNet.nick = NICK.toStdString();


    qDebug() << "IP: " << messToSend.newOrDeleteClientInNet.ip;
    qDebug() << "DESCK: " << messToSend.newOrDeleteClientInNet.descriptor;

    return messToSend;
}


