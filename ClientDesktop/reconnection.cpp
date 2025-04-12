// reconnection.cpp
#include "config.h"
#include "message.h"
#include "Mpack.hpp"
#include "userdata.h"
#include "regwindow.h"
#include "ui_regwindow.h"
#include "reconnection.h"

#include <qtimer.h>
#include <QThread>

extern UserData &userData ;
QTcpSocket* Reconnection::socket = nullptr;
bool Reconnection::Close_Window_stat = false;

Reconnection::Reconnection(QObject* parent ) :QObject(parent){

    createSocket();

    connect(socket, &QTcpSocket::connected, this, &Reconnection::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &Reconnection::onError);
    connect(socket, &QTcpSocket::disconnected, this, &Reconnection::onDisconnected);
}

void Reconnection::createSocket()
{
    if (!socket) { // Создаём сокет, если его нет
        socket = new QTcpSocket(this);
    }


    if (!userData.getSocket()) {  // Проверка перед установкой
        userData.setSocket(socket);
        qDebug() << "Socket made:" << userData.getSocket() << "desk" << socket->socketDescriptor();
    } else {
        qWarning() << "Socet is already exist (Recon)";
    }

    setConnection();
}

QTcpSocket *Reconnection::getSocket()
{
    return socket;
}

void Reconnection::setRegwind(RegWindow *regWind)
{
    this->regWindow = regWind;
    // Reconnection::regWindExe = true;
    // Reconnection::mainWindExe = false;
}

void Reconnection::onConnected() {
    qDebug() << "Connected to Server";

    if(!userData.mainWindStarted){
        regWindow->ui->listWidget_errors->clear();
        regWindow->ui->listWidget_errors->addItem("Connected to Server");
        QTimer::singleShot(3000, regWindow->ui->listWidget_errors, &QListWidget::clear);
    }
    else{
        Message message;

        message.id = MesageIdentifiers::RECONNECTION;
        message.reconnect.desck = userData.desck.toStdString();

        qDebug() << "sending old data";
        socket->write(QByteArray::fromStdString(Mpack::puck(message)));
    }
}

void Reconnection::onError() {
    if(userData.mainWindStarted){

        qWarning() << "Error connect to Server:" << socket->errorString();
        // QTimer::singleShot(3000, this, &Reconnection::setConnection);
    }
    else{
        qWarning() << "Error connect to Server:" << socket->errorString();
        // // Запускаем повторное подключение через 3 секунды:
        QTimer::singleShot(3000, this, &Reconnection::onDisconnected);
    }
}

void Reconnection::onDisconnected() {
    if(userData.mainWindStarted){
        qWarning() << "Disconnected from Server. Reconnecting...";
        QTimer::singleShot(3000, this, &Reconnection::setConnection);
    }
    else{
        qWarning() << "Disconnected from Server. Reconnecting...";
        QTimer::singleShot(3000, this, [this](){
            Reconnection::setConnection();

            if(regWindow != nullptr){
                regWindow->ui->listWidget_errors->clear();
                regWindow->ui->listWidget_errors->addItem("ERROR: lost connection");
            }
        });
    }

}

void Reconnection::setConnection()
{
    Config config;
    config.Read();
    qDebug() << "Trying to connect to" << Config::settings.server_ip << "on port" << Config::settings.server_port;
    socket->abort();
    socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);
}

