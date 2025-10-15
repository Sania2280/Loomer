#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QObject>

class KeyGenerator :QObject
{
    Q_OBJECT
public:
    KeyGenerator();

    std::vector<unsigned char> GetKey();
};

#endif // KEYGENERATOR_H
