#include "ServerConnector.h"
#include "RegWindow.h"
#include "UserData.h"
#include "config.h"

#include <QTcpSocket>
#include <winsock.h>
#include <QTimer>
#include "Mpack.hpp"


extern UserData &userData ;

ServerConnector::ServerConnector(QObject *parent, RegWindow *rWindow) :
    QObject(parent),
    socket(new QTcpSocket(this)),
    regWind(rWindow) {}

void ServerConnector::ConnectToServer()
{
    Config config;
    config.Read();

    if (!socket) { // Создаём сокет, если его нет
        socket = new QTcpSocket(this);
    }

    socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);
    userData.mainWindStarted = false;

    if (!userData.getSocket()) {  // Проверка перед установкой
        userData.setSocket(socket);
        qDebug() << "Socket made:" << userData.getSocket() << "desk" << socket->socketDescriptor();
    } else {
        qWarning() << "Socet is already exist";
    }

    if (!socket->waitForConnected(5000)) {
        qWarning() << "Не удалось подключиться к серверу:" << socket->errorString();
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &ServerConnector::slotReadyRead);

}

void ServerConnector::SendMyData(int status)
{
    QString message = QString("%1,%2,%3")
        .arg(status)
        .arg(userData.name)
        .arg(userData.pass);

        socket->write(QByteArray::fromStdString(Mpack::puck(message)));

    if (!socket->waitForBytesWritten(5000)) {
        qWarning() << "Sending data error:" << socket->errorString();
    }
}

void ServerConnector::slotReadyRead()
{

    UserData& userdata = UserData::getInstance();

    if(userdata.mainWindStarted != true){
        QString str = Mpack::unpack(socket->readAll());
        QStringList parts = str.split(",");

        qDebug() << str;

        int messType = parts[0].toInt();
        if (messType == LOGIN_SEC) {

            qDebug()<< "Got desk: " << parts[1];

            userdata.desck = parts[1];

            emit regWind->CloseWindow();
        }

    }
}
