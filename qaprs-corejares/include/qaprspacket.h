#ifndef QAPRSPACKET_H
#define QAPRSPACKET_H
#include <QObject>
#include <QByteArray>
#include <QString>


class QAPRSAXUIPacket: public QObject, public QByteArray
{
    Q_OBJECT
    public:
       QAPRSAXUIPacket( QObject *parent = 0 );
       QAPRSAXUIPacket( QString To, QString From, QString MsgText );
       ~QAPRSAXUIPacket( );

};

class QAPRSAXIPPacket: public QObject, public QByteArray
{
    Q_OBJECT
    public:
       QAPRSAXIPPacket( QObject *parent = 0 );
       QAPRSAXIPPacket( QString To, QString From, QString MsgText );
       ~QAPRSAXIPPacket( );

};

class QAPRSAGWPacket: public QObject, public QByteArray
{
    Q_OBJECT
    public:
       QAPRSAGWPacket( QObject *parent = 0 );
       QAPRSAGWPacket( QString To, QString From, QString MsgText );
       ~QAPRSAGWPacket( );

};

class QAPRSAGWRXPacket: public QObject, public QByteArray
{
    Q_OBJECT
    public:
       QAPRSAGWRXPacket( QObject *parent = 0 );
       QAPRSAGWRXPacket( QString To, QString From, QString MsgText );
       ~QAPRSAGWRXPacket( );

};

class QAPRSKISSPacket: public QObject, public QByteArray
{
    Q_OBJECT
    public:
       QAPRSKISSPacket( QObject *parent = 0 );
       QAPRSKISSPacket( QString To, QString From, QString MsgText );
       ~QAPRSKISSPacket( );

};

#endif // QAPRSPACKET_H
