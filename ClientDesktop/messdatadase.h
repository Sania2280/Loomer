#ifndef MESSDATADASE_H
#define MESSDATADASE_H

#include "message.h"
#include <QObject>
#include <QWidget>

#include <qdeadlinetimer.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QFile>

class MessDatadase : public QObject
{
    Q_OBJECT
public:
    MessDatadase();

    struct OpenDataMes
    {
        QString p;
        QString q;
    };

    void CreateDB();
    QJsonObject ReadFile(QString name);
    void SetMes(QString mes, QString nick);
    void SetOpenData(QString resId, QString p, QString q);
    void SetCloseKey(QString resId, QString openKey);
    void SetStatus(QString resId, bool stat);
    void SetOpenKey(QString id, QString openKey);
    void SetSecret(QString id, QString secret);
    QString GetCloseKey(QString resId);
    QString GetP(QString id);
    QString GetMesage(QString sendId);
    QString GetSecret(QString sendId);
    bool GetStat(QString sendId);
    void SetCryptData(QString data, QString sendId);


};

#endif // MESSDATADASE_H
