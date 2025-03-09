#include "Mpack.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wshadow"

#include <msgpack.hpp>

#pragma GCC diagnostic pop

QString Mpack::unpack(QByteArray rawData) {
    msgpack::object_handle oh = msgpack::unpack(rawData.constData(), rawData.size());
    msgpack::object obj = oh.get();
    QString data = QString::fromStdString(obj.as<std::string>());
    return data;
}

std::string Mpack::puck(QString rawData){
    std::string msg = rawData.toStdString();
    msgpack::sbuffer buffer;
    msgpack::pack(buffer, msg);
   return std::string(buffer.data(), buffer.size());
}
