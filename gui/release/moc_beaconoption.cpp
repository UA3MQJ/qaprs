/****************************************************************************
** Meta object code from reading C++ file 'beaconoption.h'
**
** Created: Fri 15. Apr 09:56:14 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/beaconoption.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'beaconoption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BeaconoptionDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   20,   19,   19, 0x05,
      77,   72,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      95,   20,   19,   19, 0x0a,
     131,   19,   19,   19, 0x08,
     149,   19,   19,   19, 0x08,
     168,   19,   19,   19, 0x08,
     185,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BeaconoptionDialog[] = {
    "BeaconoptionDialog\0\0bNum,pName,pVal\0"
    "setBeaconParam(int,QString,QString)\0"
    "bNum\0updateBeacon(int)\0"
    "RCVBeaconParam(int,QString,QString)\0"
    "saveButtonClick()\0closeButtonClick()\0"
    "mulButtonClick()\0mul2ButtonClick()\0"
};

const QMetaObject BeaconoptionDialog::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BeaconoptionDialog,
      qt_meta_data_BeaconoptionDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BeaconoptionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BeaconoptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BeaconoptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BeaconoptionDialog))
        return static_cast<void*>(const_cast< BeaconoptionDialog*>(this));
    if (!strcmp(_clname, "Ui::DialogBeaconoption"))
        return static_cast< Ui::DialogBeaconoption*>(const_cast< BeaconoptionDialog*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BeaconoptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setBeaconParam((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: updateBeacon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: RCVBeaconParam((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: saveButtonClick(); break;
        case 4: closeButtonClick(); break;
        case 5: mulButtonClick(); break;
        case 6: mul2ButtonClick(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void BeaconoptionDialog::setBeaconParam(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BeaconoptionDialog::updateBeacon(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
