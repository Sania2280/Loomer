#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#include <QObject>
#include <QMap>

class KeyStorage : public QObject
{
    Q_OBJECT
public:

    static KeyStorage& getInstance(){
        static KeyStorage storage;
        return storage;

    }

    struct KeysStruct
    {
        std::vector<unsigned char> openKey;
        std::vector<unsigned char> closeKey;
        std::vector<unsigned char> secret;
    };

    QMap<QString, KeysStruct> Keys;

private:
    KeyStorage() {};
    ~KeyStorage() {};

};

#endif // KEYSTORAGE_H
