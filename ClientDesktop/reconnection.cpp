#include "config.h"
#include "message.h"
#include "Mpack.hpp"
#include "userdata.h"
#include "regwindow.h"
#include "reconnection.h"
#include "mainwindow.h"


#include <qtimer.h>
#include <QThread>

extern UserData &userData ;
QTcpSocket* Reconnection::socket = nullptr;
bool Reconnection::Close_Window_stat = false;

Reconnection::Reconnection(QObject* parent ) :QObject(parent){

    createSocket();

    connect(socket, &QTcpSocket::connected, this, &Reconnection::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &Reconnection::onError);
    connect(socket, &QTcpSocket::readyRead, this, &Reconnection::slotReadyRed);
    connect(socket, &QTcpSocket::disconnected, this, &Reconnection::onDisconnected);


}

void Reconnection::connections()
{
    connect(Reconnection::getInterface(), &Reconnection::mainWindSocketPrint, mainWind, &MainWindow::Socket_print);
    connect(Reconnection::getInterface(), &Reconnection::mainWindSocketDelete, mainWind, &MainWindow::Socket_delete);
    connect(Reconnection::getInterface(), &Reconnection::mainWindPrintMessage, mainWind, &MainWindow::PrintMassage);

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
    connect(Reconnection::getInterface(), &Reconnection::regWindErrorWorker, regWindow, &RegWindow::logSingWorker);
    connect(Reconnection::getInterface(), &Reconnection::regWindConnectionStatWorker, regWindow, &RegWindow::connectStatWorker);
}

void Reconnection::setMainWind(MainWindow *mainWin)
{
    this->mainWind = mainWin;
    connections();
}

void Reconnection::onConnected() {
    qDebug() << "Connected to Server";

    if (!userData.mainWindStarted) {
        emit regWindConnectionStatWorker(ConnectionStat::CONNECTED);
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
        emit regWindConnectionStatWorker(ConnectionStat::DISCONNECTED);
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

void Reconnection::slotReadyRed()
{

    Message message = Mpack::unpack(socket->readAll().toStdString());

    switch (message.id) {
        case MesageIdentifiers::ID_CLIENT: // the_identifier
        {
            if (!userData.Sockets.contains(QString::fromStdString(message.newOrDeleteClientInNet.descriptor)) && userData.mainWindStarted){
                userData.Sockets.push_back(QString::fromStdString(message.newOrDeleteClientInNet.descriptor));
                emit mainWindSocketPrint();
            }
            break;
        }
        case MesageIdentifiers::ID_DELETE:
        {
            if (userData.Sockets.contains(QString::fromStdString(message.newOrDeleteClientInNet.descriptor)) && userData.mainWindStarted){
                userData.Sockets.removeAll(message.newOrDeleteClientInNet.descriptor);
                emit mainWindSocketDelete(QString::fromStdString(message.newOrDeleteClientInNet.descriptor));
            }
            break;
        }
        case MesageIdentifiers::MESAGE:
        {
            if (userData.mainWindStarted){
               emit mainWindPrintMessage(QString::fromStdString(message.messageData.message));
            }
            break;
        }
        case MesageIdentifiers::SIGN_FAIL:
        {
            if(!userData.mainWindStarted){
                emit regWindErrorWorker(MesageIdentifiers::SIGN_FAIL);
            }
            break;
        }
        case MesageIdentifiers::LOGIN_FAIL_PASS:
        {
            if(!userData.mainWindStarted){
                emit regWindErrorWorker(MesageIdentifiers::LOGIN_FAIL_PASS);
            }
            break;
        }
        case MesageIdentifiers::LOGIN_FAIL_NAME:
        {
            if(!userData.mainWindStarted){
                qDebug() << "Error name";
                emit regWindErrorWorker(MesageIdentifiers::LOGIN_FAIL_NAME);
            }
            break;
        }
        case MesageIdentifiers::SIGN_FAIL_EXIST:
        {
            if(!userData.mainWindStarted){
                emit regWindErrorWorker(MesageIdentifiers::SIGN_FAIL_EXIST);
            }
            break;
        }
        case MesageIdentifiers::LOGIN_SEC:
        {
            if(!userData.mainWindStarted){
                qDebug()<< "Got desk: " << message.registrationData.desckriptor;
                userData.desck = QString::fromStdString(message.registrationData.desckriptor);
                emit regWindow->CloseWindow();
            }
        }
        case MesageIdentifiers::SIGN_SEC:{}


        case MesageIdentifiers::LOG:{}
        case MesageIdentifiers::SIGN:{}
        case MesageIdentifiers::NONE:{}
        case MesageIdentifiers::ID_MY:{}
        case MesageIdentifiers::RECONNECTION:{}
        case MesageIdentifiers::CLIENT_READY_TO_WORCK:{}
            default:
        break;

    }
}




