/****************************************************************************
** Meta object code from reading C++ file 'portoption.h'
**
** Created: Fri 15. Apr 09:56:12 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/portoption.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'portoption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PortoptionDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   18,   17,   17, 0x05,
      73,   68,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   18,   17,   17, 0x0a,
     123,   17,   17,   17, 0x08,
     141,   17,   17,   17, 0x08,
     166,  160,   17,   17, 0x08,
     185,  160,   17,   17, 0x08,
     203,   17,   17,   17, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_PortoptionDialog[] = {
    "PortoptionDialog\0\0pNum,pName,pVal\0"
    "setPortParam(int,QString,QString)\0"
    "pNum\0updatePort(int)\0"
    "RCVPortParam(int,QString,QString)\0"
    "saveButtonClick()\0closeButtonClick()\0"
    "index\0KISSEasySetup(int)\0onProxyCheck(int)\0"
    "onProxyCheck()\0"
};

const QMetaObject PortoptionDialog::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PortoptionDialog,
      qt_meta_data_PortoptionDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PortoptionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PortoptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PortoptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PortoptionDialog))
        return static_cast<void*>(const_cast< PortoptionDialog*>(this));
    if (!strcmp(_clname, "Ui::DialogPortoption"))
        return static_cast< Ui::DialogPortoption*>(const_cast< PortoptionDialog*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PortoptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setPortParam((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: updatePort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: RCVPortParam((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: saveButtonClick(); break;
        case 4: closeButtonClick(); break;
        case 5: KISSEasySetup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: onProxyCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: onProxyCheck(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void PortoptionDialog::setPortParam(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PortoptionDialog::updatePort(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
