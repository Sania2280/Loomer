#ifndef USERDATA_H
#define USERDATA_H
#include "enums.h"
#include <QString>
#include <QTcpSocket>

class UserData : public QObject
{
    Q_OBJECT

public:

    static UserData& getInstance() {
        static UserData data;
        return data;
    }

     QString name ;
     QString pass;
     QString e_male;
     QString desck;
     RegWind status;
     bool mainWindStarted;


     void setSocket(QTcpSocket* socket) ;
     QTcpSocket* getSocket() const { return UserSocket; }

private:
    UserData() {};
    ~UserData(){};

    QTcpSocket* UserSocket = nullptr;

signals:
    void socketSet(QTcpSocket* socket);
};

#endif // USERDATA_H
