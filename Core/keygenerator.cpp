#include "keygenerator.h"
#include "sodium.h"

KeyGenerator::KeyGenerator() {}

std::vector<unsigned char> KeyGenerator::GetKey()
{
    std::vector<unsigned char> buffer(256);
    randombytes_buf(buffer.data(), static_cast<size_t>(buffer.size()));
    return buffer;
}
