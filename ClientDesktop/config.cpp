#include "config.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Config::Settings Config::settings;

void Config::Read() {
    QString filePath = "config_client.json"; // Основний конфіг
    if (!QFile::exists(filePath)) {
        filePath = "config_client.toml"; // Якщо JSON немає, шукаємо TOML
    }
    if (filePath.endsWith(".json")) {
        ReadJson(filePath);
    } else if (filePath.endsWith(".toml")) {
        ReadToml(filePath);
    } else {
        qWarning() << "Unknown config file format!";
    }
}

void Config::ReadJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error opening JSON config:" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument configJson = QJsonDocument::fromJson(data);
    if (configJson.isNull() || !configJson.isObject()) {
        qWarning() << "Invalid JSON format in config";
        return;
    }

    QJsonObject configObj = configJson.object().value("Settings").toObject();
    settings.server_ip = configObj.value("server-ip").toString();
    settings.server_port = configObj.value("server-port").toInt();
}

void Config::ReadToml(const QString &filePath) {
    try {
        settings.config_toml = toml::parse_file(filePath.toStdString());

        if (auto ip = settings.config_toml["Settings"]["server-ip"].value<std::string>()) {
            settings.server_ip = QString::fromStdString(*ip);
        }

        if (auto port = settings.config_toml["Settings"]["server-port"].value<int>()) {
            settings.server_port = static_cast<qint16>(*port);
        }

    } catch (const std::exception &e) {
        qWarning() << "Error parsing TOML:" << e.what();
    }
}
