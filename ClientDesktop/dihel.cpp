#include "dihel.h"
#include "keygenerator.h"
#include "messdatadase.h"
// #include <boost/multiprecision/cpp_int.hpp>

DiHel::DiHel() {}

void DiHel::CloseKeyGen(QString resId)
{
    KeyGenerator gen;
    std::vector<unsigned char> closeKey =  gen.GetKey();

    MessDatadase mesDB;

    QString closeKeyStr;

    for (size_t i = 0; i < closeKey.size(); i++) {
        closeKeyStr += QString::number(closeKey[i]);
    }

    mesDB.SetCloseKey(resId, closeKeyStr);
}

void DiHel::OpenKeyGen(QString resId)
{
    qDebug() << resId;
}
