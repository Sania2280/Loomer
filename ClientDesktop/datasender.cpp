#include <Mpack.hpp>

#include "datasender.h"
#include "UserData.h"
#include "clientdatabase.h"
#include "message.h"

#include "enums.h"
extern UserData &userData ;

DataSender::DataSender(QObject *parent) :
    QObject(parent)
   {}

void DataSender::SendMyData(MesageIdentifiers status)
{

    Message message;

    message.id = status;
    message.registrationData.nick = userData.name.toStdString();
    message.registrationData.pass = userData.pass.toStdString();


    userData.getSocket()->write(QByteArray::fromStdString(Mpack::puckMes(message)));

    if (!userData.getSocket()->waitForBytesWritten(5000)) {
        qWarning() << "Sending data error:" << userData.getSocket()->errorString();
    }

}

void DataSender::SendMyMasseg(Message message)
{
    if (userData.getSocket()->state() == QAbstractSocket::ConnectedState) {
        userData.getSocket()->write(Mpack::puckMes(message).data());

        qDebug() << "Mes sended";


        if (!userData.getSocket()->waitForBytesWritten(5000)) {
            qWarning() << "Sending data error:" << userData.getSocket()->errorString();
        }
    } else {
        qDebug() << "Socket not connected";
    }
}

void DataSender::SendMyKey(KeySending keysending)
{
    if (userData.getSocket()->state() == QAbstractSocket::ConnectedState) {
        userData.getSocket()->write(Mpack::puckKey(keysending).data());


        if (!userData.getSocket()->waitForBytesWritten(5000)) {
            qWarning() << "Sending data error:" << userData.getSocket()->errorString();
        }
    } else {
        qDebug() << "Socket not connected";
    }
}

void DataSender::MessPacker(QString ResNick, QString Mess)
{
    ClientDataBase DB;
    QString id = DB.GetId(ResNick);

    Message message;
    message.id = MesageIdentifiers::MESAGE;
    message.messageData.message = Mess.toStdString();
    message.messageData.resivId = id.toStdString();
    message.messageData.senderId = userData.id.toStdString();

    SendMyMasseg(message);
}

void DataSender::SendOpenKey(QString id, std::vector<unsigned char> openKey)
{
    KeySending keysrnding;
    keysrnding.idKey = CryptoEnum::OPENKEY;
    keysrnding.key.id = id.toStdString();
    keysrnding.key.key = openKey;

    // SendMyKey(keysrnding);

}



