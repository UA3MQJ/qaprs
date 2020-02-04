/****************************************************************************
** Meta object code from reading C++ file 'startwindow.h'
**
** Created: Fri 15. Apr 09:56:39 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/startwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'startwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StartWindowImpl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x0a,
      42,   17,   16,   16, 0x0a,
      59,   16,   16,   16, 0x0a,
      69,   16,   16,   16, 0x0a,
      83,   79,   16,   16, 0x0a,
     100,   16,   16,   16, 0x0a,
     114,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StartWindowImpl[] = {
    "StartWindowImpl\0\0checked\0cb1clicked(bool)\0"
    "cb2clicked(bool)\0goClick()\0allLoad()\0"
    "msg\0readMsg(QString)\0socketError()\0"
    "timeout()\0"
};

const QMetaObject StartWindowImpl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_StartWindowImpl,
      qt_meta_data_StartWindowImpl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StartWindowImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StartWindowImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StartWindowImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StartWindowImpl))
        return static_cast<void*>(const_cast< StartWindowImpl*>(this));
    if (!strcmp(_clname, "Ui::StartWindow"))
        return static_cast< Ui::StartWindow*>(const_cast< StartWindowImpl*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int StartWindowImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cb1clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: cb2clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: goClick(); break;
        case 3: allLoad(); break;
        case 4: readMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: socketError(); break;
        case 6: timeout(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
