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
#include"message.h"


#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCoreApplication>
#include <QDir>

#include <msgpack.hpp>


QList<QTcpSocket *> server::Sockets;


QMutex server::mutex;

server::server(const Config::Settings& aSettings) {

    qInstallMessageHandler(Loger::myLogMessageHandler);
    // Install custom message pattern
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss,zzz} [%{type}] [%{line}] [%{file}]: %{message}");

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
        Message message = m_pack.unpack(socket->readAll().toStdString());


        if (message.id == MesageIdentifiers::MESAGE) {
            QTcpSocket *RESIVER = nullptr; // Указатель на сокет получателя
            QTcpSocket *SENDER = nullptr; // Указатель на сокет отправителя
            // QString TEXT = parts[3];

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
            messToSend.messageData.senderDesk = SENDER->socketDescriptor();

            emit sendingMesage(RESIVER, message);
        }
        else if(message.id == MesageIdentifiers::LOG){

            ClienDataBase clientDB;
            MesageIdentifiers logInFlag = clientDB.LogIn(QString::fromStdString(message.registrationData.nickName),
                                               QString::fromStdString(message.registrationData.pass));

            if(logInFlag != MesageIdentifiers::LOGIN_FAIL_NAME && logInFlag != MesageIdentifiers::LOGIN_FAIL_PASS){
                // QString message = QString("%1,%2")
                //                       .arg(LOGIN_SEC)
                //                       .arg(socket->socketDescriptor());

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_SEC;
                messToSend.registrationData.desckriptor = QString::number(socket->socketDescriptor()).toStdString();

                qDebug() << "New desk: " << socket->socketDescriptor();

                emit sendingMesage(socket, messToSend);
            }
            else if(logInFlag == MesageIdentifiers::LOGIN_FAIL_NAME){
                // QString message = QString("%1")
                // .arg(LOGIN_FAIL_NAME);

                Message messToSend;
                messToSend.id = MesageIdentifiers::LOGIN_FAIL_NAME;

                qInfo() << "Log in fail NAME: " << message.registrationData.nickName;
                emit sendingMesage(socket, messToSend);
            }
            else if(logInFlag == MesageIdentifiers::LOGIN_FAIL_PASS){
                // QString message = QString("%1")
                // .arg(LOGIN_FAIL_PASS);

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
                                                           socket->socketDescriptor());

            if(sginUpFlag == MesageIdentifiers::SIGN_SEC){
            // QString message = QString("%1")
            //                        .arg(SIGN_SEC);

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_SEC;

                emit sendingMesage(socket, messToSend);
            }
            else if(sginUpFlag == MesageIdentifiers::SIGN_FAIL){
                // QString message = QString("%1")
                //                     .arg(SIGN_FAIL);

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_FAIL;

                emit sendingMesage(socket, messToSend);
            }
            else if(sginUpFlag == MesageIdentifiers::SIGN_FAIL_EXIST){
                // QString message = QString("%1")
                // .arg(SIGN_FAIL_EXIST);

                Message messToSend;
                messToSend.id = MesageIdentifiers::SIGN_FAIL_EXIST;
                emit sendingMesage(socket, messToSend);
            }
        }
        else if(message.id== MesageIdentifiers::CLIENT_READY_TO_WORCK){
            emit newClientConnected(socket, Sockets);
        }
    }
}

void Loger::myLogMessageHandler(const QtMsgType type, const QMessageLogContext& context, const QString& msg){
     QString formattedMsg = qFormatLogMessage(type, context, msg) + "\n";

     // Записываем в файл
     QFile logFile("app.log");
     if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
     {
         logFile.write(qUtf8Printable(formattedMsg));
     }

     // Выводим в консоль
     fprintf(stderr, "%s", qUtf8Printable(formattedMsg));
     fflush(stderr);
}




