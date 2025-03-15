#include "ServerConnector.h"
#include "regwindow.h"
#include "UserData.h"
#include "config.h"
#include "m_pack.h"
#include "ui_RegWindow.h"

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

        switch (messType) {
            case LOGIN_SEC:{
                qDebug()<< "Got desk: " << parts[1];
                userdata.desck = parts[1];
                emit regWind->CloseWindow();
                break;
                }
            case LOGIN_FAIL_NAME:{
                qDebug() << "Log in faill NAME";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: Incorrect Name");
                break;
            }
            case LOGIN_FAIL_PASS:{
                qDebug() << " Log in faill PASS";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: Incorrect Password");
                break;
            }
            case SIGN_SEC:{
                qDebug() << "Accaaunt created";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("Accaunt created succsed");
            }
            case SIGN_FAIL:{
                qDebug() << "Created accaunt ERROR";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: Accaunt creation Faild");
            }
            case SIGN_FAIL_EXIST:{
                qDebug() << "User Exist ERROR";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: User alredy exist");
            }

        }

    }
}


void ServerConnector::SetUpConnection(){
    // connect(socket, &QTcpSocket::connected, this, &ServerConnector::onConnected);
    // connect(socket, &QTcpSocket::errorOccurred, this, &ServerConnector::onError);
    // connect(socket, &QTcpSocket::disconnected, this, &ServerConnector::onDisconnected);

    // socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);

}


void ServerConnector::onConnected() {
    qDebug() << "Connected to Server";
}

void ServerConnector::onError(QAbstractSocket::SocketError error) {
    if(Close_Window_stat)return;

    qWarning() << "Error connect to Server:" << socket->errorString();
    // Запускаем повторное подключение через 3 секунды:
    QTimer::singleShot(3000, this, &::ServerConnector::SetUpConnection);
}

void ServerConnector::onDisconnected() {
    if(Close_Window_stat)return;

    qWarning() << "Disconnected from Server. Reconnecting...";
    QTimer::singleShot(3000, this, &::ServerConnector::SetUpConnection);
}

