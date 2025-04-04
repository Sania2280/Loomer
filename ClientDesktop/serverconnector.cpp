#include "serverconnector.h"
#include "regwindow.h"
#include "UserData.h"
#include "config.h"
#include "reconnection.h"

#include "Mpack.hpp"
#include "ui_RegWindow.h"
#include "message.h"



#include <QTcpSocket>
#include <winsock.h>
#include <QTimer>
#include "Mpack.hpp"


extern UserData &userData ;

ServerConnector::ServerConnector(QObject *parent, RegWindow *rWindow) :
    QObject(parent),
    socket(new QTcpSocket(this)),
    regWind(rWindow) {

    // Reconnection reconection(socket, regWind) ;


        connect(socket, &QTcpSocket::connected, this, &ServerConnector::onConnected);
        connect(socket, &QTcpSocket::errorOccurred, this, &ServerConnector::onError);
        connect(socket, &QTcpSocket::disconnected, this, &ServerConnector::onDisconnected);
     }

void ServerConnector::ConnectToServer()
{
    Config config;
    config.Read();

    if (!socket) { // Создаём сокет, если его нет
        socket = new QTcpSocket(this);
    }

    SetUpConnection();
    userData.mainWindStarted = false;

    if (!userData.getSocket()) {  // Проверка перед установкой
        userData.setSocket(socket);
        qDebug() << "Socket made:" << userData.getSocket() << "desk" << socket->socketDescriptor();
    } else {
        qWarning() << "Socet is already exist";
    }

    if (!socket->waitForConnected(5000)) {
        qWarning() << "Error connection to server:" << socket->errorString();
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &ServerConnector::slotReadyRead);

}

void ServerConnector::SendMyData(MesageIdentifiers status)
{
    Message message;

    message.id = status;
    message.registrationData.nickName = userData.name.toStdString();
    message.registrationData.pass = userData.pass.toStdString();

    socket->write(QByteArray::fromStdString(Mpack::puck(message)));

    if (!socket->waitForBytesWritten(5000)) {
        qWarning() << "Sending data error:" << socket->errorString();
    }


}



void ServerConnector::slotReadyRead()
{
    qDebug() << "Reading...";

    UserData& userdata = UserData::getInstance();

    if(userdata.mainWindStarted != true){

    Message message = Mpack::unpack(socket->readAll().toStdString());

        if(message.id == MesageIdentifiers::LOGIN_SEC){
            qDebug() << "LOGIN_SEC";
        }

        switch (message.id) {
            case MesageIdentifiers::LOGIN_SEC:{
                qDebug()<< "Got desk: " << message.registrationData.desckriptor;
                userdata.desck = QString::fromStdString(message.registrationData.desckriptor);
                emit regWind->CloseWindow();
                break;
                }
            case MesageIdentifiers::LOGIN_FAIL_NAME:{
                qDebug() << "Log in faill NAME";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: Incorrect Name");
                break;
            }
            case MesageIdentifiers::LOGIN_FAIL_PASS:{
                qDebug() << " Log in faill PASS";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: Incorrect Password");
                break;
            }
            case MesageIdentifiers::SIGN_SEC:{
                qDebug() << "Accaaunt created";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("Accaunt created succsed");
                break;
            }
            case MesageIdentifiers::SIGN_FAIL:{
                qDebug() << "Created accaunt ERROR";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: Accaunt creation Faild");
                break;
            }
            case MesageIdentifiers::SIGN_FAIL_EXIST:{
                qDebug() << "User Exist ERROR";
                regWind->ui->listWidget_errors->clear();
                regWind->ui->listWidget_errors->addItem("ERROR: User alredy exist");
                break;
            }
            case MesageIdentifiers::NONE:{}
            case MesageIdentifiers::ID_MY:{}
            case MesageIdentifiers::ID_CLIENT:{}
            case MesageIdentifiers::ID_DELETE:{}
            case MesageIdentifiers::MESAGE:{}
            case MesageIdentifiers::LOG:{}
            case MesageIdentifiers::SIGN:{}
            case MesageIdentifiers::CLIENT_READY_TO_WORCK:{}

            default: {
                break;
            }

        }

    }
}


void ServerConnector::SetUpConnection(){

    socket->abort();
    socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);
}


void ServerConnector::onConnected() {
    qDebug() << "Connected to Server";

    regWind->ui->listWidget_errors->clear();
    regWind->ui->listWidget_errors->addItem("Connected to Server");
    QTimer::singleShot(3000, regWind->ui->listWidget_errors, &QListWidget::clear);
}

void ServerConnector::onError() {
    if(Close_Window_stat)return;

    qWarning() << "Error connect to Server:" << socket->errorString();
    // Запускаем повторное подключение через 3 секунды:
    QTimer::singleShot(3000, this, &::ServerConnector::SetUpConnection);
}

void ServerConnector::onDisconnected() {
    if(Close_Window_stat)return;

    qWarning() << "Disconnected from Server. Reconnecting...";
    QTimer::singleShot(3000, this, [this](){
        ServerConnector::SetUpConnection();

        if(regWind != nullptr){
        regWind->ui->listWidget_errors->clear();
        regWind->ui->listWidget_errors->addItem("ERROR: lost connection");
        }
    });

}

