#ifndef DIFFIEHELLMAN_H
#define DIFFIEHELLMAN_H

#include "datasender.h"
#include <QObject>


class DiffieHellman : public QObject
{
    Q_OBJECT
public:
    DiffieHellman();

    void ShearingKeys(QString nick);
private:

    DataSender* dataSendlerPtr = nullptr;

signals:

    void SendOpenKey(QString id, std::vector<unsigned char> openKey);
};

#endif // DIFFIEHELLMAN_H
