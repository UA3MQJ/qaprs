#include <QScrollBar>
#include <QDebug>
#include "include/qaprsports.h"



QAPRSPort::QAPRSPort( QObject *parent ) { Q_UNUSED(parent) }

void QAPRSPort::openPort() { }
void QAPRSPort::closePort() { }
void QAPRSPort::sendPacket( QString To, QString From, QString MsgText ) { Q_UNUSED(To) Q_UNUSED(From) Q_UNUSED(MsgText) }
void QAPRSPort::setParam( QString ParamName, QString ParamValue ) { Q_UNUSED(ParamName) Q_UNUSED(ParamValue) }


//AXIP PORT
QAPRSAXIPPORT::QAPRSAXIPPORT( QObject *parent ) {
    Q_UNUSED(parent)
    PortType    = "AXIP";
    active      = false;
    udpSocket   = new QUdpSocket(this);
    beacontimer = new QTimer(this);
    TXAddress   = new QHostAddress();
};

void QAPRSAXIPPORT::setParam( QString ParamName, QString ParamValue ) {
    if ( ParamName=="RXPort" )         RXPort = ParamValue.toInt();
    if ( ParamName=="TXPort" )         TXPort = ParamValue.toInt();
    if ( ParamName=="IPAddress" )      TXAddress->setAddress( ParamValue );
    if ( ParamName=="BeaconInterval" ) BeaconInterval = ParamValue.toInt() * 1000;
    if ( ParamName=="PortName")        PortName = ParamValue;
    if ( ParamName=="Call")            Call = ParamValue;
    if ( ParamName=="BeaconText")      BeaconText = ParamValue;
    if ( ParamName=="UnprotoAddress")  UnprotoAddress = ParamValue;
    if ( ParamName=="Latitude")        Latitude = ParamValue;
    if ( ParamName=="Longitude")       Longitude = ParamValue;
    if ( ParamName=="Symbol")          Symbol = ParamValue;
}

void QAPRSAXIPPORT::openPort() {
    active = true;

    emit ToLog( "(" + QString::number( PortNum ) + ")" + PortType + "::openPort<br>" );
    emit ToLog( "(" + QString::number( PortNum ) + ")" + PortType + "::Start listen UDP on port <b>" + QString::number( RXPort ) + "</b><br>" );

    connect( udpSocket, SIGNAL( readyRead() ),
             this,      SLOT( readPendingDatagrams() ) );

    connect( beacontimer,SIGNAL( timeout() ),
             this,      SLOT( onTimer() ) );

    udpSocket->bind( RXPort );
    if (BeaconInterval > 0) beacontimer->start( BeaconInterval ); else beacontimer->stop();
}

void QAPRSAXIPPORT::closePort() {
    active = false;
    beacontimer->stop();
    udpSocket->abort();

    beacontimer->disconnect();
    udpSocket->disconnect();

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::closePort<br>");
}

