#include "cliendatabase.h"
#include "enums.h"

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

MesageIdentifiers ClienDataBase::LogIn(QString nick, QString pass)
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

    QString desckriptor = "";

    QString persID;

    bool nickCounter = false;
    bool passCounter = false;

    for (const QString &key : database.keys()) {
        QJsonValue value = database.value(key);
        if (!value.isObject())
            continue;

        QJsonObject userObj = value.toObject();

        if(userObj.value("nick").toString() == nick){
            nickCounter = true;
            if(userObj.value("password").toString() == pass){
                passCounter = true;
                desckriptor = key;
            }
        }
    }

    if(nickCounter == false) {return MesageIdentifiers::LOGIN_FAIL_NAME; qDebug() << "LOGIN_FAIL_NAME";}
    else if (passCounter == false) {return MesageIdentifiers::LOGIN_FAIL_PASS;qDebug() << "LOGIN_FAIL_PASS";}
    else {return MesageIdentifiers::LOGIN_SEC;qDebug() << "LOGIN_SEC";}


}

MesageIdentifiers ClienDataBase::SingUp(QString nick, QString pass, int descriptor)
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
            return MesageIdentifiers::SIGN_FAIL;
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

int ClienDataBase::ClientID(QJsonObject database)
{
    std::random_device rd;  // Ініціалізація випадкового пристрою
    std::mt19937 gen(rd()); // Генератор випадкових чисел
    std::uniform_int_distribution<int> distrib(1, 100); // Діапазон від 1 до 100

    int randomID = distrib(gen);

    if(database.contains(QString::number(randomID))) return ClientID(database);
    else return randomID;
}
