#ifndef CONFIG_H
#define CONFIG_H
#include <QJsonObject>
#include <QTcpSocket>

class Config {
public:
    Config(){}
    void Read();

    struct Settings {
        QJsonObject config_obj;
        qint16 server_port;
        QHostAddress::SpecialAddress server_channel;
    };

    static Settings settings;

};
#endif // CONFIG_H
