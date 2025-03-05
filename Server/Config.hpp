#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <toml++/toml.h>
#include <QTcpSocket>


class Config {
public:
    struct Settings {
        nlohmann::json config_json;  // JSON конфігурація
        toml::table config_toml;     // TOML конфігурація
        int16_t server_port;
        // std::string server_channel;
        QHostAddress::SpecialAddress server_channel;  // Kept as QHostAddress::SpecialAddress

    };

public:
    Config(std::string_view aFilePath);
    ~Config() = default;

    void Load();

    const Settings& GetSettings() const;

private:
    bool isJsonFile() const;

    std::string filePath;
    Settings settings;

    void LoadJson();
    void LoadToml();
};
