#ifndef RECONNECTION_H
#define RECONNECTION_H

#include "enums.h"
#include <QTcpSocket>
#include <QObject>

class RegWindow;
class MainWindow;

class Reconnection : public QObject
{
    Q_OBJECT

public:


    static Reconnection* getInterface(){
        static Reconnection rec;
        return &rec;
    }

    QTcpSocket* getSocket();

    void setRegwind(RegWindow* regWind);
    void setMainWind(MainWindow* mainWin);
    void createSocket();

    static QTcpSocket* socket;
    static bool Close_Window_stat;
    // static bool regWindExe;
    // static bool mainWindExe;

    void connections();

private slots:

    void onError();
    void onConnected();
    void tryReconnect();
    void onDisconnected();

    void setConnection();
    void slotReadyRed();

private:

    explicit Reconnection(QObject* parent = nullptr) ;
    Q_DISABLE_COPY(Reconnection)

    RegWindow* regWindow = nullptr;
    MainWindow* mainWind = nullptr;



signals:
    void mainWindSocketPrint();
    void mainWindSocketDelete(QString socket_to_delete);
    void mainWindPrintMessage(QString message);
    void regWindErrorWorker(MesageIdentifiers id);
    void regWindConnectionStatWorker(ConnectionStat stat);
};

#endif // RECONNECTION_H
