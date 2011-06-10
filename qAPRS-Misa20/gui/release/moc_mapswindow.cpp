/****************************************************************************
** Meta object code from reading C++ file 'mapswindow.h'
**
** Created: Fri 15. Apr 09:56:28 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/mapswindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapswindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MvScene[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,    9,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MvScene[] = {
    "MvScene\0\0point\0onMouseMove(QPointF)\0"
};

const QMetaObject MvScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_MvScene,
      qt_meta_data_MvScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MvScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MvScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MvScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MvScene))
        return static_cast<void*>(const_cast< MvScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int MvScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onMouseMove((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MvScene::onMouseMove(QPointF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_MapsWindowImpl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      30,   15,   15,   15, 0x08,
      45,   15,   15,   15, 0x08,
      57,   15,   15,   15, 0x08,
      94,   72,   15,   15, 0x08,
     121,   15,   15,   15, 0x08,
     131,   72,   15,   15, 0x08,
     160,  155,   15,   15, 0x08,
     190,  184,   15,   15, 0x08,
     211,  155,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MapsWindowImpl[] = {
    "MapsWindowImpl\0\0zoomInClick()\0"
    "zoomOutClick()\0zoomInOut()\0loadMapClick()\0"
    "mapInfFile,mapImgFile\0selectMap(QString,QString)\0"
    "drawMap()\0setMap(QString,QString)\0"
    "Call\0RCVStationInfo(QString)\0point\0"
    "onMouseMove(QPointF)\0updateStationOnMap(QString)\0"
};

const QMetaObject MapsWindowImpl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MapsWindowImpl,
      qt_meta_data_MapsWindowImpl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapsWindowImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapsWindowImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapsWindowImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapsWindowImpl))
        return static_cast<void*>(const_cast< MapsWindowImpl*>(this));
    if (!strcmp(_clname, "Ui::MapsWindow"))
        return static_cast< Ui::MapsWindow*>(const_cast< MapsWindowImpl*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MapsWindowImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: zoomInClick(); break;
        case 1: zoomOutClick(); break;
        case 2: zoomInOut(); break;
        case 3: loadMapClick(); break;
        case 4: selectMap((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: drawMap(); break;
        case 6: setMap((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: RCVStationInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: onMouseMove((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 9: updateStationOnMap((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
