/****************************************************************************
** Meta object code from reading C++ file 'qsyspackets.h'
**
** Created: Fri 15. Apr 09:56:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../core/include/qsyspackets.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsyspackets.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QAPRSPacketList[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      58,   17,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSPacketList[] = {
    "QAPRSPacketList\0\0"
    "K,DT,Port_Num,TRX,PTo,PFrom,PVia,Message\0"
    "addPacket(int,QString,int,QString,QString,QString,QString,QString)\0"
};

const QMetaObject QAPRSPacketList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QAPRSPacketList,
      qt_meta_data_QAPRSPacketList, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSPacketList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSPacketList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSPacketList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSPacketList))
        return static_cast<void*>(const_cast< QAPRSPacketList*>(this));
    return QObject::qt_metacast(_clname);
}

int QAPRSPacketList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