void QAPRSAXIPPORT::readPendingDatagrams() {
    QHostAddress *address;
    quint16 port;
    QByteArray datagram;
    QString DataIn;

    address = new QHostAddress();
    //read udp datagram
    while ( udpSocket->hasPendingDatagrams() ) {
        datagram.resize( udpSocket->pendingDatagramSize() );
        udpSocket->readDatagram( datagram.data(), datagram.size(), address, &port );
    }

    DataIn = datagram.data();
    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet</font><br>");
    //log->insertHtml( datagram +"<br>");

    int i;
    QString DestCall( "" );
    unsigned char DestSID;
    for( i=1; i<=6; i++ ) {
        if ( ( uchar( datagram.data()[i-1] ) >> 1 )!=' ' )
            DestCall += uchar( datagram.data()[i-1] ) >> 1;
    }

    DestSID = ( uchar( datagram.data()[6] ) >> 1 ) & 15;
    if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::DestCall='" );
    //log->insertPlainText( DestCall );
    //log->insertHtml( "'<br>" );

    int sh;
    sh = 7;
    QString SrcCall( "" );
    unsigned char SrcSID;
    do {
        for( i=1; i<=6; i++ ) {
            if ( ( uchar( datagram.data()[sh+i-1] ) >> 1 )!=' ' )
                SrcCall += uchar( datagram.data()[sh+i-1] ) >> 1;
        }
        SrcSID = ( uchar( datagram.data()[sh+6] ) >> 1 ) & 15;             //SID
        if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
        if ( ( ( uchar( datagram.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
        if ( ( ( uchar( datagram.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
        sh += 7;
    } while ( ( ( uchar( datagram.data()[sh-1] ) ) & 1 ) !=1 );


    //replace first ',' of the Src to '>'
    if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::SrcCall='" );
    //log->insertPlainText( SrcCall +"'\n");


    QString MsgText( "" );
    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::MsgText='" );
    MsgText = datagram.mid( sh + 2, datagram.count() - sh - 5 ); // +2 - skip Control byte and PID byte
                                                                 // -5 - skip ^^ and 2 byte of CRC-16

    //log->insertPlainText( MsgText + "\n");
    //log->insertPlainText( datagram.toHex() );
    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

    emit RXPacket( PortNum, DestCall, SrcCall, MsgText );
}

void QAPRSAXIPPORT::onTimer() {
    QString Beacon;

    Beacon.append( '=' );
    Beacon.append( Latitude.left( 2 ) );
    Beacon.append( Latitude.mid( 3 ) );
    Beacon.append( Symbol[0] );
    Beacon.append( Longitude.left( 3 ) );
    Beacon.append( Longitude.mid( 4 ) );
    Beacon.append( Symbol[1]+BeaconText+" {QAPRS}" );

    this->sendPacket( "APU25N", Call + ">" + UnprotoAddress, Beacon );

/*
    QString Ping;

    Ping.clear();
    Ping.append("UA3MQJ");
    Ping = Ping.leftJustified(9, ' ');
    Ping.append(":");
    Ping.prepend(":");
    Ping.append("?APRST");

    this->sendPacket( "APU25N", Call + ">WIDE3-1" , Ping );
*/
}

void QAPRSAXIPPORT::sendPacket( QString To, QString From, QString MsgText ){
    QAPRSAXIPPacket Packet( To, From, MsgText );
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );
    //log->insertHtml(Packet+"<br>");


    udpSocket->writeDatagram( Packet, *TXAddress, TXPort );

    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

    emit TXPacket( PortNum, To, From, MsgText );
}

// Internet Port

QAPRSINTERNETPORT::QAPRSINTERNETPORT( QObject *parent ) {
    Q_UNUSED(parent)
    PortType    = "Inet";
    active      = false;

    tcpClient   = new QTcpSocket(this);
    beacontimer = new QTimer(this);
    reconnecttimer = new QTimer(this);

    srvnum      = 0;
}


void QAPRSINTERNETPORT::setParam( QString ParamName, QString ParamValue ) {
    if ( ParamName=="BeaconInterval" ) BeaconInterval = ParamValue.toInt() * 1000;
    if ( ParamName=="PortName")        PortName = ParamValue;
    if ( ParamName=="Call")            Call = ParamValue;
    if ( ParamName=="BeaconText")      BeaconText = ParamValue;
    if ( ParamName=="UnprotoAddress")  UnprotoAddress = ParamValue;
    if ( ParamName=="Latitude")        Latitude = ParamValue;
    if ( ParamName=="Longitude")       Longitude = ParamValue;
    if ( ParamName=="Symbol")          Symbol = ParamValue;
    if ( ParamName=="User")            UserName = ParamValue;
    if ( ParamName=="Pass")            UserPass = ParamValue;
    if ( ParamName=="Filter")          APRSFilter = ParamValue;
    if ( ParamName=="Server1")         APRSServer1 = ParamValue;
    if ( ParamName=="Server2")         APRSServer2 = ParamValue;
    if ( ParamName=="Server3")         APRSServer3 = ParamValue;
}


void QAPRSINTERNETPORT::openPort() {
    active = true;

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::openPort<br>");
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Connecting to APRS server"+QString::number( srvnum + 1 )+" ...</b><br>" );

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected ()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpClientdisconnected ()));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpClienterror(QAbstractSocket::SocketError)));

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));

    connect(beacontimer, SIGNAL(timeout()), this, SLOT(onTimer()));

    QString APRSAddress;
    QString APRSPort;
    QByteArray ConnectionString;
    //p/ISS/R/U/LY/YL/ES/EU/EW/ER/4X/4Z

    QString APRSServer;

    switch ( srvnum ) {
        case 0 : APRSAddress = APRSServer1.left( APRSServer1.indexOf(':') );
                 APRSPort    = APRSServer1.mid( APRSServer1.indexOf(':')+1 );
                 break;
        case 1 : APRSAddress = APRSServer2.left( APRSServer2.indexOf(':') );
                 APRSPort    = APRSServer2.mid( APRSServer2.indexOf(':')+1 );
                 break;
        case 2 : APRSAddress = APRSServer3.left( APRSServer3.indexOf(':') );
                 APRSPort    = APRSServer3.mid( APRSServer3.indexOf(':')+1 );
                 break;
    }


    ConnectionString.clear();
    ConnectionString.append( "user " + UserName + " pass " + UserPass + " vers QAPRS v.001 filter " + APRSFilter + "\n" );

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::APRS Server Address = '"+APRSAddress+"'<br>");
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::APRS Server Port = '"+APRSPort+"'<br>");

    tcpClient->connectToHost( APRSAddress, APRSPort.toUInt() );

    tcpClient->write( ConnectionString );

    if (BeaconInterval > 0) beacontimer->start( BeaconInterval ); else beacontimer->stop();
}

void QAPRSINTERNETPORT::closePort() {
    active = false;
    tcpClient->abort();
    beacontimer->stop();

    tcpClient->disconnect();
    beacontimer->disconnect();
    reconnecttimer->disconnect();

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::closePort<br>");
}

void QAPRSINTERNETPORT::tcpClientconnected () {
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Connected<br>");
};

void QAPRSINTERNETPORT::tcpClientdisconnected () {
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Disconnected<br>");

    this->closePort();
};

void QAPRSINTERNETPORT::tcpClienterror (QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Clienterror<br>");

    this->closePort();

    //start reconnect timer
    connect(reconnecttimer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));
    reconnecttimer->start( 1000 );
};

