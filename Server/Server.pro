QT = core
QT += core network

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=C:/msgpack/msgpack-c/include
INCLUDEPATH += C:/boost_1_87_0


SOURCES += \
        main.cpp \
        sending.cpp \
        server.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    config.h \
    enums.h \
    m_pack.h \
    sending.h \
    server.h

DISTFILES += \
    .gitignore
