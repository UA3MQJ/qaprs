/****************************************************************************
** Meta object code from reading C++ file 'qsysstationlist.h'
**
** Created: Fri 15. Apr 09:56:26 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../core/include/qsysstationlist.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsysstationlist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QAPRSstationList[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   51,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSstationList[] = {
    "QAPRSstationList\0\0call\0"
    "SIGstationSetParam(QString)\0"
    "call,par_name,par_val\0"
    "stationSetParam(QString,QString,QString)\0"
};

const QMetaObject QAPRSstationList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QAPRSstationList,
      qt_meta_data_QAPRSstationList, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSstationList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSstationList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSstationList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSstationList))
        return static_cast<void*>(const_cast< QAPRSstationList*>(this));
    return QObject::qt_metacast(_clname);
}

int QAPRSstationList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SIGstationSetParam((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: stationSetParam((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QAPRSstationList::SIGstationSetParam(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