void QAPRSINTERNETPORT::tcpClientread() {
    QByteArray msg;

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet</font><br>");
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::tcpClientread<br>");

    while (tcpClient->bytesAvailable()==0);
    msg = tcpClient->readAll();

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet '" + msg.toHex() + "'</font><br>");
//    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::RX Packet "+ msg +" <br>");

    QString mesg;

    while (msg.length()>0) {

        mesg = msg.left( msg.indexOf(13)+2 );
       //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::'" + mesg + "'</font><br>");
//        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::RX Packet "+ mesg +" <br>");


        if (mesg.data()[0]!='#') {
            //log->insertHtml( "Packet='" );
            //log->insertPlainText(mesg.left(mesg.length()-2));
            //log->insertHtml( "'<br>" );

            QString DestCall("");
            DestCall = mesg.mid( mesg.indexOf(">") + 1, mesg.indexOf(",") - mesg.indexOf(">") - 1 );

            //log->insertHtml( "DestCall='" );
            //log->insertPlainText(DestCall);
            //log->insertHtml( "'<br>" );

            QString SrcCall("");
            SrcCall = mesg.left( mesg.indexOf(">") ) + mesg.mid( mesg.indexOf(","), mesg.indexOf(":") - mesg.indexOf(",") );
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';
            //log->insertHtml( "SrcCall='" );
            //log->insertPlainText(SrcCall);
            //log->insertHtml( "'<br>" );

            QString MsgText("");
            MsgText = mesg.mid( mesg.indexOf(":") + 1, mesg.length() - mesg.indexOf(":") - 3 );
            //log->insertHtml( "MsgText='" );
            //log->insertPlainText(MsgText);
            //log->insertHtml( "'<br>" );

            emit ToLog("("+QString::number(PortNum)+")"+PortType+"::RX Packet DestCall="+ DestCall +"  SrcCall="+ SrcCall +"  MsgText="+ MsgText +" <br>");

            emit RXPacket( PortNum, DestCall, SrcCall, MsgText );

        } else {
            //log->insertHtml("(" + QString::number( PortNum ) + ")" + PortType + "::SRVMsg='");
            //log->insertPlainText(mesg.left(mesg.length() - 2) );
            //log->insertHtml("'<br>");
            //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
            emit ToLog( "(" + QString::number( PortNum ) + ")" + PortType + "::SRVMsg='" + mesg.left(mesg.length() - 2) + "'" );
        }

        msg = msg.mid( msg.indexOf(10) + 1 );

    }
};

void QAPRSINTERNETPORT::onTimer() {
    QString Beacon;

    Beacon.append( '=' );
    Beacon.append( Latitude.left( 2 ) );
    Beacon.append( Latitude.mid( 3 ) );
    Beacon.append( Symbol[0] );
    Beacon.append( Longitude.left( 3 ) );
    Beacon.append( Longitude.mid( 4 ) );
    Beacon.append( Symbol[1]+BeaconText+" {QAPRS}" );

    this->sendPacket( "APU25N", Call + ">" + UnprotoAddress, Beacon );
};

void QAPRSINTERNETPORT::onReconnectTimer() {
    emit ToLog( "(" + QString::number(PortNum) + ")" + PortType + "::Try reconnect to AGW core<br>" );

    reconnecttimer->stop();
    reconnecttimer->disconnect();

    //select next aprs server
    srvnum++;
    if (srvnum>2) srvnum = 0;

    this->openPort();
}

void QAPRSINTERNETPORT::sendPacket( QString To, QString From, QString MsgText ){
    QByteArray Packet;
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );

    Packet.clear();
    Packet.append( From.left( From.indexOf('>') ) + ">APU25N,TCPIP*:" + MsgText);

    Packet.append( 13 );
    Packet.append( 10 );

    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::'" + Packet + "'<br>" );

    tcpClient->write(Packet);

    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::'" + Packet.toHex() + "'<br>" );
    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

    emit TXPacket( PortNum, To , From, MsgText );
}

// AGW Port

QAPRSAGWPORT::QAPRSAGWPORT( QObject *parent ) {
    Q_UNUSED(parent)
    PortType    = "AGW";
    active      = false;

    tcpClient = new QTcpSocket(this);
    beacontimer = new QTimer(this);
    reconnecttimer = new QTimer(this);
}

void QAPRSAGWPORT::setParam( QString ParamName, QString ParamValue ) {
    if ( ParamName=="BeaconInterval" ) BeaconInterval = ParamValue.toInt() * 1000;
    if ( ParamName=="PortName")        PortName = ParamValue;
    if ( ParamName=="Call")            Call = ParamValue;
    if ( ParamName=="BeaconText")      BeaconText = ParamValue;
    if ( ParamName=="UnprotoAddress")  UnprotoAddress = ParamValue;
    if ( ParamName=="Latitude")        Latitude = ParamValue;
    if ( ParamName=="Longitude")       Longitude = ParamValue;
    if ( ParamName=="Symbol")          Symbol = ParamValue;
    if ( ParamName=="Host")            agwHost = ParamValue;
    if ( ParamName=="AGWPort")         agwPort = ParamValue.toInt();
}

void QAPRSAGWPORT::openPort() {
    QHostAddress AGWHostAddress;

    active = true;

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::openPort<br>");
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Connecting to AGW core</b><br>" );

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected ()));

    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpClientconnected ()));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpClienterror(QAbstractSocket::SocketError)));

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));

    connect(beacontimer, SIGNAL(timeout()), this, SLOT(onTimer()));

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::AGW Host = '" + agwHost + "'<br>");

    AGWHostAddress.setAddress( agwHost );

    tcpClient->connectToHost(AGWHostAddress , 8000);
    agwStatus = 0; //set status - connect to AGW core

    QByteArray Pkt;
    Pkt.fill(0, 36);
    Pkt[4]='R';//
    tcpClient->write( Pkt ); //seng qry "Port Information"

    if (BeaconInterval > 0) beacontimer->start( BeaconInterval ); else beacontimer->stop();
}

void QAPRSAGWPORT::closePort() {
    active = false;
    beacontimer->stop();
    tcpClient->abort();

    tcpClient->disconnect();
    beacontimer->disconnect();
    reconnecttimer->disconnect();

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::closePort<br>");
}

void QAPRSAGWPORT::tcpClientconnected () {

};

void QAPRSAGWPORT::tcpClientdisconnected () {
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Disconnected<br>");

    this->closePort();
};

void QAPRSAGWPORT::tcpClienterror (QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Clienterror<br>");

    this->closePort();

    //start reconnect timer
    connect(reconnecttimer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));
    reconnecttimer->start( 30000 );
};

