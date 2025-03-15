#ifndef ENUMS_H
#define ENUMS_H

enum MesageIdentifiers {


    ID_MY      = 02,
    ID_CLIENT  = 03,
    ID_DELETE  = 04,
    MESAGE     = 05,
    LOG        = 06,
    SIGN       = 07,
    LOGIN_SEC  = 8,
    LOGIN_FAIL_NAME = 9,
    LOGIN_FAIL_PASS = 10,
    SIGN_SEC   = 11,
    SIGN_FAIL  = 12,
    SIGN_FAIL_EXIST = 13,
    CLIENT_READY_TO_WORCK = 14


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

#endif // ENUMS_H
