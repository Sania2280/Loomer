#include "cliendatabase.h"
#include <random>

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

QString ClienDataBase::LogIn(QString nick, QString pass)
{
    qDebug() << "Log In";

    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error open DB for reading:" ;
        CreateateDB();

    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject database = doc.object();

    for (const QString &key : database.keys()) {
        QJsonValue value = database.value(key);
        if (!value.isObject())
            continue;

        QJsonObject userObj = value.toObject();
        if (userObj.contains("nick") && userObj.contains("password")) {
            if (userObj.value("nick").toString() == nick &&
                userObj.value("password").toString() == pass) {
                qDebug() << "User found:" << key;

                return key;
            }
        }
    }

    return "";
}

bool ClienDataBase::SingUp(QString nick, QString pass, QTcpSocket *socket)
{
    qDebug() << "Sing Up";

    QFile file("users.json");
    // Попытка открыть файл для чтения
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error open DB for reading:";
        CreateateDB();
        // После создания базы нужно снова открыть файл для чтения
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Error open DB for reading after creation:";
            return false;
        }
    }



    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject database = doc.object();

    for (const QString &key : database.keys()) {
        QJsonValue value = database.value(key);
        if (!value.isObject())
            continue;

        QJsonObject userObj = value.toObject();
        if (userObj.contains("nick") && userObj.contains("password")) {
            if (userObj.value("nick").toString() == nick ) {
                qDebug() << "User already exist" << key;
                return false;

              }
        }
    }


    QJsonObject newUser;
    newUser["nick"] = nick;
    newUser["password"] = pass;
    newUser["desk"] = QString::number(socket->socketDescriptor());

    // Добавляем нового пользователя; желательно использовать уникальный ключ
    database[QString::number(ClientID(database))] = newUser;

    QJsonDocument newDoc(database);
    // Открываем файл для записи
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Error open DB for writing:" << file.errorString();
        return false;
    }
    file.write(newDoc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

int ClienDataBase::ClientID(QJsonObject database)
{
    std::random_device rd;  // Ініціалізація випадкового пристрою
    std::mt19937 gen(rd()); // Генератор випадкових чисел
    std::uniform_int_distribution<int> distrib(1, 100); // Діапазон від 1 до 100

    int randomID = distrib(gen);

    if(database.contains(QString::number(randomID)))
    {
        ClientID(database);
    }
    else
    {
        return randomID;
    }
    return -1;
}
