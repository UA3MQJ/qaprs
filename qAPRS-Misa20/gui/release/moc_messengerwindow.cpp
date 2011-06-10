/****************************************************************************
** Meta object code from reading C++ file 'messengerwindow.h'
**
** Created: Fri 15. Apr 09:56:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/messengerwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messengerwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MessengerWindowImpl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      57,   21,   20,   20, 0x0a,
     120,   21,   20,   20, 0x0a,
     200,  183,   20,   20, 0x0a,
     244,   20,   20,   20, 0x0a,
     260,   20,   20,   20, 0x0a,
     277,   20,   20,   20, 0x0a,
     287,   20,   20,   20, 0x0a,
     307,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MessengerWindowImpl[] = {
    "MessengerWindowImpl\0\0"
    "pNum,K,DateTime,To,From,MsgText,MID\0"
    "RXAPRSMessage(int,int,QString,QString,QString,QString,QString)\0"
    "TXAPRSMessage(int,int,QString,QString,QString,QString,QString)\0"
    "current,previous\0"
    "callCurrentChanged(QModelIndex,QModelIndex)\0"
    "toFromChanged()\0msgTextChanged()\0"
    "msgSend()\0updateStationList()\0"
    "updateMessageList()\0"
};

const QMetaObject MessengerWindowImpl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MessengerWindowImpl,
      qt_meta_data_MessengerWindowImpl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MessengerWindowImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MessengerWindowImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MessengerWindowImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessengerWindowImpl))
        return static_cast<void*>(const_cast< MessengerWindowImpl*>(this));
    if (!strcmp(_clname, "Ui::MessengerWindow"))
        return static_cast< Ui::MessengerWindow*>(const_cast< MessengerWindowImpl*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MessengerWindowImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: RXAPRSMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 1: TXAPRSMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 2: callCurrentChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 3: toFromChanged(); break;
        case 4: msgTextChanged(); break;
        case 5: msgSend(); break;
        case 6: updateStationList(); break;
        case 7: updateMessageList(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
