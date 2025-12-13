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

void MessDatadase::SetOpenData(QString resId, QString p, QString q)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    // 🔍 Проверяем, если JSON пустой ({}), добавляем новые данные сразу
    if (database.isEmpty()) {
        QJsonObject userObj;
        userObj["p"] = p;
        userObj["q"] = q;
        database[resId] = userObj;

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
            file.close();
            qDebug() << "User added to empty database";
        } else {
            qWarning() << "Failed to open file for writing";
        }
        return; // ⚠️ Выходим, чтобы не идти в цикл
    }

    // 🔄 Если база не пуста — обновляем или добавляем запись
    for (const QString &key : database.keys()) {
        QJsonObject userObj = database.value(key).toObject();

        if (key == resId) {
            userObj["p"] = p;
            userObj["q"] = q;
            database[key] = userObj;
            break;
        }
    }

    // 💾 Сохраняем изменения
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "User data saved successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }
}


void MessDatadase::SetCloseKey(QString resId, QString closeKey)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == resId){
            userObj["CloseKey"] = closeKey;
            database[key] = userObj;
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Close Key successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }

}

QString MessDatadase::GetCloseKey(QString resId)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);
    QString closeKey;

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == resId){
            closeKey.push_back(userObj.value("CloseKey").toString());
            break;
        }
    }
    return closeKey;
}

void MessDatadase::SetStatus(QString resId, bool stat)
{

    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == resId){
            if(stat){
                userObj["stat"] = "SENDER";
            }else if(!stat){
                userObj["stat"] = "RESIVER";
            }

            database[key] = userObj;
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Status added successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }
}

void MessDatadase::SetOpenKey(QString id, QString openKey)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == id){
            userObj["OpenKey"] = openKey;
            database[key] = userObj;
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Open Key added successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }
}

void MessDatadase::SetSecret(QString id, QString secret)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == id){
            userObj["Secret"] = secret;
            database[key] = userObj;
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Open Key added successfully";
    } else {
        qWarning() << "Failed to open file for writing";
    }
}



QString MessDatadase::GetP(QString id)
{

    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    QString p;
    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == id){
            p.push_back(userObj.value("p").toString());
            break;
        }
    }
    return p;
}

QString MessDatadase::GetSecret(QString sendId)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    QString secret ;
    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == sendId){
            secret = userObj.value("Secret").toString();
            break;
        }
    }
    return secret;
}

QString MessDatadase::GetMesage(QString sendId)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    QString text ;
    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == sendId){
            text = userObj.value("mes").toString();
            break;
        }
    }
    return text;
}

bool MessDatadase::GetStat(QString sendId)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    bool stat = false;
    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == sendId){
            if(userObj.value("stat").toString() == "RESEVER") stat = false;
            else if(userObj.value("stat").toString() == "SENDER") stat = true;
            break;
        }
    }
    return stat;
}

void MessDatadase::SetCryptData(QString data, QString sendId)
{
    QFile file(fileNamemDB);
    QJsonObject database = ReadFile(fileNamemDB);

    for (const QString & key : database.keys()) {

        QJsonValue value = database.value(key);
        QJsonObject userObj = value.toObject();

        if(key == sendId){
            userObj["CryptData"] = data;
            database[key] = userObj;
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Open Key added successfully";
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
