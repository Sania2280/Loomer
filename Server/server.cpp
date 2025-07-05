#include "qmutex.h"
#include <QDebug>
#include <QStringBuilder>
#include <QTimer>

#include "server.h"
#include "sending.h"
#include "enums.h"
#include "cliendatabase.h"
#include"message.h"

#include "Mpack.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCoreApplication>

#include "Mpack.hpp"


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
    connect(this, &server::sendingClientData, sendingPtr, &Sending::MessagePacker);
    connect(this, &server::disconnectedClient, sendingPtr, &Sending::GetDisconnectedClient);
    connect(this, &server::sendingMesage, sendingPtr, &Sending::SendToSocket);
    connect(this, &server::sendingLogInData, sendingPtr, &Sending::SendToSocketLogInData);
}

void server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *socket = new QTcpSocket(this);
    if (socket->setSocketDescriptor(socketDescriptor)) {
        QString clientIP = socket->peerAddress().toString();
        qInfo() << "Client connected from IP:" << clientIP
                 << "with deck:" << socketDescriptor;


        QString IP = socket->peerAddress().toString();

        connect(socket, &QTcpSocket::readyRead, this, &server::slotsReadyRead);
        connect(socket, &QTcpSocket::disconnected, this,
                [this, socketDescriptor, IP]() {
            qInfo() << "disconnected dest form server" << socketDescriptor;
            emit disconnectedClient(socketDescriptor);
        });

    }
}

void server::slotsReadyRead() {
    QTcpSocket *  socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {

        qInfo() << "Reading data...";
        ClienDataBase clientDB;
        Message message = Mpack::unpack(socket->readAll().toStdString());
        QString id = clientDB.GetId(QString::number(socket->socketDescriptor()));

        if (message.id == MesageIdentifiers::MESAGE) {
            QString RESIVER = nullptr; // Указатель на сокет получателя
            QString SENDER = nullptr; // Указатель на сокет отправителя

            for (auto  i = ClientsData.begin(); i != ClientsData.end(); i++) {
                if (i.key() == message.messageData.resivId){
                    RESIVER = i.key();
                }
                if (i.key() == message.messageData.senderId){
                    SENDER = i.key();
                }
            }

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);

            Message messToSend;
            messToSend.id = MesageIdentifiers::MESAGE;
            messToSend.messageData.senderId = SENDER.toStdString();
            messToSend.messageData.message = message.messageData.message;

            emit sendingMesage(RESIVER, messToSend);
        }
        else if(message.id == MesageIdentifiers::LOG){

            ClienDataBase::LogInStruct logInStruct = ClienDataBase::LogInStruct();
            logInStruct = clientDB.LogIn(QString::fromStdString(message.registrationData.nick),
                                        QString::fromStdString(message.registrationData.pass));


            if(logInStruct.mesID != MesageIdentifiers::LOGIN_FAIL_NAME && logInStruct.mesID != MesageIdentifiers::LOGIN_FAIL_PASS){

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_SEC;
                // messToSend.registrationData.desckriptor = QString::number(socket->socketDescriptor()).toStdString();
                messToSend.registrationData.id = logInStruct.clientID.toStdString();

                emit sendingLogInData(socket, messToSend);
            }
            else if(logInStruct.mesID == MesageIdentifiers::LOGIN_FAIL_NAME){

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_FAIL_NAME;

                qInfo() << "Log in fail NAME: " << message.registrationData.nick;

                emit sendingLogInData(socket, messToSend);
            }
            else if(logInStruct.mesID == MesageIdentifiers::LOGIN_FAIL_PASS){

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_FAIL_PASS;

                qInfo() << "Log in fail PASS : " << socket->socketDescriptor();

                emit sendingLogInData(socket, messToSend);
            }

        }
        else if(message.id == MesageIdentifiers::SIGN){
            MesageIdentifiers sginUpFlag = clientDB.SingUp(QString::fromStdString(message.registrationData.nick),
                                                           QString::fromStdString(message.registrationData.pass),
                                                           static_cast<int>(socket->socketDescriptor()));

            if(sginUpFlag == MesageIdentifiers::SIGN_SEC){

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_SEC;

                emit sendingLogInData(socket, messToSend);
            }
            else if(sginUpFlag == MesageIdentifiers::SIGN_FAIL){

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_FAIL;

                emit sendingLogInData(socket, messToSend);
            }
            else if(sginUpFlag == MesageIdentifiers::SIGN_FAIL_EXIST){

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_FAIL_EXIST;

                emit sendingLogInData(socket, messToSend);
            }
        }
        else if(message.id == MesageIdentifiers::CLIENT_READY_TO_WORCK){

            QString nick = clientDB.GetNick(message.dbID);
            clientDB.RewriteDesk(message.dbID, QString::number(socket->socketDescriptor()));

            ClientsData[QString::fromStdString(message.dbID)].socket = socket;
            ClientsData[QString::fromStdString(message.dbID)].nick = nick;

            emit sendingClientData(MesageIdentifiers::CLIENT_READY_TO_WORCK, QString::fromStdString(message.dbID), ClientsData);
        }
        else if(message.id == MesageIdentifiers::RECONNECTION){
            qDebug() << "Reconnection: " << message.reconnect.desck;

            if (!ClientsData.contains(QString::fromStdString(message.reconnect.id))){
                ClientsData[QString::fromStdString(message.reconnect.id)].nick = clientDB.GetNick(message.reconnect.id);
                ClientsData[QString::fromStdString(message.reconnect.id)].socket = socket;

                clientDB.RewriteDesk(message.reconnect.id, QString::number(socket->socketDescriptor()));

                Message messageToSend;
                messageToSend.reconnect.nick = ClientsData[QString::fromStdString(message.reconnect.id)].nick.toStdString();
                messageToSend.reconnect.id = message.reconnect.id;
                emit sendingClientData(MesageIdentifiers::RECONNECTION, QString::fromStdString(message.reconnect.id), ClientsData);

            }
            else{
                qWarning() << "User Exist";
            }
        }
    }
}



