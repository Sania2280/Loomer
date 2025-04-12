#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H

#include "enums.h"
#include <QString>
#include <QObject>
#include <QTcpSocket>

class RegWindow;

class ServerConnector : public QObject
{
    Q_OBJECT

public:
    explicit ServerConnector(QObject* parent = nullptr, RegWindow *rWindow = nullptr);

    void ConnectToServer();
    void SendMyData(MesageIdentifiers status);



private:
    QTcpSocket *socket;
    RegWindow *regWind;
    bool Close_Window_stat = false;
    QByteArray buffer;



signals:
    void socketConnected();         // Сигнал о подключении

public slots:

    void SetUpConnection();

    void slotReadyRead();           // Слот для чтения данных из сокета

private slots:

    void onError();
    void onConnected();
    void onDisconnected();

};

#endif // SERVERCONNECTOR_H
