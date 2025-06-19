#include "datasender.h"
#include "UserData.h"

#include <Mpack.hpp>
#include "message.h"

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


    userData.getSocket()->write(QByteArray::fromStdString(Mpack::puck(message)));

    if (!userData.getSocket()->waitForBytesWritten(5000)) {
        qWarning() << "Sending data error:" << userData.getSocket()->errorString();
    }

}

void DataSender::SendMyMasseg(Message message)
{
    if (userData.getSocket()->state() == QAbstractSocket::ConnectedState) {
        userData.getSocket()->write(Mpack::puck(message).data());


        if (!userData.getSocket()->waitForBytesWritten(5000)) {
            qWarning() << "Sending data error:" << userData.getSocket()->errorString();
        }
    } else {
        qDebug() << "Socket not connected";
    }
}



