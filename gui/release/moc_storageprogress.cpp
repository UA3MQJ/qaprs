/****************************************************************************
** Meta object code from reading C++ file 'storageprogress.h'
**
** Created: Fri 15. Apr 09:56:08 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/storageprogress.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'storageprogress.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StorageProgressDialog[] = {

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
      27,   23,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_StorageProgressDialog[] = {
    "StorageProgressDialog\0\0msg\0"
    "storeProgress(QString)\0"
};

const QMetaObject StorageProgressDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_StorageProgressDialog,
      qt_meta_data_StorageProgressDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StorageProgressDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StorageProgressDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StorageProgressDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StorageProgressDialog))
        return static_cast<void*>(const_cast< StorageProgressDialog*>(this));
    if (!strcmp(_clname, "Ui::DialogStorageProgress"))
        return static_cast< Ui::DialogStorageProgress*>(const_cast< StorageProgressDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int StorageProgressDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: storeProgress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
