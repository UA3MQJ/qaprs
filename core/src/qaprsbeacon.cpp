#include <QDebug>
#include "../../core/include/qaprsbeacon.h"


QAPRSBeacon::QAPRSBeacon( QObject *parent ) {
    Q_UNUSED(parent)
    this->setBeaconStatus( 1 );
    beacontimer = new QTimer(this);
    statustimer = new QTimer(this);
    Interval    = -1;
    sInterval   = -1;

    stored = true;

    qDebug() << "Beacon::Create";

}

QAPRSBeacon::~QAPRSBeacon() {

}

void QAPRSBeacon::setParam( QString ParamName, QString ParamValue ) {

    //qDebug() << "QAPRSBeacon::setParam " << ParamName << "=" << ParamValue;

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
    if ( ParamName=="STATINTERVAL" ) {
        sInterval = ParamValue.toInt();
    }
    if ( ParamName=="UNPROTO" ) {
        Unproto = ParamValue;
    }
    if ( ParamName=="TEXT" ) {
        Text = ParamValue;
    }
    if ( ParamName=="STATTEXT" ) {
        sText = ParamValue;
    }
    if ( ParamName=="SYM" ) {
        Sym = ParamValue;
    }
    if ( ParamName=="PORT_NUM" ) {
        PortNum = ParamValue.toInt();
    }

    stored = false;

}

QString QAPRSBeacon::getParam( QString ParamName ) {

    //qDebug() << "QAPRSBeacon::getParam( QString " + ParamName + " )";

    if ( ParamName=="CALL" ) {
        return Call;
    }
    if ( ParamName=="LAT" ) {
        return Lat;
    }
    if ( ParamName=="LNG" ) {
        return Lng;
    }
    if ( ParamName=="INTERVAL" ) {
        return QString::number( Interval );
    }
    if ( ParamName=="STATINTERVAL" ) {
        return QString::number( sInterval );
    }
    if ( ParamName=="UNPROTO" ) {
        return Unproto;
    }
    if ( ParamName=="TEXT" ) {
        return Text;
    }
    if ( ParamName=="STATTEXT" ) {
        return sText;
    }
    if ( ParamName=="SYM" ) {
        return Sym;
    }
    if ( ParamName=="PORT_NUM" ) {
        return QString::number( PortNum );
    }

    return "";

}

QString QAPRSBeacon::getParamName( int parNum ) {

    switch ( parNum ) {
        case 0 :
        return "CALL";
        break;
        case 1 :
        return "LAT";
        break;
        case 2 :
        return "LNG";
        break;
        case 3 :
        return "INTERVAL";
        break;
        case 4 :
        return "STATINTERVAL";
        break;
        case 5 :
        return "UNPROTO";
        break;
        case 6 :
        return "TEXT";
        break;
        case 7 :
        return "STATTEXT";
        break;
        case 8 :
        return "SYM";
        break;
        case 9 :
        return "PORT_NUM";
        break;
    }

    return "";

}

QString QAPRSBeacon::getParamValue( int parNum ) {

    return getParam( getParamName( parNum ) );

}

void QAPRSBeacon::updateBeacon() {

    this->closeBeacon();
    this->openBeacon();

}

void QAPRSBeacon::openBeacon() {

    this->setBeaconStatus( 0 );

    connect( beacontimer, SIGNAL( timeout() ),
             this,        SLOT( onTimer() ) );


    if ( Interval > 0 ) {
        //qDebug() << "interval = " << Interval;
        //qDebug() << "beacon started";
        beacontimer->start( Interval * 1000 );
    } else {
        //qDebug() << "interval = " << Interval;
        //qDebug() << "beacon stop";
        beacontimer->stop();
    }


    //статус маяка
    connect( statustimer, SIGNAL( timeout() ),
             this,        SLOT( onSTimer() ) );

    if ( sInterval > 0 ) {
        //qDebug() << "interval = " << Interval;
        //qDebug() << "beacon started";
        statustimer->start( sInterval * 1000 );
    } else {
        //qDebug() << "interval = " << Interval;
        //qDebug() << "beacon stop";
        statustimer->stop();
    }

}

void QAPRSBeacon::closeBeacon() {

    this->setBeaconStatus( 1 );
    beacontimer->stop();
    beacontimer->disconnect();
    statustimer->stop();
    statustimer->disconnect();

}

void QAPRSBeacon::onTimer() {

    //qDebug() << "Send Beacon";
    QString To;
    QString From;
    QString MsgText;

    To      = SysVars->getVal( "APRSCall" );
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

void QAPRSBeacon::onSTimer() {

    //qDebug() << "Send Status text";
    QString To;
    QString From;
    QString MsgText;
    QString tStatText;

    To      = SysVars->getVal( "APRSCall" );
    From    = Call + ">" + Unproto;

    //>261020zUI-View32 V2.03
    MsgText.append( ">" + QDateTime::currentDateTime().toUTC().toString( "ddhhmm" ) + "z");
    tStatText = sText;

    //str.replace(QString("ou"), QString("o"));

    MsgText.append( tStatText );

    if ( MsgText.indexOf( "$VERSION" ) != -1 )
        MsgText.replace( QString("$VERSION"), SysVars->getVal( "APRSVersion" ) );

    //qDebug() << "APRSVersion=" << APRSVersion;
    //qDebug() << "MsgText=" << MsgText;

    emit sendBeacon( iBeaconNum,  PortNum, To, From, MsgText );

}

void QAPRSBeacon::sendBeacon() {

    onTimer();

}

void QAPRSBeacon::sendStatus() {
//послать статус принудительно

    onSTimer();

}
