#ifndef RECONNECTION_H
#define RECONNECTION_H
#include <QTcpSocket>
#include <QObject>

class RegWindow;

class Reconnection : public QObject
{
    Q_OBJECT

public:

    static Reconnection& getInterface(){
        static Reconnection rec;
        return rec;
    }

    QTcpSocket* getSocket();

    void setRegwind(RegWindow* regWind);
    void createSocket();

    static QTcpSocket* socket;
    static bool Close_Window_stat;
    // static bool regWindExe;
    // static bool mainWindExe;


private slots:

    void onError();
    void onConnected();
    void tryReconnect();
    void onDisconnected();

    void setConnection();

private:

    explicit Reconnection(QObject* parent = nullptr) ;
    Q_DISABLE_COPY(Reconnection)

    RegWindow* regWindow = nullptr;

};

#endif // RECONNECTION_H
