#include "sending.h"
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <windows.h>

QMutex mutex1;

Sending::Sending(server *srv, QObject *parent)
    : QThread(parent), m_server(srv) {}

void Sending::Get_New_Client(QTcpSocket *socket, QList<QTcpSocket *> Sockets_reciverd) {

    Sockets = Sockets_reciverd;

    // Отправляем идентификатор новому клиенту
    QString MyIdentifier = QString("02,%1,%2") // my_identifier
                               .arg(socket->peerAddress().toString())
                               .arg(QString::number(socket->socketDescriptor()));
    sendToSocket(socket, MyIdentifier);


    for (int i = 0; i < Sockets.size(); ++i) {
        QTcpSocket *otherSocket = Sockets[i];

        // Пропускаем отключенные сокеты
        if (otherSocket->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Skipping disconnected socket:" << otherSocket;
            continue;
        }

        // Пропускаем сам себя
        if (socket->socketDescriptor() == otherSocket->socketDescriptor()) {
            continue;
        }

        // Отправляем новому клиенту информацию о других сокетах
        QString identifier1 = QString("03,%1,%2") // the_identifier
                                  .arg(otherSocket->peerAddress().toString())
                                  .arg(QString::number(otherSocket->socketDescriptor()));
        sendToSocket(socket, identifier1);

        // Отправляем другим сокетам информацию о новом клиенте
        QString identifier2 = QString("03,%1,%2") // the_identifier
                                  .arg(socket->peerAddress().toString())
                                  .arg(QString::number(socket->socketDescriptor()));
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

        QString identifier3 = QString("04,%1,%2")
                                  .arg(IP)
                                  .arg(socket);
        sendToSocket(Sockets[i], identifier3);
    }
    mutex1.unlock();
}

void Sending::sendToSocket(QTcpSocket *socket, const QString &message) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Socket not connected:" << socket->socketDescriptor();
        return;
    }

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << message;

    socket->write(data);

    qDebug()<<"mesage" << message;

    if (!socket->waitForBytesWritten(5000)) { // Timeout для предотвращения вечного ожидания
        qDebug() << "Error waiting for bytes to be written:"
                 << socket->errorString();
    }

    QThread::msleep(130);
}
