#ifndef RECONNECTION_H
#define RECONNECTION_H

#include <QObject>
#include <QTcpSocket>

class RegWindow; // forward declaration

class Reconnection : public QObject
{
    Q_OBJECT

public:
    explicit Reconnection(QTcpSocket* socket, RegWindow* rWindow = nullptr);


    void SetUpConnection();

    bool Close_Window_stat = false;

private:
    QTcpSocket* m_socket;
    RegWindow* m_regWind;

    void onConnected();
    void onError();
    void onDisconnected();
};

#endif // RECONNECTION_H
