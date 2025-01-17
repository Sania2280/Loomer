/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Server/server.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSserverENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSserverENDCLASS = QtMocHelpers::stringData(
    "server",
    "newClientConnected",
    "",
    "QTcpSocket*",
    "socet",
    "QList<QTcpSocket*>&",
    "Sockets",
    "disconnectedClient",
    "qintptr",
    "IP",
    "sendingMesage",
    "socket",
    "message",
    "slotsReadyRead"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSserverENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   38,    2, 0x06,    1 /* Public */,
       7,    2,   43,    2, 0x06,    4 /* Public */,
      10,    2,   48,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    0,   53,    2, 0x08,   10 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString,    4,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,   11,   12,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject server::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_CLASSserverENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSserverENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSserverENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<server, std::true_type>,
        // method 'newClientConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTcpSocket *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QList<QTcpSocket*> &, std::false_type>,
        // method 'disconnectedClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qintptr, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'sendingMesage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTcpSocket *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'slotsReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<server *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newClientConnected((*reinterpret_cast< std::add_pointer_t<QTcpSocket*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<QTcpSocket*>&>>(_a[2]))); break;
        case 1: _t->disconnectedClient((*reinterpret_cast< std::add_pointer_t<qintptr>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->sendingMesage((*reinterpret_cast< std::add_pointer_t<QTcpSocket*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->slotsReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTcpSocket* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (server::*)(QTcpSocket * , QList<QTcpSocket*> & );
            if (_t _q_method = &server::newClientConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (server::*)(qintptr , QString );
            if (_t _q_method = &server::disconnectedClient; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (server::*)(QTcpSocket * , const QString & );
            if (_t _q_method = &server::sendingMesage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSserverENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void server::newClientConnected(QTcpSocket * _t1, QList<QTcpSocket*> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void server::disconnectedClient(qintptr _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void server::sendingMesage(QTcpSocket * _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
