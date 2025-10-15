#include "messdatadase.h"
#include "UserData.h"
#include "clientdatabase.h"

QString fileNamemDB;
MessDatadase::MessDatadase() {}

void MessDatadase::CreateDB()
{
    QString filetemp = ("messageDb" + UserData::getInstance().id + ".json");
    fileNamemDB = filetemp;
    QJsonObject database;


    QFile file(fileNamemDB);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "JSON made:";
    } else {
        qWarning() << "Error open JSON:" ;
    }
}

void MessDatadase::SetMes(QString mes, QString nick)
{
    QFile file(fileNamemDB);

    QJsonObject database = ReadFile(fileNamemDB);

    ClientDataBase DB;
    QString resId = DB.GetId(nick);

    QJsonObject newMes;

    newMes["mes"] = mes;
    database[resId] = newMes;

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "User added successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }

}



QJsonObject MessDatadase::ReadFile(QString name)
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
