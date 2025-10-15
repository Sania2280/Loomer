#include <Mpack.hpp>
#include <qendian.h>

#include "datasender.h"
#include "UserData.h"
#include "clientdatabase.h"
#include "message.h"
#include "messdatadase.h"
#include "keyexchange.h"
// #include "keygenerator.h"

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


    QByteArray mes;

    auto sub = Mpack::puck(message);
    quint32 size = static_cast<quint32> (sub.size());
    quint32 size_be = qToBigEndian(size);
    qDebug() << "Size1: " << size;

    // mes.push_back(reinterpret_cast <char *>(&size));
    // mes.push_back(sub.data());

    mes.append(reinterpret_cast <char *>(&size_be), sizeof(size_be));
    mes.append(sub.data(), static_cast <qsizetype> (sub.size()));

    userData.getSocket()->write(mes);
    // userData.getSocket()->write(QByteArray::fromStdString(Mpack::puck(message)));

    if (!userData.getSocket()->waitForBytesWritten(5000)) {
        qWarning() << "Sending data error:" << userData.getSocket()->errorString();
    }

    userData.bufer.clear();

}

void DataSender::SendMyMasseg(Message message)
{
    if (userData.getSocket()->state() == QAbstractSocket::ConnectedState) {

        QByteArray mes;

        auto sub = Mpack::puck(message);
        quint32 size = static_cast<quint32> (sub.size());
        quint32 size_be = qToBigEndian(size);

        mes.append(reinterpret_cast <char *>(&size_be), sizeof(size_be));
        mes.append(sub.data(), static_cast <qsizetype> (sub.size()));

        userData.getSocket()->write(mes);
        // QByteArray data = Mpack::puck(message).data();
        // qDebug() << "Mes size:" << data.size();


        if (!userData.getSocket()->waitForBytesWritten(5000)) {
            qWarning() << "Sending data error:" << userData.getSocket()->errorString();
        }
    } else {
        qDebug() << "Socket not connected";
    }
    userData.bufer.clear();

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

    MessDatadase mesDB;
    mesDB.SetMes(Mess, ResNick);

    SendMyMasseg(message);
}

void DataSender::SendKeyIniForServer(QString ResNick)
{
    KeyExchange keyEx;

    Message message = keyEx.GetOpenKey(ResNick.toStdString());
    SendMyMasseg(message);

}



