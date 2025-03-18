#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <windows.h>

#include "sending.h"
#include "enums.h"
#include "m_pack.h"

#include <msgpack.hpp>

QMutex mutex1;

Sending::Sending(server *srv, QObject *parent)
    : QThread(parent), m_server(srv) {}


void Sending::Get_New_Client(QTcpSocket *socket, QList<QTcpSocket *> Sockets_reciverd) {

    Sockets = Sockets_reciverd;

    // Отправляем идентификатор новому клиенту
    // QString MyIdentifier = String_to_Send(QString::number(ID_MY)
    //                                       ,socket->peerAddress().toString()
    //                                       ,QString::number(socket->socketDescriptor()));

    // sendToSocket(socket, MyIdentifier);

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

        // QString identifier1 = String_to_Send(QString::number(ID_CLIENT)
        //                                      ,otherSocket->peerAddress().toString()
        //                                      ,QString::number(otherSocket->socketDescriptor()));

        Message messToSend1 = ObjectToSend(MesageIdentifiers::ID_CLIENT,
                                           otherSocket->peerAddress().toString(),
                                           QString::number(otherSocket->socketDescriptor()));


        // messToSend1.id = MesageIdentifiers::ID_CLIENT;
        // messToSend1.newOrDeleteClientInNet.ip = otherSocket->peerAddress().toString().toStdString();
        // messToSend1.newOrDeleteClientInNet.descriptor = otherSocket->socketDescriptor();

        sendToSocket(socket, messToSend1);

        // Отправляем другим сокетам информацию о новом клиенте

         // QString identifier2 = String_to_Send(QString::number(ID_CLIENT)
         //                                     ,socket->peerAddress().toString()
         //                                     ,QString::number(socket->socketDescriptor()));

        Message messToSend2 = ObjectToSend(MesageIdentifiers::ID_CLIENT,
                                           socket->peerAddress().toString(),
                                           QString::number(socket->socketDescriptor()));


        // messToSend1.id = MesageIdentifiers::ID_CLIENT;
        // messToSend1.newOrDeleteClientInNet.ip = socket->peerAddress().toString().toStdString();
        // messToSend1.newOrDeleteClientInNet.descriptor = socket->socketDescriptor();

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

        // QString identifier3 = String_to_Send(QString::number(ID_DELETE), IP
        //                                      ,QString::number(static_cast<qint64>(socket)));

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

    M_pack m_pack;

    qDebug() << message.registrationData.desckriptor;

    socket->write(m_pack.puck(message).data());
    socket->flush();


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

    return messToSend;
}


