#include <QDebug>
#include <QStringBuilder>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>

#include "server.h"
#include "sending.h"
#include "enums.h"
#include "Config.hpp"
#include "m_pack.h"
#include "qmutex.h"



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

        emit newClientConnected(socket, Sockets);
    }
}

void server::slotsReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        M_pack m_pack;
        qInfo() << "Reading data...";
        QString str = m_pack.unpack(socket->readAll());

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


 QString M_pack::unpack(QByteArray rawData) {
     msgpack::object_handle oh = msgpack::unpack(rawData.constData(), rawData.size());
     msgpack::object obj = oh.get();
     QString data = QString::fromStdString(obj.as<std::string>());
     return data;
 }

 std::string M_pack::puck(QString rawData){
     std::string msg = rawData.toStdString();
     msgpack::sbuffer buffer;
     msgpack::pack(buffer, msg);
    return std::string(buffer.data(), buffer.size());
 }

