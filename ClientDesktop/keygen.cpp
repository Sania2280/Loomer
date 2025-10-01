#include "keygen.h"
#include "sodium/randombytes.h"


KeyGen::KeyGen() {}

std::vector<unsigned char> KeyGen::KeyGenerator()
{
    std::vector<unsigned char> buffer(256);
    randombytes_buf(buffer.data(), static_cast<size_t>(buffer.size()));

    return buffer;
}
