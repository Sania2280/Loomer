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

    if (!userData.mainWindStarted) {
        regWindow->ui->listWidget_errors->clear();
        regWindow->ui->listWidget_errors->addItem("Connected to Server");
        QTimer::singleShot(3000, regWindow->ui->listWidget_errors, &QListWidget::clear);
    } else {
        Message message;
        message.id = MesageIdentifiers::RECONNECTION;
        message.reconnect.desck = userData.desck.toStdString();

        qDebug() << "Sending old data";
        socket->write(QByteArray::fromStdString(Mpack::puck(message)));
    }
}

void Reconnection::onError() {

    qWarning() << "Error connect to Server:" << socket->errorString();

    Reconnection::tryReconnect();
}

void Reconnection::onDisconnected() {
    qWarning() << "Disconnected from Server. Reconnecting...";

    if (!userData.mainWindStarted && regWindow) {
        regWindow->ui->listWidget_errors->clear();
        regWindow->ui->listWidget_errors->addItem("ERROR: lost connection");
    }

    QTimer::singleShot(3000, this, &Reconnection::tryReconnect);
}

void Reconnection::tryReconnect() {
    if (socket->state() == QAbstractSocket::ConnectedState || socket->state() == QAbstractSocket::ConnectingState) {
        // Уже подключен или в процессе подключения — ничего не делаем
        return;
    }

    qDebug() << "Attempting to reconnect...";
    setConnection();

    // Пробуем снова через 3 секунды, если не подключено
    QTimer::singleShot(3000, this, &Reconnection::tryReconnect);
}

void Reconnection::setConnection() {
    Config config;
    config.Read();

    qDebug() << "Trying to connect to" << Config::settings.server_ip << "on port" << Config::settings.server_port;

    socket->abort();  // Прерываем текущее соединение, если было
    socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);
}

