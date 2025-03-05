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
            settings.config_json = nlohmann::json::parse(file, nullptr, true, true);

            // Ensure "Settings" exists
            if (!settings.config_json.contains("Settings")) {
                throw std::runtime_error("Missing 'Settings' section in JSON config.");
            }

            auto& settings_json = settings.config_json["Settings"];

            // Validate 'server-port' (must exist & be a number)
            if (settings_json.contains("server-port") && settings_json["server-port"].is_number()) {
                settings.server_port = settings_json["server-port"];
            } else {
                throw std::runtime_error("Invalid or missing 'server-port' in JSON config.");
            }

            // Validate 'host-adres'
            if (settings_json.contains("host-adres") && settings_json["host-adres"].is_string()) {
                std::string hostAddressStr = settings_json["host-adres"];

                if (hostAddressStr == "Any") {
                    settings.server_channel = QHostAddress::Any;
                } else if (hostAddressStr == "LocalHost") {
                    settings.server_channel = QHostAddress::LocalHost;
                } else {
                    throw std::runtime_error("Invalid 'host-adres' value in JSON config: " + hostAddressStr);
                }
            } else {
                throw std::runtime_error("Missing or invalid 'host-adres' in JSON config.");
            }

        } catch (const std::exception& e) {
            throw std::runtime_error("Error parsing JSON: " + std::string(e.what()));
        }
    }


    void Config::LoadToml() {
        try {
            settings.config_toml = toml::parse_file(filePath);

            // Validate & Assign 'server-port'
            if (settings.config_toml.contains("Settings") &&
                settings.config_toml["Settings"]["server-port"].is_integer()) {
                settings.server_port = *settings.config_toml["Settings"]["server-port"].value<int16_t>();
            } else {
                throw std::runtime_error("Missing or invalid 'server-port' in TOML config.");
            }

            // Validate & Convert 'host-adres' to QHostAddress::SpecialAddress
            if (settings.config_toml.contains("Settings") &&
                settings.config_toml["Settings"]["host-adres"].is_string()) {

                std::string hostAddressStr = *settings.config_toml["Settings"]["host-adres"].value<std::string>();

                if (hostAddressStr == "Any") {
                    settings.server_channel = QHostAddress::Any;
                } else if (hostAddressStr == "LocalHost") {
                    settings.server_channel = QHostAddress::LocalHost;
                } else {
                    throw std::runtime_error("Invalid 'host-adres' value in TOML config: " + hostAddressStr);
                }
            } else {
                throw std::runtime_error("Missing or invalid 'host-adres' in TOML config.");
            }

        } catch (const std::exception& e) {
            throw std::runtime_error("Error parsing TOML: " + std::string(e.what()));
        }
    }

    const Config::Settings& Config::GetSettings() const {
        return settings;
    }
