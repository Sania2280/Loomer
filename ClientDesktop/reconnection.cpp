#include "aes.h"
#include "config.h"
#include "message.h"
#include "Mpack.hpp"
#include "messdatadase.h"
#include "userdata.h"
#include "regwindow.h"
#include "reconnection.h"
#include "mainwindow.h"
#include "clientdatabase.h"
#include "keygenerator.h"
#include "datasender.h"
#include <QThread>


#include <qtimer.h>

extern UserData &userData ;
QTcpSocket* Reconnection::socket = nullptr;
bool Reconnection::Close_Window_stat = false;
ClientDataBase DB;
MessDatadase mesDB;

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

void Reconnection::cryptoConnection()
{
    // connect(Reconnection::getInterface(), &Reconnection::getOpenData, )
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
        message.reconnect.id = userData.id.toStdString();

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
    Message message;

    userData.getInstance().bufer.append(socket->readAll());

    while (true) {
        // ждём хотя бы 4 байта для длины
        if (userData.getInstance().bufer.size() < static_cast<int>(sizeof(size_t)))
        {
            break;
        }

        size_t size;
        memcpy(&size, userData.getInstance().bufer.constData(), sizeof(size_t));
        qDebug() << "Mes size: " << size;

        // ждём пока придёт всё сообщение
        if (userData.getInstance().bufer.size() < static_cast<int>(sizeof(size_t) + size))
        {
            break;
        }

        // извлекаем сообщение
        QByteArray msgData = userData.getInstance().bufer.mid(sizeof(size_t), static_cast <qsizetype> (size));

        // удаляем его из буфера
        userData.getInstance().bufer.remove(
            0,
            static_cast<qsizetype>(sizeof(size_t) + size));
        // распаковываем msgpack
        message = Mpack::unpack(msgData.toStdString());

        qDebug() << "Got the message!";
    }

    userData.getInstance().bufer.clear();


    // Message message = Mpack::unpack(socket->readAll().toStdString());


    switch (message.id) {
        case MesageIdentifiers::ID_CLIENT:
        {
            if ( userData.mainWindStarted){
                userData.ClientsData[QString::fromStdString(message.newOrDeleteClientInNet.id)].nick = QString::fromStdString(message.newOrDeleteClientInNet.nick);
                DB.AddUser(message);
                emit mainWindSocketPrint(DB.GetAllUsersNicks());
            }
            break;
        }
        case MesageIdentifiers::ID_DELETE:
        {
            if ( userData.mainWindStarted){
                QString nickToDelete = DB.GetNick(QString::fromStdString(message.newOrDeleteClientInNet.id));
                DB.ChangeUserStat(QString::fromStdString(message.newOrDeleteClientInNet.id));
                qDebug() << "To Delete :" << QString::fromStdString(message.newOrDeleteClientInNet.id);
                emit mainWindSocketDelete(nickToDelete);
            }
            break;
        }
        case MesageIdentifiers::MESAGE:
        {
            if (userData.mainWindStarted){
                AES aes;

                QString mes = aes.decryptString(
                    QString::fromStdString(message.messageData.message),
                    mesDB.GetSecret(QString::fromStdString(message.messageData.senderId)));

               emit mainWindPrintMessage(mes);
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
                userData.id = QString::fromStdString(message.registrationData.id);
                DB.CreateDB();
                mesDB.CreateDB();
                emit regWindow->CloseWindow();
            }
            break;
        }
        case MesageIdentifiers::OPENDATA:
        {
            if (userData.mainWindStarted){

                QString p ;
                QString q ;

                for (size_t i = 0; i < message.openData.p.size(); ++i) {
                    p += QString::number(message.openData.p[i]);
                }

                for (size_t i = 0; i < message.openData.q.size(); ++i) {
                    q += QString::number(message.openData.q[i]);
                }

                mesDB.SetOpenData(QString().fromStdString(message.openData.sendId), p, q);
                mesDB.SetStatus(QString().fromStdString(message.openData.sendId), false);

                KeyGenerator keyGen;
                QString closeKey;

                std::vector<unsigned char> key = keyGen.GetKey();

                for (size_t i = 0; i < key.size(); ++i) {
                    closeKey += QString::number(key[i]);
                }

                QString openKey = keyGen.GetOpenKeyOrSecret(q, closeKey, p);

                mesDB.SetCloseKey(QString::fromStdString(message.openData.sendId), closeKey);
                mesDB.SetOpenKey(QString().fromStdString(message.openData.sendId), openKey);

                DataSender dataSendlser;
                dataSendlser.SendMyOpenKey(QString::fromStdString(message.openData.sendId), openKey);

            }
            break;
        }
        case MesageIdentifiers::OPENDATAMY:
        {
            qDebug() << "OPENDATAMY";
            if (userData.mainWindStarted){


                QString p ;
                QString q ;

                for (size_t i = 0; i < message.openData.p.size(); ++i) {
                    p += QString::number(message.openData.p[i]);
                }

                for (size_t i = 0; i < message.openData.q.size(); ++i) {
                    q += QString::number(message.openData.q[i]);
                }

                mesDB.SetOpenData(QString().fromStdString(message.openData.resId), p, q);
                mesDB.SetStatus(QString().fromStdString(message.openData.resId), true);


                KeyGenerator keyGen;
                QString closeKey;

                std::vector<unsigned char> key = keyGen.GetKey();

                for (size_t i = 0; i < key.size(); ++i) {
                    closeKey += QString::number(key[i]);
                }

                QString openKey = keyGen.GetOpenKeyOrSecret(q, closeKey, p);
                mesDB.SetCloseKey(QString::fromStdString(message.openData.resId), closeKey);
                mesDB.SetOpenKey(QString().fromStdString(message.openData.resId), openKey);

                DataSender dataSendlser;
                dataSendlser.SendMyOpenKey(QString::fromStdString(message.openData.resId), openKey);

            }
            break;
        case MesageIdentifiers::OPENKEY:
        {
              if (userData.mainWindStarted){

                KeyGenerator keyGen;

                  if(userData.id == QString::fromStdString(message.openKeyPac.resId)){

                      QString p = mesDB.GetP(QString::fromStdString(message.openKeyPac.sendId));
                      QString closeKey = mesDB.GetCloseKey(QString::fromStdString(message.openKeyPac.sendId));

                      QString secret = keyGen.GetOpenKeyOrSecret(QString::fromStdString(message.openKeyPac.openKey), closeKey, p);
                      mesDB.SetSecret(QString::fromStdString(message.openKeyPac.sendId), secret);
                  }

                  bool stat = mesDB.GetStat(QString::fromStdString(message.openKeyPac.sendId));

                  if(stat){
                      AES aes;
                      QString crypt = aes.encryptString(mesDB.GetMesage(QString::fromStdString(message.openKeyPac.sendId)),
                                                           mesDB.GetSecret(QString::fromStdString(message.openKeyPac.sendId)));

                      DataSender dataSendler;
                      dataSendler.MessPacker(DB.GetNick(QString::fromStdString(message.openKeyPac.sendId)), crypt);

                      mesDB.SetCryptData(crypt, QString::fromStdString(message.openKeyPac.sendId));
                  }

              }
              break;
        }

        }

        case MesageIdentifiers::RECONNECTION:{}

        case MesageIdentifiers::SIGN_SEC:{}

        case MesageIdentifiers::LOG:{}
        case MesageIdentifiers::SIGN:{}
        case MesageIdentifiers::NONE:{}
        case MesageIdentifiers::ID_MY:{}
        case MesageIdentifiers::CLIENT_READY_TO_WORCK:{}
            default:
        break;

    }
}




