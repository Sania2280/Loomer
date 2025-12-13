#ifndef AES_H
#define AES_H

#include <QObject>

class AES : QObject
{
    Q_OBJECT
public:
    AES(){};
    QString encryptString(const QString &plaintext, const QString &keyStr);

    // Дешифрование Base64 -> возвращает исходную строку
    QString decryptString(const QString &ciphertextBase64, const QString &keyStr);

private:
    QByteArray crypt(const QByteArray &plaintext, const QByteArray &key);
    QByteArray decrypt(const QByteArray &ciphertext, const QByteArray &key);

    QByteArray keyToBytes(const QString &keyStr);
};

#endif // AES_H
