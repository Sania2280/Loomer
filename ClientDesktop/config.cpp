#include "config.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


void Config::Read() {
    QFile file("config_client.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error open config";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Парсим JSON
    QJsonDocument config_json = QJsonDocument::fromJson(data);

    if (config_json.isNull()) {
        qDebug() << "Error worck with config";
        return;
    }

    QJsonObject config_obj = config_json.object();

    const auto SettingsObj = config_obj.value("Settings").toObject();

    Config::settings.server_ip = SettingsObj.value("server-ip").toString();
    Config::settings.server_port = static_cast<qint16>(SettingsObj.value("server-port").toInt());

}
