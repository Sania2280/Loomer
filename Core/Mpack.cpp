#include "Mpack.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wshadow"

#include <msgpack.hpp>

#pragma GCC diagnostic pop

std::string Mpack::puck(Message &rawData){

    msgpack::sbuffer buffer;
    msgpack::pack(buffer, rawData);
    return std::string(buffer.data(), buffer.size());
}

Message Mpack::unpack(std::string rawData) {

    msgpack::object_handle oh = msgpack::unpack(rawData.data(), rawData.size());
    msgpack::object obj = oh.get();

    Message data;
    obj.convert(data);
    return data;
}
