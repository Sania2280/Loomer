#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H

#include "enums.h"
#include <QString>
#include <QObject>
#include <QTcpSocket>

class RegWindow;

class ServerConnector : public QObject
{
    Q_OBJECT  // Убираем точку с запятой

public:
    explicit ServerConnector(QObject* parent = nullptr, RegWindow *rWindow = nullptr);

    void ConnectToServer();
    void SendMyData(MesageIdentifiers status);



private:
    QTcpSocket *socket;
    RegWindow *regWind;
    bool Close_Window_stat = false;



signals:
    void socketConnected();         // Сигнал о подключении

public slots:

    void SetUpConnection();

private slots:
    void slotReadyRead();           // Слот для чтения данных из сокета

    void onConnected();
    void onError(QAbstractSocket::SocketError error);
    void onDisconnected();

};

#endif // SERVERCONNECTOR_H
