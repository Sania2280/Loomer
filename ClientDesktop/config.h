#pragma once

#include <nlohmann/json.hpp>
#include <toml++/toml.h>
#include <QString>

#include "mainwindow.h"

class Config {
private:
    MainWindow *mainWindow;
    void ReadJson(const QString &filePath);
    void ReadToml(const QString &filePath);

public:
    Config() = default;
    void Read();

     Config(MainWindow *mw) : mainWindow(mw) {}

    struct Settings {
        nlohmann::json config_json;  // JSON конфігурація
        toml::table config_toml;     // TOML конфігурація
        qint16 server_port;
        QString server_ip;
    };

    static Settings settings;

};
