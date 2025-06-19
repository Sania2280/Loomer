#include "clientdatabase.h"
#include <qdeadlinetimer.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "userdata.h"

#include <QFile>
#include <QDebug>
#include <QStringBuilder>


ClientDataBase::ClientDataBase() {}


QString fileName ;


void ClientDataBase::CreateDB()
{
    QString filetemp = ("usersClient" + UserData::getInstance().desck + ".json");
    fileName = filetemp;
    QJsonObject database;


    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "JSON made:";
    } else {
        qWarning() << "Error open JSON:" ;
    }
}

void ClientDataBase::CreateDBfirst(QString desk)
{
    QString filename = "users" + desk + ".json";
    QFile file(filename);
}

void ClientDataBase::AddUser(Message mesege)
{
    qDebug() << fileName;
    QFile file(fileName);

    QJsonObject database;

    if(file.open(QIODevice::ReadOnly)){
        QJsonDocument oldDoc = QJsonDocument::fromJson(file.readAll());
        file.close();
        database = oldDoc.object();

        for (const QString &key : database.keys()) {
            QJsonValue value = database.value(key);
            if (!value.isObject()){
                continue;
            }

            QJsonObject userObj = value.toObject();
            if(userObj.contains(QString::fromStdString(mesege.newOrDeleteClientInNet.id))){
                qWarning() << "User is exist";
                break;
            }
        }

    }
    else{
        CreateDB();
        AddUser(mesege);
    }

    file.close();

    QJsonObject newUser;
    database[QString::fromStdString(mesege.newOrDeleteClientInNet.id)] = newUser;

    if(file.open(QIODevice::WriteOnly)){
        newUser["nick"] = QString::fromStdString(mesege.newOrDeleteClientInNet.nick);
        newUser["desk"] = QString::fromStdString(mesege.newOrDeleteClientInNet.descriptor);

        database[QString::fromStdString(mesege.newOrDeleteClientInNet.id)] = newUser;

        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
    }
    file.close();
}