void QAPRSAGWPORT::tcpClientread() {
    QByteArray mesg;

    while (tcpClient->bytesAvailable()==0);

    mesg = tcpClient->readAll();

    //log->insertPlainText ("("+QString::number(PortNum)+")"+PortType+"::AGW version '"+mesg.mid(4,1)+"'\n");
    //log->insertPlainText("("+QString::number(PortNum)+")"+PortType+"::AGW len '"+QString::number(mesg.length())+"'\n");
    //log->insertPlainText("("+QString::number(PortNum)+")"+PortType+"::AGW packet '"+mesg.toHex()+"'\n");

    if (agwStatus==0) {        
        agwStatus=1;
        QByteArray Pkt;
        Pkt.fill(0, 36);
        Pkt[4]='G';//
        tcpClient->write( Pkt ); //seng qry "Port Information"
    } else
    if (agwStatus==1) {
        //read information of AGW port list

        mesg = mesg.mid(36);

        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::AGW Ports information<br>");
        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Ports count = "+mesg.left(mesg.indexOf(';'))+"<br>");

        if ( agwPort>( mesg.left(mesg.indexOf(';')) ).toInt() ) {
            //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#FF0000'>Ports number (" +
            //                 QString::number( agwPort ) + ") not support on AGW Core!</font><br>");
            emit ToLog( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#FF0000'>Ports number (" +
                              QString::number( agwPort ) + ") not support on AGW Core!</font><br>");

            this->closePort();
            return ;
        }

        mesg = mesg.mid( mesg.indexOf( ';' ) + 1 );

        do  {
            //log->insertHtml("("+QString::number(PortNum)+")"+PortType+"::"+mesg.left(mesg.indexOf(';'))+"<br>");
            emit ToLog("("+QString::number(PortNum)+")"+PortType+"::"+mesg.left(mesg.indexOf(';'))+"<br>");
            mesg = mesg.mid( mesg.indexOf( ';' ) + 1);
        } while ( mesg.indexOf( ';' ) !=-1 );

        agwStatus = 2;
        QByteArray Pkt;
        Pkt.fill(0, 36);
        Pkt[4]='k';
        tcpClient->write( Pkt ); //seng qry Start Monitoring Data

    } else
    if (agwStatus==2) {

        if ((uchar(mesg.data()[0])+1)==agwPort) {
            //if packet to my port

            QString DataIn;
            DataIn = mesg.data();
            //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet</font><br>");
            //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::AGW port " + QString::number( uchar(mesg.data()[0])+1 ) + "<br>");

            //log->insertHtml( "("+QString::number(PortNum)+")"+PortType+"::RX Packet len: '" + QString::number( mesg.length() ) + "'<br>" );
            //log->insertHtml( "("+QString::number(PortNum)+")"+PortType+"::RX Packet hex: '" + mesg.toHex() + "'<br>" );
            mesg = mesg.mid( 37 );
            //log->insertHtml( "("+QString::number(PortNum)+")"+PortType+"::RX Packet: '" +  mesg + "'<br>" );

            int i;
            QString DestCall( "" );
            unsigned char DestSID;
            for( i=1; i<=6; i++ ) {
                if ( ( uchar( mesg.data()[i-1] ) >> 1 )!=' ' )
                    DestCall += uchar( mesg.data()[i-1] ) >> 1;
            }

            DestSID = ( uchar( mesg.data()[6] ) >> 1 ) & 15;
            if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

            //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::DestCall='" );
            //log->insertPlainText( DestCall + "'\n");

            int sh;
            sh = 7;
            QString SrcCall( "" );
            unsigned char SrcSID;
            do {
                for( i=1; i<=6; i++ ) {
                    if ( ( uchar( mesg.data()[sh+i-1] ) >> 1 )!=' ' )
                        SrcCall += uchar( mesg.data()[sh+i-1] ) >> 1;
                }
                SrcSID = ( uchar( mesg.data()[sh+6] ) >> 1 ) & 15;             //SID
                if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
                if ( ( ( uchar( mesg.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
                if ( ( ( uchar( mesg.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
                sh += 7;
            } while ( ( ( uchar( mesg.data()[sh-1] ) ) & 1 ) !=1 );

            //replace first ',' of the Src to '>'
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

            //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::SrcCall='" );
            //log->insertPlainText( SrcCall + "'\n" );

            QString MsgText( "" );
            //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::MsgText='" );
            MsgText = mesg.mid( sh + 2 , mesg.count() - sh - 3 ); // +2 - skip Control byte and PID byte
                                                                         // -5 - skip ^^ and 2 byte of CRC-16

            //log->insertPlainText( MsgText + "'\n");
            //log->insertPlainText( datagram.toHex() );
            //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

            emit RXPacket( PortNum, DestCall, SrcCall, MsgText );


        } //end if my port

    }

    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

};

void QAPRSAGWPORT::onTimer() {
    QString Beacon;

    Beacon.append( '=' );
    Beacon.append( Latitude.left( 2 ) );
    Beacon.append( Latitude.mid( 3 ) );
    Beacon.append( Symbol[0] );
    Beacon.append( Longitude.left( 3 ) );
    Beacon.append( Longitude.mid( 4 ) );
    Beacon.append( Symbol[1]+BeaconText+" {QAPRS}" );

    this->sendPacket( "APU25N", Call + ">" + UnprotoAddress, Beacon );
};

void QAPRSAGWPORT::onReconnectTimer() {
    emit ToLog( "(" + QString::number(PortNum) + ")" + PortType + "::Try reconnect to AGW core<br>" );

    reconnecttimer->stop();
    reconnecttimer->disconnect();
    this->openPort();
}

void QAPRSAGWPORT::sendPacket( QString To, QString From, QString MsgText ){

    //020000004b00000000000000000000000000000000000000000000002d0000000000000000+packet without CRC, eof 0x0d
    //                                                         ^-data len (pack len - 36)
    //         ^- 'K'
    // ^-agw port numbe ( = agwPort-1 )
    //QAPRSAGWPacket

    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );

    QAPRSAGWPacket Packet( To, From, MsgText );
    emit TXPacket( PortNum, To, From, MsgText );

    Packet.data()[0] = agwPort - 1;
    //log->insertPlainText("("+QString::number(PortNum)+")"+PortType+"::Packet= "+Packet.toHex()+"<br>");

    tcpClient->write( Packet ); //
}


//KISS PORT

ComReadThread::ComReadThread(QObject *parent)
     : QThread(parent)
 {

 };

ComReadThread::~ComReadThread()
 {
    wait();
 };


void ComReadThread::run() {
#ifdef Q_OS_WIN
    forever {
        //qDebug() << "waiting data from com port";

        static char *buffer = NULL;
        if(!buffer) { buffer = (char *)malloc(1024); };
        DWORD len = 1024;
        DWORD feedback = 0;
        ReadFile(m_Handle, buffer, len, &feedback, NULL);
        //qDebug() << "data readed";

        QByteArray ComData;
        if (feedback>0)     {

            ComData.clear();
            ComData.resize(feedback);
            for(DWORD i=0;i<feedback;i++){
                ComData.data()[i] = buffer[i];
            };

            ComData = ComData.mid( 2, ComData.length() - 3 );

            QString DataIn;

            DataIn = ComData.data();

            int i;
            QString DestCall( "" );
            unsigned char DestSID;
            for( i=1; i<=6; i++ ) {
                if ( ( uchar( ComData.data()[i-1] ) >> 1 )!=' ' )
                    DestCall += uchar( ComData.data()[i-1] ) >> 1;
            };

            DestSID = ( uchar( ComData.data()[6] ) >> 1 ) & 15;
            if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

            int sh;
            sh = 7;
            QString SrcCall( "" );
            unsigned char SrcSID;
            do {
                for( i=1; i<=6; i++ ) {
                    if ( ( uchar( ComData.data()[sh+i-1] ) >> 1 )!=' ' )
                        SrcCall += uchar( ComData.data()[sh+i-1] ) >> 1;
                };
                SrcSID = ( uchar( ComData.data()[sh+6] ) >> 1 ) & 15;             //SID
                if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
                if ( ( ( uchar( ComData.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
                if ( ( ( uchar( ComData.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
                sh += 7;
            } while ( ( ( uchar( ComData.data()[sh-1] ) ) & 1 ) !=1 );


            //replace first ',' of the Src to '>'
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

            QString MsgText( "" );
            MsgText = ComData.mid( sh + 2, ComData.count() - sh - 3 ); // +2 - skip Control byte and PID byte
                                                                         // -5 - skip ^^ and 2 byte of CRC-16 (NO IN KISS)
            emit RXPacket( DestCall, SrcCall, MsgText  );

        };

    }
#endif
};

QAPRSKISSPORT::QAPRSKISSPORT( QObject *parent ) {
    Q_UNUSED(parent)
    PortType     = "KISS";
    active       = false;
    beacontimer  = new QTimer(this);
    ComReadtimer = new QTimer(this);
    //CRThread    = new ComReadThread(this);
};

void QAPRSKISSPORT::setParam( QString ParamName, QString ParamValue ) {
    if ( ParamName=="BeaconInterval" ) BeaconInterval = ParamValue.toInt() * 1000;
    if ( ParamName=="PortName")        PortName = ParamValue;
    if ( ParamName=="Call")            Call = ParamValue;
    if ( ParamName=="BeaconText")      BeaconText = ParamValue;
    if ( ParamName=="UnprotoAddress")  UnprotoAddress = ParamValue;
    if ( ParamName=="Latitude")        Latitude = ParamValue;
    if ( ParamName=="Longitude")       Longitude = ParamValue;
    if ( ParamName=="Symbol")          Symbol = ParamValue;
    if ( ParamName=="COM") {
                                       ComName.clear();
                                       ComName.append( ParamValue );
    }
#ifdef Q_OS_WIN
    if ( ParamName=="Speed")           ComSpeed = ParamValue.toInt();
#endif
    if ( ParamName=="TXDelay")         TXDelay = ParamValue.toInt();
    if ( ParamName=="TXTail")          TXTail = ParamValue.toInt();
    if ( ParamName=="SlotTime")        SlotTime = ParamValue.toInt();
    if ( ParamName=="Persist")         Persist = ParamValue.toInt();
    if ( ParamName=="FullDuplex")      FullDuplex = (ParamValue=="T");
    if ( ParamName=="ExitFromKISS")    ExitFromKISS = ParamValue;
    //commmand
    // ^C - 03h
    // ^M - 0Dh
    char ctrlC = 0x03;
    char ctrlM = 0x0D;

    if ( ParamName=="Command1") {
        Command1.clear();
        Command1.append( ParamValue );
        Command1.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command1.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command1.append( ctrlM );
    }
    if ( ParamName=="Command2") {
        Command2.clear();
        Command2.append( ParamValue );
        Command2.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command2.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command2.append( ctrlM );
    }
    if ( ParamName=="Command3") {
        Command3.clear();
        Command3.append( ParamValue );
        Command3.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command3.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command3.append( ctrlM );
    }
    if ( ParamName=="Command4") {
        Command4.clear();
        Command4.append( ParamValue );
        Command4.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command4.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command4.append( ctrlM );
    }

}

void QAPRSKISSPORT::openPort() {
    active = true;

    emit ToLog( "(" + QString::number( PortNum ) + ")" + PortType + "::openPort<br>" );

	ComReadData.clear();

#ifdef Q_OS_WIN
    QString COMPortName;

    COMPortName.clear();
    COMPortName.append( ComName );

    m_Handle = CreateFile( reinterpret_cast<const wchar_t *>( COMPortName.utf16() ), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_Handle == INVALID_HANDLE_VALUE) {

        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Can't open COM port<br>");

        this->closePort();
        return ;
    }

    //CRThread->m_Handle = m_Handle;
    //connect(CRThread, SIGNAL(RXPacket(QString, QString, QString)), this, SLOT(onComRead(QString, QString, QString)));

    TIMEOUT = 1000;
    COMMTIMEOUTS CommTimeOuts;

    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;

    if (!SetCommTimeouts(m_Handle, &CommTimeOuts)) {

        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Can't set COMM timeouts<br>");

        this->closePort();
        return ;
    };

    DCB ComDCM;
    memset( &ComDCM, 0, sizeof( ComDCM ) );
    ComDCM.DCBlength = sizeof( DCB );
    GetCommState( m_Handle, &ComDCM);

    ComDCM.BaudRate = ComSpeed; //9600
    ComDCM.ByteSize = 8;
    ComDCM.Parity = NOPARITY;
    ComDCM.StopBits = ONESTOPBIT;
    ComDCM.fAbortOnError = TRUE;
    ComDCM.fDtrControl = DTR_CONTROL_DISABLE;
    ComDCM.fRtsControl = RTS_CONTROL_DISABLE;
    ComDCM.fBinary = TRUE;
    ComDCM.fParity = FALSE;
    ComDCM.fInX = FALSE;
    ComDCM.fOutX = FALSE;
    ComDCM.XonChar = 0;
    ComDCM.XoffChar = uint8_t(0xFF);
    ComDCM.fErrorChar = FALSE;
    ComDCM.fNull = FALSE;
    ComDCM.fOutxCtsFlow = FALSE;
    ComDCM.fOutxDsrFlow = FALSE;
    ComDCM.XonLim = 128;
    ComDCM.XoffLim = 128;

    if (!SetCommState( m_Handle, &ComDCM )) {

        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::Can't set COMM state<br>");

        this->closePort();
        return ;
    };

    //send init commands
    DWORD feedback;
#endif

#ifdef Q_OS_UNIX
    int feedback = 0;
#endif

    //Command 1
    if (Command1.length()>1) {
        #ifdef Q_OS_WIN
            feedback = 0;
            WriteFile( m_Handle, Command1.data() , uint(Command1.size()), &feedback, 0 );
        #endif

        #ifdef Q_OS_UNIX
            feedback = 0;
            feedback = write(fd, Command1.data(), Command1.length());
        #endif
    }
    //Command 2
    if (Command2.length()>1) {
        #ifdef Q_OS_WIN
            feedback = 0;
            WriteFile( m_Handle, Command2.data() , uint(Command2.size()), &feedback, 0 );
        #endif

        #ifdef Q_OS_UNIX
            feedback = 0;
            feedback = write(fd, Command2.data(), Command2.length());
        #endif
    }
    //Command 3
    if (Command3.length()>1) {
        #ifdef Q_OS_WIN
            feedback = 0;
            WriteFile( m_Handle, Command3.data() , uint(Command3.size()), &feedback, 0 );
        #endif

        #ifdef Q_OS_UNIX
            feedback = 0;
            feedback = write(fd, Command3.data(), Command3.length());
        #endif
    }
    //Command 4
    if (Command4.length()>1) {
        #ifdef Q_OS_WIN
            feedback = 0;
            WriteFile( m_Handle, Command4.data() , uint(Command4.size()), &feedback, 0 );
        #endif

        #ifdef Q_OS_UNIX
            feedback = 0;
            feedback = write(fd, Command4.data(), Command4.length());
        #endif
    }

    //send KISS parameters
    QByteArray KISSParam;
    //01 - TXDelay
    KISSParam.clear();
    KISSParam.append( 192 );
    KISSParam.append( 1 );
    KISSParam.append( uchar(TXDelay) );
    KISSParam.append( 192 );
    #ifdef Q_OS_WIN
        feedback = 0;
        WriteFile( m_Handle, KISSParam.data() , uint(KISSParam.size()), &feedback, 0 );
    #endif
    #ifdef Q_OS_UNIX
        feedback = 0;
        feedback = write(fd, KISSParam.data(), KISSParam.length());
    #endif

    //02 - Persistance
    KISSParam.clear();
    KISSParam.append( 192 );
    KISSParam.append( 2 );
    KISSParam.append( uchar(Persist) );
    KISSParam.append( 192 );
    #ifdef Q_OS_WIN
        feedback = 0;
        WriteFile( m_Handle, KISSParam.data() , uint(KISSParam.size()), &feedback, 0 );
    #endif
    #ifdef Q_OS_UNIX
        feedback = 0;
        feedback = write(fd, KISSParam.data(), KISSParam.length());
    #endif

    //03 - SLotTime
    KISSParam.clear();
    KISSParam.append( 192 );
    KISSParam.append( 3 );
    KISSParam.append( uchar(SlotTime) );
    KISSParam.append( 192 );
    #ifdef Q_OS_WIN
        feedback = 0;
        WriteFile( m_Handle, KISSParam.data() , uint(KISSParam.size()), &feedback, 0 );
    #endif
    #ifdef Q_OS_UNIX
        feedback = 0;
        feedback = write(fd, KISSParam.data(), KISSParam.length());
    #endif

    //04 - TXTail
    KISSParam.clear();
    KISSParam.append( 192 );
    KISSParam.append( 4 );
    KISSParam.append( uchar(TXTail) );
    KISSParam.append( 192 );
    #ifdef Q_OS_WIN
        feedback = 0;
        WriteFile( m_Handle, KISSParam.data() , uint(KISSParam.size()), &feedback, 0 );
    #endif
    #ifdef Q_OS_UNIX
        feedback = 0;
        feedback = write(fd, KISSParam.data(), KISSParam.length());
    #endif

    //05 - FullDuplex
    KISSParam.clear();
    KISSParam.append( 192 );
    KISSParam.append( 5 );
    KISSParam.append( uchar(FullDuplex) );
    KISSParam.append( 192 );
    #ifdef Q_OS_WIN
        feedback = 0;
        WriteFile( m_Handle, KISSParam.data() , uint(KISSParam.size()), &feedback, 0 );
    #endif
    #ifdef Q_OS_UNIX
        feedback = 0;
        feedback = write(fd, KISSParam.data(), KISSParam.length());
    #endif



#ifdef Q_OS_UNIX
 	struct termios options;
 	
	/* ПФЛТЩФЙЕ РПТФБ */
	fd = open("/dev/ttyd0", O_RDWR | O_NOCTTY | O_NDELAY);
	fcntl(fd, F_SETFL, FNDELAY);
	
	/* РПМХЮЕОЙЕ ФЕЛХЭЙИ ПРГЙК */
	tcgetattr(fd, &options);
	
	/* ХУФБОПЧЛБ ОЕУФТХЛФХТЙТПЧБООПЗП ЧЧПДБ, 1-УЕЛХОДОЩК ФБКНБХФ */
	options.c_cflag     |= (CLOCAL | CREAD);
	options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag     &= ~OPOST;
	options.c_cc[VMIN]  = 0;
	options.c_cc[VTIME] = 10;
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);	
	/* ХУФБОПЧЛБ ПРГЙК */
	tcsetattr(fd, TCSANOW, &options); 	
 
#endif

    connect(beacontimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    if (BeaconInterval > 0) beacontimer->start( BeaconInterval ); else beacontimer->stop();

    //com portus assincronus readus timerus, every 0.1 sec
    connect(ComReadtimer, SIGNAL(timeout()), this, SLOT(onComReadTimer()));
    ComReadtimer->start( 100 );

    //CRThread->start();

}

void QAPRSKISSPORT::closePort() {
    QByteArray KISSExitCommand;
    QString TExitFromKISS;

#ifdef Q_OS_WIN

    DWORD feedback;

    TExitFromKISS=ExitFromKISS;
    if (ExitFromKISS!="") {
        KISSExitCommand.clear();
            while (TExitFromKISS.length()>0) {
                //log->insertHtml("("+QString::number(PortNum)+")"+PortType+"::1="+TExitFromKISS.mid(0, TExitFromKISS.indexOf(",") )+"<br>");
                //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

                KISSExitCommand.append( TExitFromKISS.mid(0, TExitFromKISS.indexOf(",") ).toInt() );

                if (TExitFromKISS.indexOf(",")==-1)
                        TExitFromKISS.clear();
                            else TExitFromKISS = TExitFromKISS.mid( TExitFromKISS.indexOf(",")+1 );
            }
                    feedback = 0;
        WriteFile( m_Handle, KISSExitCommand.data() , uint(KISSExitCommand.size()), &feedback, 0 );
    }
#endif

    active = false;
    beacontimer->stop();
    ComReadtimer->stop();
    //CRThread->terminate();

    beacontimer->disconnect();
    ComReadtimer->disconnect();
    //CRThread->disconnect();

#ifdef Q_OS_WIN
    CloseHandle( m_Handle );
    m_Handle = 0;
#endif

#ifdef Q_OS_UNIX
	close(fd); 
#endif

    emit ToLog("("+QString::number(PortNum)+")"+PortType+"::closePort<br>");
}

void QAPRSKISSPORT::onTimer() {
    QString Beacon;

    Beacon.append( '=' );
    Beacon.append( Latitude.left( 2 ) );
    Beacon.append( Latitude.mid( 3 ) );
    Beacon.append( Symbol[0] );
    Beacon.append( Longitude.left( 3 ) );
    Beacon.append( Longitude.mid( 4 ) );
    Beacon.append( Symbol[1]+BeaconText+" {QAPRS}" );

    this->sendPacket( "APU25N", Call + ">" + UnprotoAddress, Beacon );

}

void QAPRSKISSPORT::onComReadTimer() {
#ifdef Q_OS_WIN
// Test if data in inQueue(outQueue)
    QByteArray ComData;
    QByteArray OneMsg;

    COMSTAT ComStat;
    DWORD   e, aInQueue;
    if (ClearCommError(m_Handle, &e, &ComStat)) {
        aInQueue = ComStat.cbInQue;
        if (aInQueue>0) {
            //log->insertHtml("("+QString::number(PortNum)+")"+PortType+"::ready fr read "+QString::number(aInQueue)+" bytes<br>");
            //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
            //read com data

            static char *buffer = NULL;
            if(!buffer) { buffer = (char *)malloc(1024); };
            DWORD len = 1024;
            DWORD feedback = 0;
            ReadFile(m_Handle, buffer, len, &feedback, NULL);
            //qDebug() << "data readed";

            if (feedback>0) {
                ComData.clear();
                ComData.resize(feedback);
                for(DWORD i=0;i<feedback;i++){
                    ComData.data()[i] = buffer[i];
                }
            }

        }

    }
#endif

#ifdef Q_OS_UNIX

    QByteArray ComData;
    QByteArray OneMsg;

    char buffer[1024];  /* ВХЖЕТ ЧЧПДБ */
    char *bufptr;      /* ФЕЛХЭЙК УЙНЧПМ Ч ВХЖЕТЕ */
    int  nbytes;       /* ЮЙУМП РТПЮЙФБООЩИ ВБКФПЧ*/
    int feedback = 0;

    //read data from port
    bufptr = buffer;
    while ((nbytes = read(fd, bufptr, buffer + sizeof(buffer) - bufptr - 1)) > 0) {
        bufptr += nbytes;
        feedback += nbytes;
    };
		    
    //if data readed then append data to buffer ComReadData
    if (feedback>0) {
        for(int i=0;i<feedback;i++) {
            ComReadData.append( buffer[i] );
        }
    }

    //if buffer start from 0xc0 and end 0xc0 then data read complete
    if ( (uchar(ComReadData.data()[0])==uchar(0xc0)) and (uchar(ComReadData.data()[ ComReadData.length()-1 ])==uchar(0xc0))  ){
        ComData.clear();
        ComData.append( ComReadData );
        ComReadData.clear();
    }
 

#endif            

            if (ComData.length()>0)     {

                //ComData = QByteArray::fromHex("c00082a0aa646a9ce0aa82669aa29460ae92888a624062ae92888a64406503f03d353830312e38334e2f30333835312e313345232841464b292032392e3235303c2d3e31342e313033207b55495633324e7d0dc0c00082a0aa646a9ce0aa82669aa29460ae92888a624062ae92888a64406503f03d353830312e38334e2f30333835312e313345232841464b292032392e3235303c2d3e31342e313033207b55495633324e7d0dc0");
                while ( ComData.length()>0 ) {
                    //FEND 0xC0
                    //log->insertHtml("("+QString::number(PortNum)+")"+PortType+":: "+ComData.toHex() +"<br>");
                    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

                    if ( ((ComData.data()[0])==char(0xC0)) and ((ComData.data()[1])==char(0x00)) ) {

                        ComData = ComData.mid( 2 ); //delete first FEND and command 0
                        OneMsg = ComData.left( ComData.indexOf( 0xC0 ) );

                        int i;
                        QString DestCall( "" );
                        unsigned char DestSID;
                        for( i=1; i<=6; i++ ) {
                            if ( ( uchar( OneMsg.data()[i-1] ) >> 1 )!=' ' )
                                DestCall += uchar( OneMsg.data()[i-1] ) >> 1;
                        };

                        DestSID = ( uchar( OneMsg.data()[6] ) >> 1 ) & 15;
                        if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

                        int sh;
                        sh = 7;
                        QString SrcCall( "" );
                        unsigned char SrcSID;
                        do {
                            for( i=1; i<=6; i++ ) {
                                if ( ( uchar( OneMsg.data()[sh+i-1] ) >> 1 )!=' ' )
                                    SrcCall += uchar( OneMsg.data()[sh+i-1] ) >> 1;
                            };
                            SrcSID = ( uchar( OneMsg.data()[sh+6] ) >> 1 ) & 15;             //SID
                            if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
                            if ( ( ( uchar( OneMsg.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
                            if ( ( ( uchar( OneMsg.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
                            sh += 7;
                        } while ( ( ( uchar( OneMsg.data()[sh-1] ) ) & 1 ) !=1 );


                        //replace first ',' of the Src to '>'
                        if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

                        QString MsgText( "" );
                        MsgText = OneMsg.mid( sh + 2, OneMsg.count() - sh - 3 ); // +2 - skip Control byte and PID byte
                                                                                     // -5 - skip ^^ and 2 byte of CRC-16 (NO IN KISS)
                        emit RXPacket( PortNum, DestCall, SrcCall, MsgText );

                    } else {

                        //log->insertHtml("("+QString::number(PortNum)+")"+PortType+"::ignore packet<br>");
                        //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
                        emit ToLog("("+QString::number(PortNum)+")"+PortType+"::ignore packet<br>");

                        ComData = ComData.mid( 2 ); //delete first FEND and command 0
                    }

                    ComData = ComData.mid( ComData.indexOf( 0xC0 ) + 1 );

                }
            }

}


void QAPRSKISSPORT::onComRead(QString DestCall, QString SrcCall, QString MsgText) {
    emit RXPacket( PortNum, DestCall, SrcCall, MsgText );
}

void QAPRSKISSPORT::sendPacket( QString To, QString From, QString MsgText ){

    QAPRSKISSPacket Packet( To, From, MsgText );
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );
    //log->insertHtml(Packet.toHex()+"<br>");
    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::"+MsgText+"<br>" );

    #ifdef Q_OS_WIN
        DWORD feedback = 0;
        WriteFile( m_Handle, Packet.data() , int(Packet.size()), &feedback, 0 );
        //if (!WriteFile( m_Handle, Packet.data() , uint(Packet.size()), &feedback, 0 ) || feedback!=uint(Packet.size())) {
        //    log->insertHtml("("+QString::number(PortNum)+")"+PortType+"::Write error<br>");
        //    log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
        //    this->closePort();
        //    return ;
        //}
    #endif
    
    #ifdef Q_OS_UNIX
    	int feedback = 0;
    	feedback = write(fd, Packet.data(), Packet.length());
 	    emit ToLog( "(" + QString::number( PortNum ) + ")" + PortType + "::TX feedback="+QString::number(feedback)+"<br>" ); 
    
    #endif

    emit TXPacket( PortNum, To, From, MsgText );

}



