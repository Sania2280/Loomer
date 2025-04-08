// reconnection.cpp
#include "reconnection.h"
#include "regwindow.h"
#include "ui_regwindow.h"
#include "config.h"
#include "userdata.h"
#include <qtimer.h>

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
    Reconnection::Close_Window_stat = false;
}

void Reconnection::onConnected() {
    qDebug() << "Connected to Server";

    regWindow->ui->listWidget_errors->clear();
    regWindow->ui->listWidget_errors->addItem("Connected to Server");
    QTimer::singleShot(3000, regWindow->ui->listWidget_errors, &QListWidget::clear);
}

void Reconnection::onError() {
    if(Close_Window_stat)return;

    qWarning() << "Error connect to Server:" << socket->errorString();
    // Запускаем повторное подключение через 3 секунды:
    QTimer::singleShot(3000, this, &Reconnection::setConnection);
}

void Reconnection::onDisconnected() {
    if(Close_Window_stat)return;

    qWarning() << "Disconnected from Server. Reconnecting...";
    QTimer::singleShot(3000, this, [this](){
        Reconnection::setConnection();

        if(regWindow != nullptr){
            regWindow->ui->listWidget_errors->clear();
            regWindow->ui->listWidget_errors->addItem("ERROR: lost connection");
        }
    });

}

void Reconnection::setConnection()
{
    Config config;
    config.Read();
    qDebug() << "Trying to connect to" << Config::settings.server_ip << "on port" << Config::settings.server_port;
    socket->abort();
    socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);

}

