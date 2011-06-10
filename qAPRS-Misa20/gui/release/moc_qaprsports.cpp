/****************************************************************************
** Meta object code from reading C++ file 'qaprsports.h'
**
** Created: Fri 15. Apr 09:55:58 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../core/include/qaprsports.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qaprsports.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QAPRSPort[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   11,   10,   10, 0x05,
      71,   50,   10,   10, 0x05,
     113,   11,   10,   10, 0x05,
     140,   50,   10,   10, 0x05,
     193,  182,   10,   10, 0x05,
     224,  214,   10,   10, 0x05,
     253,  243,   10,   10, 0x05,
     282,  272,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSPort[] = {
    "QAPRSPort\0\0pNum,Packet\0"
    "RXAXPacket(int,QByteArray)\0"
    "pNum,To,From,MsgText\0"
    "RXAPRSPacket(int,QString,QString,QString)\0"
    "TXAXPacket(int,QByteArray)\0"
    "TXAPRSPacket(int,QString,QString,QString)\0"
    "pNum,state\0changeState(int,int)\0"
    "pNum,isRX\0RXPacket(int,bool)\0pNum,isTX\0"
    "TXPacket(int,bool)\0pNum,busy\0"
    "changeBUSYState(int,bool)\0"
};

const QMetaObject QAPRSPort::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QAPRSPort,
      qt_meta_data_QAPRSPort, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSPort::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSPort))
        return static_cast<void*>(const_cast< QAPRSPort*>(this));
    return QObject::qt_metacast(_clname);
}

int QAPRSPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: RXAXPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 1: RXAPRSPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: TXAXPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 3: TXAPRSPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 4: changeState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: RXPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: TXPacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: changeBUSYState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QAPRSPort::RXAXPacket(int _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QAPRSPort::RXAPRSPacket(int _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QAPRSPort::TXAXPacket(int _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QAPRSPort::TXAPRSPacket(int _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QAPRSPort::changeState(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QAPRSPort::RXPacket(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QAPRSPort::TXPacket(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QAPRSPort::changeBUSYState(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
static const uint qt_meta_data_QAPRSAXIPPORT[] = {

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
      15,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSAXIPPORT[] = {
    "QAPRSAXIPPORT\0\0readPendingDatagrams()\0"
};

const QMetaObject QAPRSAXIPPORT::staticMetaObject = {
    { &QAPRSPort::staticMetaObject, qt_meta_stringdata_QAPRSAXIPPORT,
      qt_meta_data_QAPRSAXIPPORT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSAXIPPORT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSAXIPPORT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSAXIPPORT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSAXIPPORT))
        return static_cast<void*>(const_cast< QAPRSAXIPPORT*>(this));
    return QAPRSPort::qt_metacast(_clname);
}

int QAPRSAXIPPORT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAPRSPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readPendingDatagrams(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_QAPRSINTERNETPORT[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      40,   18,   18,   18, 0x08,
      76,   64,   18,   18, 0x08,
     121,   18,   18,   18, 0x08,
     137,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSINTERNETPORT[] = {
    "QAPRSINTERNETPORT\0\0tcpClientconnected()\0"
    "tcpClientdisconnected()\0socketError\0"
    "tcpClienterror(QAbstractSocket::SocketError)\0"
    "tcpClientread()\0onReconnectTimer()\0"
};

const QMetaObject QAPRSINTERNETPORT::staticMetaObject = {
    { &QAPRSPort::staticMetaObject, qt_meta_stringdata_QAPRSINTERNETPORT,
      qt_meta_data_QAPRSINTERNETPORT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSINTERNETPORT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSINTERNETPORT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSINTERNETPORT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSINTERNETPORT))
        return static_cast<void*>(const_cast< QAPRSINTERNETPORT*>(this));
    return QAPRSPort::qt_metacast(_clname);
}

int QAPRSINTERNETPORT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAPRSPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: tcpClientconnected(); break;
        case 1: tcpClientdisconnected(); break;
        case 2: tcpClienterror((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: tcpClientread(); break;
        case 4: onReconnectTimer(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_QAPRSAGWPORT[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      35,   13,   13,   13, 0x08,
      71,   59,   13,   13, 0x08,
     116,   13,   13,   13, 0x08,
     132,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSAGWPORT[] = {
    "QAPRSAGWPORT\0\0tcpClientconnected()\0"
    "tcpClientdisconnected()\0socketError\0"
    "tcpClienterror(QAbstractSocket::SocketError)\0"
    "tcpClientread()\0onReconnectTimer()\0"
};

const QMetaObject QAPRSAGWPORT::staticMetaObject = {
    { &QAPRSPort::staticMetaObject, qt_meta_stringdata_QAPRSAGWPORT,
      qt_meta_data_QAPRSAGWPORT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSAGWPORT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSAGWPORT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSAGWPORT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSAGWPORT))
        return static_cast<void*>(const_cast< QAPRSAGWPORT*>(this));
    return QAPRSPort::qt_metacast(_clname);
}

int QAPRSAGWPORT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAPRSPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: tcpClientconnected(); break;
        case 1: tcpClientdisconnected(); break;
        case 2: tcpClienterror((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: tcpClientread(); break;
        case 4: onReconnectTimer(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_QAPRSKISSPORT[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      35,   32,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSKISSPORT[] = {
    "QAPRSKISSPORT\0\0onComReadTimer()\0,,\0"
    "onComRead(QString,QString,QString)\0"
};

const QMetaObject QAPRSKISSPORT::staticMetaObject = {
    { &QAPRSPort::staticMetaObject, qt_meta_stringdata_QAPRSKISSPORT,
      qt_meta_data_QAPRSKISSPORT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSKISSPORT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSKISSPORT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSKISSPORT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSKISSPORT))
        return static_cast<void*>(const_cast< QAPRSKISSPORT*>(this));
    return QAPRSPort::qt_metacast(_clname);
}

int QAPRSKISSPORT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAPRSPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onComReadTimer(); break;
        case 1: onComRead((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_QAPRSFLDIGIPORT[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      38,   16,   16,   16, 0x08,
      74,   62,   16,   16, 0x08,
     119,   16,   16,   16, 0x08,
     135,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSFLDIGIPORT[] = {
    "QAPRSFLDIGIPORT\0\0tcpClientconnected()\0"
    "tcpClientdisconnected()\0socketError\0"
    "tcpClienterror(QAbstractSocket::SocketError)\0"
    "tcpClientread()\0onReconnectTimer()\0"
};

const QMetaObject QAPRSFLDIGIPORT::staticMetaObject = {
    { &QAPRSPort::staticMetaObject, qt_meta_stringdata_QAPRSFLDIGIPORT,
      qt_meta_data_QAPRSFLDIGIPORT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSFLDIGIPORT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSFLDIGIPORT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSFLDIGIPORT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSFLDIGIPORT))
        return static_cast<void*>(const_cast< QAPRSFLDIGIPORT*>(this));
    return QAPRSPort::qt_metacast(_clname);
}

int QAPRSFLDIGIPORT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAPRSPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: tcpClientconnected(); break;
        case 1: tcpClientdisconnected(); break;
        case 2: tcpClienterror((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: tcpClientread(); break;
        case 4: onReconnectTimer(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_QAPRSVASKAPORT[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      37,   15,   15,   15, 0x08,
      73,   61,   15,   15, 0x08,
     118,   15,   15,   15, 0x08,
     134,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QAPRSVASKAPORT[] = {
    "QAPRSVASKAPORT\0\0tcpClientconnected()\0"
    "tcpClientdisconnected()\0socketError\0"
    "tcpClienterror(QAbstractSocket::SocketError)\0"
    "tcpClientread()\0onReconnectTimer()\0"
};

const QMetaObject QAPRSVASKAPORT::staticMetaObject = {
    { &QAPRSPort::staticMetaObject, qt_meta_stringdata_QAPRSVASKAPORT,
      qt_meta_data_QAPRSVASKAPORT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAPRSVASKAPORT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAPRSVASKAPORT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAPRSVASKAPORT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAPRSVASKAPORT))
        return static_cast<void*>(const_cast< QAPRSVASKAPORT*>(this));
    return QAPRSPort::qt_metacast(_clname);
}

int QAPRSVASKAPORT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAPRSPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: tcpClientconnected(); break;
        case 1: tcpClientdisconnected(); break;
        case 2: tcpClienterror((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: tcpClientread(); break;
        case 4: onReconnectTimer(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
