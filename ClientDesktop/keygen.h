#ifndef KEYGEN_H
#define KEYGEN_H

#include <QObject>
#include <QWidget>


class KeyGen
{
public:
    KeyGen();

    static std::vector<unsigned char> KeyGenerator();
    static std::vector<unsigned char> GenerateP();
    static std::vector<unsigned char> GenerateQ();

private:
};

#endif // KEYGEN_H
