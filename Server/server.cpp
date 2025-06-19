#include "qmutex.h"
#include <QDebug>
#include <QStringBuilder>
#include <QTimer>

#include "server.h"
#include "sending.h"
#include "enums.h"
#include "Config.hpp"
#include "cliendatabase.h"
#include"message.h"

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

    }
}

void server::slotsReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {

        qInfo() << "Reading data...";
        Message message = Mpack::unpack(socket->readAll().toStdString());

        if (message.id == MesageIdentifiers::MESAGE) {
            QTcpSocket *RESIVER = nullptr; // Указатель на сокет получателя
            QTcpSocket *SENDER = nullptr; // Указатель на сокет отправителя

            for (int i = 0; i < Sockets.size(); ++i) {
                if (Sockets[i]->socketDescriptor() == std::stoll(message.messageData.resivDesk)){
                    RESIVER = Sockets[i];
                }
                if (Sockets[i]->socketDescriptor() == std::stoll(message.messageData.senderDesk)){
                    SENDER = Sockets[i];
                }
            }

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);      

            Message messToSend;
            messToSend.id = MesageIdentifiers::MESAGE;
            messToSend.messageData.senderDesk = QString::number(SENDER->socketDescriptor()).toStdString();
            messToSend.messageData.message = message.messageData.message;

            emit sendingMesage(RESIVER, messToSend);
        }
        else if(message.id == MesageIdentifiers::LOG){

            ClienDataBase clientDB;
            MesageIdentifiers logInFlag = clientDB.LogIn(QString::fromStdString(message.registrationData.nickName),
                                               QString::fromStdString(message.registrationData.pass));

            if(logInFlag != MesageIdentifiers::LOGIN_FAIL_NAME && logInFlag != MesageIdentifiers::LOGIN_FAIL_PASS){

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_SEC;
                messToSend.registrationData.desckriptor = QString::number(socket->socketDescriptor()).toStdString();

                qDebug() << "New desk: " << socket->socketDescriptor();

                emit sendingMesage(socket, messToSend);
            }
            else if(logInFlag == MesageIdentifiers::LOGIN_FAIL_NAME){

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_FAIL_NAME;

                qInfo() << "Log in fail NAME: " << message.registrationData.nickName;
                emit sendingMesage(socket, messToSend);
            }
            else if(logInFlag == MesageIdentifiers::LOGIN_FAIL_PASS){

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_FAIL_PASS;

                qDebug() << "Log in fail PASS : " << socket->socketDescriptor();
                emit sendingMesage(socket, messToSend);
            }
        }
        else if(message.id == MesageIdentifiers::SIGN){
            ClienDataBase clientDB;
            MesageIdentifiers sginUpFlag = clientDB.SingUp(QString::fromStdString(message.registrationData.nickName),
                                                           QString::fromStdString(message.registrationData.pass),
                                                           static_cast<int>(socket->socketDescriptor()));

            if(sginUpFlag == MesageIdentifiers::SIGN_SEC){

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_SEC;

                emit sendingMesage(socket, messToSend);
            }
            else if(sginUpFlag == MesageIdentifiers::SIGN_FAIL){

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_FAIL;

                emit sendingMesage(socket, messToSend);
            }
            else if(sginUpFlag == MesageIdentifiers::SIGN_FAIL_EXIST){

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_FAIL_EXIST;
                emit sendingMesage(socket, messToSend);
            }
        }
        else if(message.id == MesageIdentifiers::CLIENT_READY_TO_WORCK){
            qDebug() << socket->socketDescriptor() << ": CLIENT_READY_TO_WORCK";
            ClienDataBase clientDB;

            QString nick = clientDB.GetNick(QString::number(socket->socketDescriptor()).toStdString());

            qDebug() << socket->socketDescriptor() << "nick:" << nick;

            emit newClientConnected(socket, Sockets, nick);
        }
        else if(message.id == MesageIdentifiers::RECONNECTION){
            qDebug() << "Reconnection: " << message.reconnect.desck;
        }
    }
}
