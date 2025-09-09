#include "diffiehellman.h"
#include "keygen.h"
#include "keystorage.h"
#include <qDebug>
#include "clientdatabase.h"

#include "datasender.h"

DiffieHellman::DiffieHellman() {

    dataSendlerPtr = new DataSender ;


    connect(this, &DiffieHellman::SendOpenKey, dataSendlerPtr, &DataSender::SendOpenKey);
}

void DiffieHellman::ShearingKeys(QString nick)
{
    ClientDataBase DB;

    QString id = DB.GetId(nick);
    KeyStorage& storage = KeyStorage::getInstance();

    storage.Keys[id].closeKey = KeyGen::KeyGenerator();
    storage.Keys[id].openKey = KeyGen::KeyGenerator();

    emit SendOpenKey(id, storage.Keys[id].openKey);
}
