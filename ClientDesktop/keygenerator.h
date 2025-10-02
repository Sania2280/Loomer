#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QObject>
#include <QWidget>

#include "sodium.h"

class KeyGenerator : public QObject
{
    Q_OBJECT
public:
    KeyGenerator();

    std::vector<unsigned char> GetKey();
};

#endif // KEYGENERATOR_H
