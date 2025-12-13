#include "aes.h"
#include <openssl/evp.h>
#include <openssl/rand.h>


// ===== Шифрование строки =====
QByteArray AES::keyToBytes(const QString &keyStr)
{
    QByteArray keyBytes = keyStr.toUtf8();

    // AES-256 требует ровно 32 байта
    if (keyBytes.size() < 32) keyBytes.append(QByteArray(32 - keyBytes.size(), 0));
    if (keyBytes.size() > 32) keyBytes = keyBytes.left(32);

    return keyBytes;
}

QString AES::encryptString(const QString &plaintext, const QString &keyStr)
{
    QByteArray key = keyToBytes(keyStr);

    // IV = нули
    QByteArray iv(16, 0);
    QByteArray plainBytes = plaintext.toUtf8();
    QByteArray cipherBytes = crypt(plainBytes, key);
    return QString::fromUtf8(cipherBytes.toBase64());
}

QString AES::decryptString(const QString &ciphertextBase64, const QString &keyStr)
{
    QByteArray key = keyToBytes(keyStr);
    QByteArray cipherBytes = QByteArray::fromBase64(ciphertextBase64.toUtf8());
    QByteArray plainBytes = decrypt(cipherBytes, key);
    return QString::fromUtf8(plainBytes);
}

QByteArray AES::crypt(const QByteArray &plaintext, const QByteArray &key)
{
    QByteArray ciphertext;
    ciphertext.resize(plaintext.size() + EVP_MAX_BLOCK_LENGTH);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, ciphertext_len = 0;

    // IV = нули
    QByteArray iv(16, 0);

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char *>(key.constData()),
                       reinterpret_cast<const unsigned char *>(iv.constData()));

    EVP_EncryptUpdate(ctx,
                      reinterpret_cast<unsigned char *>(ciphertext.data()),
                      &len,
                      reinterpret_cast<const unsigned char *>(plaintext.constData()),
                      static_cast<int>(plaintext.size()));
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx,
                        reinterpret_cast<unsigned char *>(ciphertext.data()) + len,
                        &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

QByteArray AES::decrypt(const QByteArray &ciphertext, const QByteArray &key)
{
    QByteArray plaintext;
    plaintext.resize(ciphertext.size());

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, plaintext_len = 0;

    QByteArray iv(16, 0);

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char *>(key.constData()),
                       reinterpret_cast<const unsigned char *>(iv.constData()));

    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char *>(plaintext.data()),
                      &len,
                      reinterpret_cast<const unsigned char *>(ciphertext.constData()),
                      static_cast<int>(ciphertext.size()));
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx,
                        reinterpret_cast<unsigned char *>(plaintext.data()) + len,
                        &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return plaintext;
}
