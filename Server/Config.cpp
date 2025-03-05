#include <fstream>
#include <stdexcept>
#include <QHostAddress>

#include "Config.hpp"

Config::Config(std::string_view aFilePath) : filePath{aFilePath} {}

void Config::Load() {
    if (isJsonFile()) {
        LoadJson();
    } else {
        LoadToml();
    }
}

bool Config::isJsonFile() const {
    return filePath.substr(filePath.find_last_of('.') + 1) == "json";
}

void Config::LoadJson() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening JSON config file: " + filePath);
    }

    try {
        file >> settings.config_json;
        settings.server_port = settings.config_json["Settings"]["server-port"];
        settings.server_channel = settings.config_json["Settings"]["server-ip"];
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing JSON: " + std::string(e.what()));
    }
}

void Config::LoadToml() {
    try {
        settings.config_toml = toml::parse_file(filePath);
        settings.server_port = *settings.config_toml["Settings"]["server-port"].value<int16_t>();
        settings.server_channel = settings.config_json["Settings"]["server-ip"];
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing TOML: " + std::string(e.what()));
    }
}

const Config::Settings& Config::GetSettings() const {
    return settings;
}
