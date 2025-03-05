#include <QCoreApplication>
#include <iostream>
#include "server.h"
#include "sending.h"
#include "Config.hpp"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    std::string configPath = "./config.json"; // Значення за замовчуванням

    // Обробка аргументів командного рядка
    if (argc > 1) {
        std::string arg = argv[1];

        if (arg == "--config.json") {
            configPath = "./config.json";
        } else if (arg == "--config.toml") {
            configPath = "./config.toml";
        } else {
            std::cerr << "Invalid argument! Use --config.json or --config.toml" << std::endl;
            return 1; // Вихід з помилкою
        }
    }

    std::cout << "Using config: " << configPath << std::endl;

    Config config{configPath};
    config.Load();

    server s{config.GetSettings()};  // Створюємо сервер
    Sending sendObj(&s);

    sendObj.start();
    s.setSending(sendObj);

    return a.exec();
}
