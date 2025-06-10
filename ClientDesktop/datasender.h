#ifndef DATASENDER_H
#define DATASENDER_H

#include "enums.h"
#include "message.h"
#include <QString>
#include <QObject>
#include <QTcpSocket>



class DataSender : public QObject
{
    Q_OBJECT

public:
    explicit DataSender(QObject* parent = nullptr);

    void SendMyData(MesageIdentifiers status);
    void SendMyMasseg(Message message);

};

#endif // DATASENDER_H
