#include "qmutex.h"
#include <QDebug>
#include <QStringBuilder>
#include <QTimer>

#include "server.h"
#include "sending.h"
#include "enums.h"
#include "Config.hpp"
#include "cliendatabase.h"

#include "Mpack.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCoreApplication>

#include "Mpack.hpp"

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
        QString str = Mpack::unpack(socket->readAll());
        qInfo() << "Reading data..." << str;

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
            QString logInFlag = clientDB.LogIn(parts[1], parts[2]);

            if(logInFlag != "LOGIN_FAIL_NAME" && logInFlag != "LOGIN_FAIL_PASS"){
                QString message = QString("%1,%2")
                                      .arg(LOGIN_SEC)
                                      .arg(socket->socketDescriptor());

                qDebug() << "New desk: " << socket->socketDescriptor();

                emit sendingMesage(socket, message);
            }
            else if(logInFlag == "LOGIN_FAIL_NAME"){
                QString message = QString("%1")
                .arg(LOGIN_FAIL_NAME);

                qInfo() << "Log in fail NAME: " << socket->socketDescriptor();
                emit sendingMesage(socket, message);
            }
            else if(logInFlag == "LOGIN_FAIL_PASS"){
                QString message = QString("%1")
                .arg(LOGIN_FAIL_PASS);

                qDebug() << "Log in fail PASS : " << socket->socketDescriptor();
                emit sendingMesage(socket, message);
            }
        }
        else if(messageType == SIGN){
            ClienDataBase clientDB;
            QString sginUpFlag = clientDB.SingUp(parts[1], parts[2], socket);

            if(sginUpFlag == "SIGN_SEC"){
            QString message = QString("%1")
                                   .arg(SIGN_SEC);
                emit sendingMesage(socket, message);
            }
            else if(sginUpFlag == "SIGN_FAIL"){
                QString message = QString("%1")
                                    .arg(SIGN_FAIL);
                emit sendingMesage(socket, message);
            }
            else if(sginUpFlag == "SIGN_FAIL_EXIST"){
                QString message = QString("%1")
                .arg(SIGN_FAIL_EXIST);
                emit sendingMesage(socket, message);
            }
        }
        else if(messageType == CLIENT_READY_TO_WORCK){
            emit newClientConnected(socket, Sockets);
        }
    }
}
