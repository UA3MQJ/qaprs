#include <QDebug>
#include "../../core/include/qaprsbeacon.h"


QAPRSBeacon::QAPRSBeacon( QObject *parent ) {

    this->setBeaconStatus( 1 );
    beacontimer = new QTimer(this);
    Interval = -1;
    APRSCall = "APZ010";

}

QAPRSBeacon::~QAPRSBeacon() {

}

void QAPRSBeacon::setParam( QString ParamName, QString ParamValue ) {

    if ( ParamName=="CALL" ) {
        Call = ParamValue;
    }
    if ( ParamName=="LAT" ) {
        Lat = ParamValue;
    }
    if ( ParamName=="LNG" ) {
        Lng = ParamValue;
    }
    if ( ParamName=="INTERVAL" ) {
        Interval = ParamValue.toInt();
    }
    if ( ParamName=="UNPROTO" ) {
        Unproto = ParamValue;
    }
    if ( ParamName=="TEXT" ) {
        Text = ParamValue;
    }
    if ( ParamName=="SYM" ) {
        Sym = ParamValue;
    }
    if ( ParamName=="PORT_NUM" ) {
        PortNum = ParamValue.toInt();
    }

}

void QAPRSBeacon::openBeacon() {

    this->setBeaconStatus( 0 );

    connect( beacontimer, SIGNAL( timeout() ),
             this,        SLOT( onTimer() ) );

    if (Interval > 0) beacontimer->start( Interval * 1000 ); else beacontimer->stop();

}

void QAPRSBeacon::closeBeacon() {

    this->setBeaconStatus( 1 );
    beacontimer->stop();
    beacontimer->disconnect();

}

void QAPRSBeacon::onTimer() {

    //qDebug() << "Send Beacon";
    QString To;
    QString From;
    QString MsgText;

    To      = APRSCall;
    From    = Call + ">" + Unproto;

    MsgText.append( '=' );
    MsgText.append( Lat.left( 2 ) );
    MsgText.append( Lat.mid( 3 ) );
    MsgText.append( Sym[0] );
    MsgText.append( Lng.left( 3 ) );
    MsgText.append( Lng.mid( 4 ) );
    MsgText.append( Sym[1]+Text+" {QAPRS}" );

    emit sendBeacon( iBeaconNum,  PortNum, To, From, MsgText );

}

