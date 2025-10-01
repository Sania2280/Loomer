#ifndef KEYGEN_H
#define KEYGEN_H

#include <QObject>
#include <QWidget>

class KeyGen
{
public:
    KeyGen();

    static std::vector<unsigned char> KeyGenerator();
private:
};

#endif // KEYGEN_H
