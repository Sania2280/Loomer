#include "m_pack.h"
#include <msgpack.hpp>
#include "message.h"

std::string M_pack::puck(Message &rawData){

    msgpack::sbuffer buffer;
    msgpack::pack(buffer, rawData);
    return std::string(buffer.data(), buffer.size());
}

Message M_pack::unpack(std::string rawData) {

    msgpack::object_handle oh = msgpack::unpack(rawData.data(), rawData.size());
    msgpack::object obj = oh.get();

    Message data;
    obj.convert(data);
    return data;
}
