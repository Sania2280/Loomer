#ifndef BUFER_H
#define BUFER_H

#include <QObject>
#include <QByteArray>
class Buf : public QObject
{
    Q_OBJECT

public:
    static Buf& getInstance() {
        static Buf data;
        return data;
    }

    QByteArray bufer;


private:
    Buf() {}
    ~Buf() {}

};

#endif // BUFER_H
