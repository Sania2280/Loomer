#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
// #include <windows.h>
#include <msgpack.hpp>

#include "sending.h"
#include "enums.h"

#include <msgpack.hpp>

QMutex mutex1;

Sending::Sending(server *srv, QObject *parent)
    : QThread(parent), m_server(srv) {}


void Sending::Get_New_Client(QTcpSocket *socket, QList<QTcpSocket *> Sockets_reciverd) {

    Sockets = Sockets_reciverd;

    // Отправляем идентификатор новому клиенту
    QString MyIdentifier = String_to_Send(QString::number(ID_MY)
                                          ,socket->peerAddress().toString()
                                          ,QString::number(socket->socketDescriptor()));

    sendToSocket(socket, MyIdentifier);


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
        QString identifier1 = String_to_Send(QString::number(ID_CLIENT)
                                             ,otherSocket->peerAddress().toString()
                                             ,QString::number(otherSocket->socketDescriptor()));

        sendToSocket(socket, identifier1);

        // Отправляем другим сокетам информацию о новом клиенте
        QString identifier2 = String_to_Send(QString::number(ID_CLIENT)
                                             ,socket->peerAddress().toString()
                                             ,QString::number(socket->socketDescriptor()));

        sendToSocket(otherSocket, identifier2);
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

        QString identifier3 = String_to_Send(QString::number(ID_DELETE), IP
                                             ,QString::number(static_cast<qint64>(socket)));

        sendToSocket(Sockets[i], identifier3);
    }
    mutex1.unlock();
}

void Sending::sendToSocket(QTcpSocket *socket, const QString &message) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qWarning() << "Socket not connected:" << socket->socketDescriptor();
        return;
    }


    std::string msg = message.toStdString();

    msgpack::sbuffer buffer;
    msgpack::pack(buffer, msg);

    socket->write(buffer.data(), buffer.size());

    qInfo()<<"mesage" << message;

    if (!socket->waitForBytesWritten(5000)) { // Timeout для предотвращения вечного ожидания
        qWarning() << "Error waiting for bytes to be written:"
                   << socket->errorString();
    }


}


QString Sending::String_to_Send(QString ID, QString IP, QString DESCK)
{
    QString mesage = QString("%1,%2,%3")
    .arg(ID)
        .arg(IP)
        .arg(DESCK);
    return mesage;
}

