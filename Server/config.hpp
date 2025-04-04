#pragma once

#include <QJsonObject>
#include <QTcpSocket>

class Config {
public:
    struct Settings {
        QJsonObject config_obj;
        qint16 server_port;
        QHostAddress::SpecialAddress server_channel;
    };

public:
    Config(std::string_view aFilePath);
    ~Config() = default;

    void Load();

    const Settings& GetSettings() const;

private:
    std::string filePath;

    Settings settings;
};
