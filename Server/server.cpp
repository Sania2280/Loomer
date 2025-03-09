#include "qmutex.h"
#include <QDebug>
#include <QStringBuilder>
#include <QTimer>

#include "server.h"
#include "sending.h"
#include "enums.h"
#include "Config.hpp"
#include "m_pack.h"
#include "cliendatabase.h"


#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCoreApplication>

#include <msgpack.hpp>


QList<QTcpSocket *> server::Sockets;


QMutex server::mutex;

server::server(const Config::Settings& aSettings) {
    if (this->listen(aSettings.server_channel, aSettings.server_port)) {
        qDebug() << "Server started on port 2323";
    } else {
        qDebug() << "Error starting server";
    }

}

void server::setSending(Sending &sending) {
    sendingPtr = &sending;
    connect(this, &server::newClientConnected, sendingPtr, &Sending::Get_New_Client);
    connect(this, &server::disconnectedClient, sendingPtr, &Sending::Get_Disconnected_Client);
    connect(this, &server::sendingMesage, sendingPtr, &Sending::sendToSocket);
}

void server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *socket = new QTcpSocket(this);
    if (socket->setSocketDescriptor(socketDescriptor)) {
        QString clientIP = socket->peerAddress().toString();
        qInfo() << "Client connected from IP:" << clientIP
                 << "with deck:" << socketDescriptor;

        Sockets.push_back(socket);

        QString IP = socket->peerAddress().toString();

        connect(socket, &QTcpSocket::readyRead, this, &server::slotsReadyRead);
        connect(socket, &QTcpSocket::disconnected, this,
                [this, socketDescriptor, IP]() {
            qInfo() << "disconnected dest form server" << socketDescriptor;
            emit disconnectedClient(socketDescriptor, IP);
        });

        // emit newClientConnected(socket, Sockets);
    }
}

void server::slotsReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        M_pack m_pack;
        qInfo() << "Reading data...";
        QString str = m_pack.unpack(socket->readAll());
        qInfo() << str;

        QString Identifier = str.left(1);
        QStringList parts = str.split(",");
        int messageType = parts[0].toInt();

        if (messageType == MESAGE) {
            QTcpSocket *RESIVER = nullptr; // Указатель на сокет получателя
            QTcpSocket *SENDER = nullptr; // Указатель на сокет отправителя
            QString TEXT = parts[3];

            for (int i = 0; i < Sockets.size(); ++i) {
                if (Sockets[i]->socketDescriptor() == parts[1].toLongLong())
                    RESIVER = Sockets[i];
                if (Sockets[i]->socketDescriptor() == parts[2].toLongLong())
                    SENDER = Sockets[i];
            }

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            QString message = QString("%1,%2,%3,%4")
                                  .arg(MESAGE)
                                  .arg(QString::number(RESIVER->socketDescriptor()))
                                  .arg(QString::number(SENDER->socketDescriptor()))
                                  .arg(TEXT);
            out << message;
            emit sendingMesage(RESIVER, message);
        }
        else if(messageType == LOG){

            ClienDataBase clientDB;
            QString desckriptor = clientDB.LogIn(parts[1], parts[2]);

            QString message = QString("%1,%2")
                                  .arg(LOGIN_SEC)
                                  .arg(socket->socketDescriptor());


            qDebug() << "New desk: " << socket->socketDescriptor();

            emit sendingMesage(socket, message);
        }
        else if(messageType == SIGN){

            ClienDataBase clientDB;
            bool desckriptor = clientDB.SingUp(parts[1], parts[2], socket);

            QString message = QString("%1,%2")
                                  .arg(SIGN_SEC)
                                  .arg(desckriptor);
        }
        else if(messageType == CLIENT_READY_TO_WORCK){
            emit newClientConnected(socket, Sockets);
        }
    }
}
