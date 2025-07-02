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
    QString filetemp = ("usersClient" + UserData::getInstance().id + ".json");
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

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument oldDoc = QJsonDocument::fromJson(file.readAll());
        file.close();
        database = oldDoc.object();
    } else {
        CreateDB(); // создаём пустой JSON-файл, если он не существует
        AddUser(mesege); // повторный вызов после создания файла
        return;
    }

    QString userID = QString::fromStdString(mesege.newOrDeleteClientInNet.id);

    if (database.contains(userID)) {
        qWarning() << "User already exists. Skipping addition.";
        return;
    }

    if(database.contains(UserData::getInstance().id)){
        qWarning() << "User already exists. Skipping addition.";
        return;
    }

    // Создаём нового пользователя
    QJsonObject newUser;
    newUser["nick"] = QString::fromStdString(mesege.newOrDeleteClientInNet.nick);

    database[userID] = newUser;

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "User added successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }
}

QVector<QString> ClientDataBase::GetAllUsersNicks()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error opening DB for reading.";
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON format.";
    }

    QJsonObject database = doc.object();

    QVector<QString> Nicks;

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        Nicks.push_back(userObj.value("nick").toString());
    }

    return Nicks;

}

QString ClientDataBase::GetId(QString Nick)
{
   QJsonObject database =  ReadFile(fileName);

    for (const QString& key : database.keys()) {
        QJsonObject userObj = database.value(key).toObject();
        QString nick = userObj.value("nick").toString();

        if (nick == Nick) {
            return key;
        }
    }
    return QString();
}

QString ClientDataBase::GetNick(QString id)
{
    QJsonObject database = ReadFile(fileName);

    for (const QString& key : database.keys()) {
        QJsonObject userObj = database.value(key).toObject();

        if (key == id) {
            return userObj.value("nick").toString();  // Знайшли ID
        }
    }
    return "";
}


QJsonObject ClientDataBase::ReadFile(QString name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error opening DB for reading.";
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON format.";
    }

    QJsonObject database = doc.object();

    return database;
}

