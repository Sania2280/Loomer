#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <windows.h>

#include "sending.h"
#include "enums.h"

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


void Sending::Get_New_Client(QTcpSocket *socket, QList<QTcpSocket *> Sockets_reciverd) {

    Sockets = Sockets_reciverd;

    QThread::msleep(100);

    for (int i = 0; i < Sockets.size(); ++i) {
        QTcpSocket *otherSocket = Sockets[i];

        // Пропускаем отключенные сокеты
        if (otherSocket->state() != QAbstractSocket::ConnectedState) {
            qWarning() << "Skipping disconnected socket:" << otherSocket;
            continue;
        }

        // Пропускаем сам себя
        if (socket->socketDescriptor() == otherSocket->socketDescriptor()) {
            continue;
        }

        // Отправляем новому клиенту информацию о других сокетах

        Message messToSend1 = ObjectToSend(MesageIdentifiers::ID_CLIENT,
                                           otherSocket->peerAddress().toString(),
                                           QString::number(otherSocket->socketDescriptor()));


        sendToSocket(socket, messToSend1);

        // Отправляем другим сокетам информацию о новом клиенте


        Message messToSend2 = ObjectToSend(MesageIdentifiers::ID_CLIENT,
                                           socket->peerAddress().toString(),
                                           QString::number(socket->socketDescriptor()));

        sendToSocket(otherSocket, messToSend2);
    }

}

void Sending::Get_Disconnected_Client(qintptr socket, QString IP) {

    mutex1.lock();
    for (int i = 0; i < Sockets.size(); i++) {
        if (Sockets[i]->state() != QAbstractSocket::ConnectedState) {
            Sockets.removeAt(i);
        }
    }

    for (int i = 0; i < Sockets.size(); i++) {

        Message messToSend3 = ObjectToSend(MesageIdentifiers::ID_DELETE, IP, QString::number(socket));

        sendToSocket(Sockets[i], messToSend3);
    }
    mutex1.unlock();
}

void Sending::sendToSocket(QTcpSocket *socket, Message message) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Socket not connected:" << socket->socketDescriptor();
        return;
    }


    qDebug() << message.registrationData.desckriptor;

    if(message.id == MesageIdentifiers::MESAGE){
        qDebug() << "MESAGE";
    }
    socket->write(Mpack::puck(message).data());

    socket->flush();

    qDebug() << "Mess size: " << sizeof(message);



    // if (!socket->waitForBytesWritten(5000)) { // Timeout для предотвращения вечного ожидания
    //     qWarning() << "Error waiting for bytes to be written:"
    //                << socket->errorString();
    // }

}


Message Sending::ObjectToSend(MesageIdentifiers ID, QString IP, QString DESCK)
{
    Message messToSend;

    messToSend.id = ID;
    messToSend.newOrDeleteClientInNet.ip = IP.toStdString();
    messToSend.newOrDeleteClientInNet.descriptor = DESCK.toStdString();


    qDebug() << "IP: " << messToSend.newOrDeleteClientInNet.ip;
    qDebug() << "DESCK: " << messToSend.newOrDeleteClientInNet.descriptor;

    return messToSend;
}


