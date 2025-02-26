#ifndef M_PACK_H
#define M_PACK_H
#include <QString>

class M_pack{
public:
    QString unpack(QByteArray rawData);
    std::string puck(QString rawData);
};

#endif // M_PACK_H
