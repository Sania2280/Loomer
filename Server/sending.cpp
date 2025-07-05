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


void Sending::MessagePacker(MesageIdentifiers mesId, QString newId, QMap<QString, server::ClientInfo> &clientsData)
{
    ClientsData = clientsData;

    QThread::msleep(150);


        for (auto orderdId = ClientsData.begin(); orderdId != ClientsData.end(); ++orderdId) {

            QString key = orderdId.key();
            server::ClientInfo orderdClient = orderdId.value();


            // Пропускаем отключенные сокеты
            if (orderdClient.socket->state() != QAbstractSocket::ConnectedState) {
                qWarning() << "Skipping disconnected socket:" << orderdClient.socket->socketDescriptor();
                continue;
            }

            // Пропускаем сам себя
            if (newId == key) {
                continue;
            }

            // Отправляем новому клиенту информацию о других сокетах

            MesageIdentifiers MesidToSentd = MesageIdentifiers::NONE;

            ClienDataBase clientDB;
            if(mesId == MesageIdentifiers::ID_CLIENT || mesId == MesageIdentifiers::CLIENT_READY_TO_WORCK){
                MesidToSentd = MesageIdentifiers::ID_CLIENT;
            }
            else if(mesId == MesageIdentifiers::RECONNECTION){
                MesidToSentd = MesageIdentifiers::RECONNECTION;
            }

            if(MesidToSentd == MesageIdentifiers::NONE){
                qWarning() << "Mess Packer ERROR";
            }

            Message messToSend1 = ObjectToSend(MesidToSentd,
                                               key,
                                               orderdClient.nick,
                                               true);


            SendToSocket(newId, messToSend1);

            // Отправляем другим сокетам информацию о новом клиенте


            Message messToSend2 = ObjectToSend(MesidToSentd,
                                               newId,
                                               ClientsData[newId].nick,
                                               true);

            SendToSocket(key, messToSend2);
        }


}

void Sending::GetDisconnectedClient(qintptr socket) {

    ClienDataBase clientDB;

    QString clientIDtoRemuve = clientDB.GetId(QString::number(socket));

    for (auto i = ClientsData.begin(); i != ClientsData.end(); i++) {

        QString key = i.key();
        server::ClientInfo orderdClient = i.value();

        if(key == clientIDtoRemuve){
            continue;
        }

        Message messToSend3 = ObjectToSend(MesageIdentifiers::ID_DELETE,
                                           clientIDtoRemuve,
                                           QString(),
                                           false);

        qDebug() <<"To delete" <<  clientIDtoRemuve;

     SendToSocket(key, messToSend3);
    }


    ClientsData.remove(clientIDtoRemuve);
}

void Sending::SendToSocket(QString id, Message &message) {

    if (!ClientsData.contains(id)) {
        qWarning() << "ID not found in ClientsData:" << id;
        return;
    }

    if (!ClientsData[id].socket) {
        qWarning() << "Socket is nullptr for ID:" << id;
        return;
    }

    if (ClientsData[id].socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Socket not connected:" << ClientsData[id].socket->socketDescriptor();
        return;
    }


    ClientsData[id].socket->write(Mpack::puck(message).data());

    ClientsData[id].socket->flush();


    // if (!socket->waitForBytesWritten(5000)) { // Timeout для предотвращения вечного ожидания
    //     qWarning() << "Error waiting for bytes to be written:"
    //                << socket->errorString();
    // }

}


void Sending::SendToSocketLogInData(QTcpSocket *socket, Message &message)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Socket not connected:" << socket->socketDescriptor();
        return;
    }

    socket->write(Mpack::puck(message).data());

    // socket->flush();

}


Message Sending::ObjectToSend(MesageIdentifiers MESID, QString ID, QString NICK, bool stat)
{
    Message messToSend;

    qInfo() << "ID: " << ID;
    qInfo() << "Message Type" << MesType(MESID);

    messToSend.id = MESID;
    messToSend.newOrDeleteClientInNet.id = ID.toStdString();
    messToSend.newOrDeleteClientInNet.nick = NICK.toStdString();
    messToSend.newOrDeleteClientInNet.stat = stat;


    return messToSend;
}


QString Sending::MesType(MesageIdentifiers id)
{
    if(id == MesageIdentifiers::MESAGE) return "MESSAGE";
    else if(id == MesageIdentifiers::ID_CLIENT) return "ID_CLIENT";
    else if(id == MesageIdentifiers::ID_DELETE) return "ID_DELETE";
    else if(id == MesageIdentifiers::CLIENT_READY_TO_WORCK) return "CLIENT_READY_TO_WORCK";
    else if(id == MesageIdentifiers::RECONNECTION) return "RECONNECTION";
    else return "";
}
