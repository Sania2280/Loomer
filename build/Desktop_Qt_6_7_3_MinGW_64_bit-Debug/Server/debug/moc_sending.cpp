/****************************************************************************
** Meta object code from reading C++ file 'sending.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Server/sending.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sending.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSSendingENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSendingENDCLASS = QtMocHelpers::stringData(
    "Sending",
    "Get_New_Client",
    "",
    "QTcpSocket*",
    "socet",
    "QList<QTcpSocket*>",
    "Sockets_reciverd",
    "Get_Disconnected_Client",
    "qintptr",
    "socket",
    "IP",
    "sendToSocket",
    "message"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSendingENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   32,    2, 0x0a,    1 /* Public */,
       7,    2,   37,    2, 0x0a,    4 /* Public */,
      11,    2,   42,    2, 0x0a,    7 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString,    9,   10,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    9,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject Sending::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_CLASSSendingENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSendingENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSendingENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Sending, std::true_type>,
        // method 'Get_New_Client'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTcpSocket *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QList<QTcpSocket*>, std::false_type>,
        // method 'Get_Disconnected_Client'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qintptr, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'sendToSocket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTcpSocket *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void Sending::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Sending *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Get_New_Client((*reinterpret_cast< std::add_pointer_t<QTcpSocket*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<QTcpSocket*>>>(_a[2]))); break;
        case 1: _t->Get_Disconnected_Client((*reinterpret_cast< std::add_pointer_t<qintptr>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->sendToSocket((*reinterpret_cast< std::add_pointer_t<QTcpSocket*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QTcpSocket*> >(); break;
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
    }
}

const QMetaObject *Sending::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Sending::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSendingENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Sending::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
