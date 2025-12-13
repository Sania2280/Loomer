#include "keygenerator.h"
#include "sodium.h"
#include "openssl/bn.h"

KeyGenerator::KeyGenerator() {}

std::vector<unsigned char> KeyGenerator::GetKey()
{
    std::vector<unsigned char> buffer(64);
    randombytes_buf(buffer.data(), static_cast<size_t>(buffer.size()));
    return buffer;
}

QString KeyGenerator::GetOpenKeyOrSecret(QString a, QString closeKey, QString p )
{
    BN_CTX* ctx = BN_CTX_new();
    std::string closeKeyStr = closeKey.toStdString();
    std::string pstr = p.toStdString();
    std::string qstp = a.toStdString();
    BIGNUM* clKey = BN_new();
    BIGNUM* opKey = BN_new();
    BIGNUM* P = BN_new();
    BIGNUM* A = BN_new();
    BIGNUM* res = BN_new();

    BN_dec2bn(&clKey, closeKeyStr.c_str());
    BN_dec2bn(&P, pstr.c_str());
    BN_dec2bn(&A, qstp.c_str());

    BN_mod_exp(res, A, clKey, P, ctx);

    char* charKey = BN_bn2dec(res);
    QString result = QString::fromUtf8(charKey);


    BN_free(clKey); BN_free(opKey); BN_free(P); BN_free(res); BN_CTX_free(ctx);

    return result;
}
