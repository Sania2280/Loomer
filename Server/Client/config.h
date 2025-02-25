#ifndef CONFIG_H
#define CONFIG_H

#include "mainwindow.h"
#include <QJsonObject>

class Config {
private:
    MainWindow *mainWindow;

public:
    Config(){}
    void Read();

     Config(MainWindow *mw) : mainWindow(mw) {}

    struct Settings {
        QJsonObject config_obj;
        qint16 server_port;
        QString server_ip;
    };

    static Settings settings;

};
#endif // CONFIG_H
