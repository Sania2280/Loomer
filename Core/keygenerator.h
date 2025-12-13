#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QObject>

class KeyGenerator :QObject
{
    Q_OBJECT
public:
    KeyGenerator();

    std::vector<unsigned char> GetKey();
    QString GetOpenKeyOrSecret(QString a, QString closeKey, QString p);

};

#endif // KEYGENERATOR_H
