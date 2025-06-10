#ifndef ENUMS_H
#define ENUMS_H

#include <qobject.h>
enum class MesageIdentifiers {
    NONE,
    ID_MY,
    ID_CLIENT,
    ID_DELETE,
    MESAGE,
    LOG,
    SIGN,
    LOGIN_SEC,
    LOGIN_FAIL_NAME,
    LOGIN_FAIL_PASS,
    SIGN_SEC,
    SIGN_FAIL,
    SIGN_FAIL_EXIST,
    CLIENT_READY_TO_WORCK,
    RECONNECTION
};

enum class Directorys {
    IMAGED ,
    STYLES ,
    CONFIG
};

enum class Files {
    USER ,
    BUTON ,
    STYLE ,
    ITEM,
    NON
};

enum class RegWind {
    LOG,
    SIGN
};

enum class ConnectionStat{
    CONNECTED,
    DISCONNECTED
};

Q_DECLARE_METATYPE(MesageIdentifiers)

#endif // ENUMS_H
