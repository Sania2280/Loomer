#include "cliendatabase.h"
#include "enums.h"

#include <random>

const QString DBname = "users.json";

ClienDataBase::ClienDataBase(QObject *parent)
    : QObject{parent}
{}

void ClienDataBase::CreateateDB()
{
    QJsonObject database;
    QJsonObject users; // пустой объект пользователей
    database["users"] = users;

    QFile file("users.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "JSON made:";
    } else {
        qWarning() << "Error open JSON:" ;
    }
}

ClienDataBase::LogInStruct ClienDataBase::LogIn(QString nick, QString pass)
{
    qDebug() << "Log In";

    QJsonObject database = ReadFile(DBname);


    bool nickCounter = false;
    bool passCounter = false;
    QString clientID = QString();

    for (const QString &key : database.keys()) {
        QJsonValue value = database.value(key);
        if (!value.isObject())
            continue;

        QJsonObject userObj = value.toObject();

        if(userObj.value("nick").toString() == nick){
            nickCounter = true;
            if(userObj.value("password").toString() == pass){
                passCounter = true;
                clientID = key;
                break;
            }
        }
    }



    if(nickCounter == false) {
        qDebug() << "LOGIN_FAIL_NAME";

        LogInStruct logInStruct;
        logInStruct.mesID = MesageIdentifiers::LOGIN_FAIL_NAME;
        logInStruct.clientID = clientID;

        return logInStruct;
    }
    else if (passCounter == false) {
        qDebug() << "LOGIN_FAIL_PASS";

        LogInStruct logInStruct;
        logInStruct.mesID = MesageIdentifiers::LOGIN_FAIL_PASS;
        logInStruct.clientID = clientID;


        return logInStruct;
    }

    LogInStruct logInStruct;
    logInStruct.mesID = MesageIdentifiers::LOGIN_SEC;
    logInStruct.clientID = clientID;

    qDebug() << "LOGIN_SEC";
    return logInStruct;


}

MesageIdentifiers ClienDataBase::SingUp(QString nick, QString pass, int descriptor)
{
    qDebug() << "Sing Up";


    QJsonObject database = ReadFile(DBname);


    for (const QString &key : database.keys()) {
        QJsonValue value = database.value(key);
        if (!value.isObject())
            continue;

        QJsonObject userObj = value.toObject();
        if (userObj.contains("nick") && userObj.contains("password")) {
            if (userObj.value("nick").toString() == nick ) {
                qDebug() << "User already exist" << key;
                return MesageIdentifiers::SIGN_FAIL_EXIST;

              }
        }
    }


    QJsonObject newUser;
    newUser["nick"] = nick;
    newUser["password"] = pass;
    newUser["desk"] = descriptor;

    // Добавляем нового пользователя; желательно использовать уникальный ключ
    database[QString::number(ClientID(database))] = newUser;

    QFile file(DBname);
    QJsonDocument newDoc(database);
    // Открываем файл для записи
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Error open DB for writing:" << file.errorString();
        return MesageIdentifiers::SIGN_FAIL;
    }
    file.write(newDoc.toJson(QJsonDocument::Indented));
    file.close();

    return MesageIdentifiers::SIGN_SEC;
}

QString ClienDataBase::GetNick(std::string id)
{

    QJsonObject database = ReadFile(DBname);

    QString targetDesk = QString::fromStdString(id).trimmed();

    for (const QString &key : database.keys()) {
        QJsonValue value = database.value(key);
        if (!value.isObject())
            continue;

        if (key == QString::fromStdString(id)) {
            qDebug() << "Nick found";
            QJsonObject userObj = value.toObject();
            return userObj.value("nick").toString();
        }


    }
    qDebug() << "Descriptor not found:" << targetDesk;
    return "";
}


int ClienDataBase::ClientID(QJsonObject database)
{
    std::random_device rd;  // Ініціалізація випадкового пристрою
    std::mt19937 gen(rd()); // Генератор випадкових чисел
    std::uniform_int_distribution<int> distrib(1, 100); // Діапазон від 1 до 100

    int randomID = distrib(gen);

    if(database.contains(QString::number(randomID))) {
        return ClientID(database);
    }
    else {
        return randomID;
    }   
}

QString ClienDataBase::GetId(QString desk)
{
    QJsonObject database = ReadFile(DBname);

    for (const QString& key : database.keys()) {

        QJsonValue value = database.value(key);

        QJsonObject userObj = value.toObject();

        if(userObj.value("desk") == desk){
            return key;
        }
    }

    return QString();


}

void ClienDataBase::RewriteDesk(std::string id, QString desk)
{
    QJsonObject database = ReadFile(DBname);

    QString userID = QString::fromStdString(id);

    if (database.contains(userID)) {
        QJsonObject userObj = database[userID].toObject();
        userObj["desk"] = desk;
        database[userID] = userObj; // Обязательно записываем обратно!

        // Перезапись файла
        QFile file(DBname);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QJsonDocument(database).toJson(QJsonDocument::Indented));
            file.close();
            qDebug() << "Desk rewritten successfully.";
        } else {
            qWarning() << "Failed to open DB for writing.";
        }
    } else {
        qWarning() << "User ID not found:" << userID;
    }
}

QJsonObject ClienDataBase::ReadFile(QString name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error opening DB for reading.";
        CreateateDB();
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
