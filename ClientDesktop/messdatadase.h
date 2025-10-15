#ifndef MESSDATADASE_H
#define MESSDATADASE_H

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

    void CreateDB();
    QJsonObject ReadFile(QString name);
    void SetMes(QString mes, QString nick);
};

#endif // MESSDATADASE_H
