#include <QDebug>
#include "../../core/include/qaprsports.h"


//базовый класс
     QAPRSPort::QAPRSPort( QObject *parent ) { }
     QAPRSPort::~QAPRSPort() { }
void QAPRSPort::openPort() { }
void QAPRSPort::closePort() { }
void QAPRSPort::setParam( QString ParamName, QString ParamValue ) {}
void QAPRSPort::updatePort() { }
void QAPRSPort::sendAPRSPacket( QString To, QString From, QString MsgText ) { }
void QAPRSPort::sendAXPacket( QByteArray AXPacket ) { }


// реализация AXIP порта
QAPRSAXIPPORT::QAPRSAXIPPORT( QObject *parent ) {

    this->setPortType( "AXIP" );
    this->setPortName( "n\\a" );
    this->setPortStatus( 1 );
    this->setRXPacket( FALSE );
    this->setTXPacket( FALSE );
    this->setBUSY( FALSE );

    udpSocket   = new QUdpSocket(this);
    TXAddress   = new QHostAddress();
    TXAddress->setAddress( "127.0.0.1" );
    RXPort      = 95;
    TXPort      = 95;

    qDebug() << "Port::Create(AXIP)";
    

}

QAPRSAXIPPORT::~QAPRSAXIPPORT() {

    udpSocket->disconnect();
    delete udpSocket;
    delete TXAddress;

    udpSocket = NULL;
    TXAddress = NULL;

}

void QAPRSAXIPPORT::openPort(){

    setPortStatus( 0 ); //включен

    connect( udpSocket, SIGNAL( readyRead() ),
             this,      SLOT( readPendingDatagrams() ) );

    udpSocket->bind( QHostAddress::LocalHost, RXPort );

    changeBUSYState( portNum(), isBUSY() );

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSAXIPPORT::closePort(){

    setPortStatus( 1 );
    udpSocket->abort();
    udpSocket->disconnect();
    qDebug() << ( tr( "(%1)%2::closePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSAXIPPORT::setParam( QString ParamName, QString ParamValue ){

    //qDebug() << "setParam ParamName=" << ParamName << "; ParamValue=" << ParamValue;
    if ( ParamName=="RXPort" ) {
        if ((ParamValue.toInt()>0)and(ParamValue.toInt()<65536)) RXPort = ParamValue.toInt();
        //qDebug() << "RXPort=" << RXPort;
    }

    if ( ParamName=="TXPort" ) {
        if ((ParamValue.toInt()>0)and(ParamValue.toInt()<65536)) TXPort = ParamValue.toInt();
        //qDebug() << "TXPort=" << TXPort;
    }

    if ( ParamName=="IPAddress" ) {
        TXAddress->setAddress( ParamValue );
        //qDebug() << "IPAddress=" << TXAddress;
    }

}

void QAPRSAXIPPORT::updatePort(){

    udpSocket->close();
    udpSocket->disconnect();

    connect( udpSocket, SIGNAL( readyRead() ),
             this,      SLOT( readPendingDatagrams() ) );

    udpSocket->bind( RXPort );
    qDebug() << ( tr( "(%1)%2::updatePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSAXIPPORT::sendAPRSPacket( QString To, QString From, QString MsgText ){

    QAPRSAXIPPacket Packet( To, From, MsgText );
    if ( portStatus() == 0 ) {
        emit TXAPRSPacket( portNum(), To, From, MsgText );

        this->sendAXPacket( Packet );
    }

}

void QAPRSAXIPPORT::sendAXPacket( QByteArray AXPacket ) {

    if ( portStatus() == 0 ) {
        emit TXPacket( portNum(), TRUE );
        udpSocket->writeDatagram( AXPacket, *TXAddress, TXPort );
        emit TXAXPacket( portNum(), AXPacket );
        emit TXPacket( portNum(), FALSE );
    }

}

void QAPRSAXIPPORT::readPendingDatagrams() {
    
    QByteArray datagram;
    QString DataIn;

    QHostAddress sender;
    quint16 senderPort;

    emit RXPacket( portNum(), TRUE );

    datagram.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(datagram.data(), datagram.size(),
                            &sender, &senderPort);

    DataIn = datagram.data();

    //qDebug() << "AXIP PORT RX PNUM=" << portNum();

    emit RXAXPacket( portNum(), datagram );
    emit RXPacket( portNum(), FALSE );

    int i;
    QString DestCall( "" );
    unsigned char DestSID;
    for( i=1; i<=6; i++ ) {
        if ( ( uchar( datagram.data()[i-1] ) >> 1 )!=' ' )
            DestCall += uchar( datagram.data()[i-1] ) >> 1;
    };

    DestSID = ( uchar( datagram.data()[6] ) >> 1 ) & 15;
    if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

    int sh;
    sh = 7;
    QString SrcCall( "" );
    unsigned char SrcSID;
    do {
        for( i=1; i<=6; i++ ) {
            if ( ( uchar( datagram.data()[sh+i-1] ) >> 1 )!=' ' )
                SrcCall += uchar( datagram.data()[sh+i-1] ) >> 1;
        };
        SrcSID = ( uchar( datagram.data()[sh+6] ) >> 1 ) & 15;             //SID
        if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
        if ( ( ( uchar( datagram.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
        if ( ( ( uchar( datagram.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
        sh += 7;
    } while ( ( ( uchar( datagram.data()[sh-1] ) ) & 1 ) !=1 );

    //заменяем первую запятую в списке позывных на '>'
    if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

    QString MsgText( "" );
    MsgText = datagram.mid( sh + 2, datagram.count() - sh - 5 ); // +2 - skip Control byte and PID byte

    emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );

}

// реализация INTERNET порта
QAPRSINTERNETPORT::QAPRSINTERNETPORT( QObject *parent ) {

    this->setPortType( "Inet" );
    this->setPortName( "n\\a" );
    this->setPortStatus( 1 );
    this->setRXPacket( FALSE );
    this->setTXPacket( FALSE );
    this->setBUSY( FALSE );

    UserName = "";
    UserPass = "";
    APRSFilter = "";
    APRSServer1 = "127.0.0.1:14000";
    APRSServer2 = "127.0.0.1:14000";
    APRSServer3 = "127.0.0.1:14000";
    UseProxy = FALSE;
    UseProxyAuth = FALSE;
    ProxyName = "";
    ProxyPort = 80;
    ProxyUser = "";
    ProxyPass = "";

    tcpClient   = new QTcpSocket(this);
    reconnecttimer = new QTimer(this);
    srvnum      = 0;

    qDebug() << "Port::Create(Inet)";

}

QAPRSINTERNETPORT::~QAPRSINTERNETPORT() {

    tcpClient->close();
    tcpClient->disconnect();
    delete tcpClient;
    tcpClient = NULL;

    reconnecttimer->stop();
    reconnecttimer->disconnect();
    delete reconnecttimer;
    reconnecttimer = NULL;

}

void QAPRSINTERNETPORT::openPort(){

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );
    qDebug() << ( tr( "(%1)%2::Connecting to APRS server %3 ...<br>" ).arg( portNum() ).arg( portType() ).arg( srvnum + 1 ) );

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected ()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpClientconnected ()));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpClienterror(QAbstractSocket::SocketError)));

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));


    QString APRSAddress;
    QString APRSPort;
    QByteArray ConnectionString;

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
    ConnectionString.append( "user " + UserName + " pass " + UserPass + " vers QAPRS v.002 filter " + APRSFilter + "\n" );

    //emit ToLog("("+QString::number(PortNum)+")"+PortType+tr("::APRS Server Address = '")+APRSAddress+"'<br>");
    //emit ToLog("("+QString::number(PortNum)+")"+PortType+tr("::APRS Server Port = '")+APRSPort+"'<br>");
    qDebug() << ( tr( "(%1)%2::APRS Server Address = <br>" ).arg( portNum() ).arg( portType() ).arg( APRSAddress ) );
    qDebug() << ( tr( "(%1)%2::APRS Server Port = <br>" ).arg( portNum() ).arg( portType() ).arg( APRSPort ) );

    setPortStatus( 0 ); //включен

    QNetworkProxy proxy;

    if ( UseProxy == TRUE ) {
        qDebug() << "Use PROXY";
        proxy.setType( QNetworkProxy::HttpProxy );
        qDebug() << "Proxy = " << ProxyName;
        qDebug() << "Port = " << ProxyPort;
        qDebug() << "ProxyUser = " << ProxyUser;
        qDebug() << "ProxyPass = " << ProxyPass;
        proxy.setHostName( ProxyName );
        proxy.setPort( ProxyPort );
        proxy.setUser( ProxyUser );
        proxy.setPassword( ProxyPass );
    };

    if ( UseProxy == FALSE ) {
        qDebug() << "No PROXY";
        proxy.setType( QNetworkProxy::NoProxy );
    };


    QNetworkProxy::setApplicationProxy(proxy);
    tcpClient->setProxy( proxy );

    tcpClient->connectToHost( APRSAddress, APRSPort.toInt() );

    tcpClient->write( ConnectionString );
//===========================================
    changeBUSYState( portNum(), isBUSY() );

}

void QAPRSINTERNETPORT::closePort(){

    setPortStatus( 1 ); //отключен
    qDebug() << ( tr( "(%1)%2::closePort<br>" ).arg( portNum() ).arg( portType() ) );

    tcpClient->abort();
    reconnecttimer->stop();

    tcpClient->disconnect();
    reconnecttimer->disconnect();

}

void QAPRSINTERNETPORT::setParam( QString ParamName, QString ParamValue ){

    if ( ParamName=="User")            UserName = ParamValue;
    if ( ParamName=="Pass")            UserPass = ParamValue;
    if ( ParamName=="Filter")          APRSFilter = ParamValue;
    if ( ParamName=="Server1")         APRSServer1 = ParamValue;
    if ( ParamName=="Server2")         APRSServer2 = ParamValue;
    if ( ParamName=="Server3")         APRSServer3 = ParamValue;
    if ( ParamName=="Proxy")           UseProxy = (ParamValue=="T");
    if ( ParamName=="PAuth")           UseProxyAuth = (ParamValue=="T");
    if ( ParamName=="PName")           ProxyName = ParamValue;
    if ( ParamName=="PPort")           ProxyPort = ParamValue.toInt();
    if ( ParamName=="PUser")           ProxyUser = ParamValue;
    if ( ParamName=="PPass")           ProxyPass = ParamValue;

}

void QAPRSINTERNETPORT::updatePort() {

    this->closePort();
    this->openPort();

    qDebug() << ( tr( "(%1)%2::updatePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSINTERNETPORT::sendAPRSPacket( QString To, QString From, QString MsgText ){

    QAPRSAXIPPacket Packet( To, From, MsgText );

    QByteArray APRSPacket;

    if ( portStatus() == 0 ) {
        APRSPacket.clear();
        APRSPacket.append( From.left( From.indexOf('>') ) + ">"+APRSCall()+",TCPIP*:" + MsgText);

        APRSPacket.append( 13 );
        APRSPacket.append( 10 );

        tcpClient->write(APRSPacket);
        emit TXAPRSPacket( portNum(), To, From, MsgText );

        this->sendAXPacket( APRSPacket );
    }

}

void QAPRSINTERNETPORT::sendAXPacket( QByteArray AXPacket ) {

    if ( portStatus() == 0 ) {
        emit TXPacket( portNum(), TRUE );
        emit TXAXPacket( portNum(), AXPacket );
        emit TXPacket( portNum(), FALSE );
    }

}

void QAPRSINTERNETPORT::tcpClientread() {

    QByteArray msg;

    //while ( tcpClient->bytesAvailable() == 0 );
    msg = tcpClient->readAll();

    QString mesg;
    QString MsgText;
    QString SrcCall;
    QString DestCall;

    while ( msg.length() > 0 ) {

       mesg = msg.left( msg.indexOf( 10 ) + 1 );

        if ( mesg.data()[ 0 ] != '#' ) {


            DestCall = mesg.mid( mesg.indexOf( ">" ) + 1, mesg.indexOf( "," ) - mesg.indexOf( ">" ) - 1 );
            //qDebug() << ( "(" + QString::number( portNum() ) + ")" + portType() + ":: destcall=" + DestCall + " <br>" );

            SrcCall = mesg.left( mesg.indexOf( ">" ) ) + mesg.mid( mesg.indexOf( "," ), mesg.indexOf( ":" ) - mesg.indexOf( "," ) );
            if ( SrcCall.indexOf( ',' ) != -1 ) SrcCall[ SrcCall.indexOf( ',' ) ] = '>';
            //qDebug() << ( "(" + QString::number( portNum() ) + ")" + portType() + ":: SrcCall=" + SrcCall + " <br>" );

            MsgText = mesg.mid( mesg.indexOf( ":" ) + 1, mesg.length() - mesg.indexOf( ":" ) - 3 );
            //qDebug() << ( "(" + QString::number( portNum() ) + ")" + portType() + ":: MsgText=" + MsgText + " <br>" );

            emit RXPacket( portNum(), TRUE );

            QAPRSAXIPPacket Packet( DestCall, SrcCall, MsgText );
            emit RXAXPacket( portNum(), Packet );
            emit RXPacket( portNum(), FALSE );
            emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );



        } else {
            emit RXPacket( portNum(), TRUE );
            emit RXPacket( portNum(), FALSE );
            //log->insertHtml("(" + QString::number( PortNum ) + ")" + PortType + "::SRVMsg='");
            //log->insertPlainText(mesg.left(mesg.length() - 2) );
            //log->insertHtml("'<br>");
            //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

            //re-remark for show APRS server messages in system log
            //emit ToLog( "(" + QString::number( PortNum ) + ")" + PortType + "::SRVMsg='" + mesg.left(mesg.length() - 2) + "'<br>" );
        };

        msg = msg.mid( msg.indexOf(10) + 1 );

    };

};

void QAPRSINTERNETPORT::onReconnectTimer() {

    qDebug() << ( tr( "(%1)%2::try reconnect to APRS server<br>" ).arg( portNum() ).arg( portType() ) );

    reconnecttimer->stop();
    reconnecttimer->disconnect();

    //select next aprs server
    srvnum++;
    if (srvnum>2) srvnum = 0;

    this->openPort();

}

void QAPRSINTERNETPORT::tcpClientconnected () {
};

void QAPRSINTERNETPORT::tcpClientdisconnected () {

    qDebug() << ( tr( "(%1)%2::disconnected<br>" ).arg( portNum() ).arg( portType() ) );
    this->closePort();

};

void QAPRSINTERNETPORT::tcpClienterror (QAbstractSocket::SocketError socketError) {

    qDebug() << ( tr( "(%1)%2::Clienterror<br>" ).arg( portNum() ).arg( portType() ) );
    this->closePort();
    setPortStatus( 2 ); //ошибка

    //start reconnect timer
    connect(reconnecttimer, SIGNAL( timeout() ), this, SLOT( onReconnectTimer() ) );
    reconnecttimer->start( 30000 );

};

// реализация AGW порта
QAPRSAGWPORT::QAPRSAGWPORT( QObject *parent ) {

    this->setPortType( "AGW" );
    this->setPortName( "n\\a" );
    this->setPortStatus( 1 );
    this->setRXPacket( FALSE );
    this->setTXPacket( FALSE );
    this->setBUSY( FALSE );

    agwHost = "127.0.0.1";
    agwPort = 1;

    tcpClient = new QTcpSocket(this);
    reconnecttimer = new QTimer(this);

    qDebug() << "Port::Create(AGW)";

}

QAPRSAGWPORT::~QAPRSAGWPORT() {

    tcpClient->close();
    tcpClient->disconnect();
    delete tcpClient;
    tcpClient = NULL;

    reconnecttimer->stop();
    reconnecttimer->disconnect();
    delete reconnecttimer;
    reconnecttimer = NULL;

}

void QAPRSAGWPORT::openPort(){

    QHostAddress AGWHostAddress;

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );
    qDebug() << ( tr( "(%1)%2::connecting to AGW core<br>" ).arg( portNum() ).arg( portType() ) );
    setPortStatus( 0 ); //включен

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected ()));

    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpClientconnected ()));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpClienterror(QAbstractSocket::SocketError)));

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));

    //emit ToLog("("+QString::number(PortNum)+")"+PortType+tr("::AGW Host = '") + agwHost + "'<br>");
    qDebug() << ( tr( "(%1)%2::AGW Host = %3<br>" ).arg( portNum() ).arg( portType() ).arg( agwHost ) );

    AGWHostAddress.setAddress( agwHost );

    tcpClient->connectToHost(AGWHostAddress , 8000);
    agwStatus = 0; //set status - connect to AGW core

    QByteArray Pkt;
    Pkt.fill(0, 36);
    Pkt[4]='R';//
    tcpClient->write( Pkt ); //seng qry "Port Information"

    changeBUSYState( portNum(), isBUSY() );

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSAGWPORT::closePort(){

    setPortStatus( 1 ); //отключен
    qDebug() << ( tr( "(%1)%2::closePort<br>" ).arg( portNum() ).arg( portType() ) );

    reconnecttimer->stop();
    tcpClient->close();

    tcpClient->disconnect();
    reconnecttimer->disconnect();

}

void QAPRSAGWPORT::updatePort(){

    this->closePort();
    this->openPort();

    qDebug() << ( tr( "(%1)%2::updatePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSAGWPORT::setParam( QString ParamName, QString ParamValue ) {

    if ( ParamName=="Host")            agwHost = ParamValue;
    if ( ParamName=="AGWPort")         agwPort = ParamValue.toInt();

}

void QAPRSAGWPORT::tcpClientconnected () {

};

void QAPRSAGWPORT::tcpClientdisconnected () {

    qDebug() << ( tr( "(%1)%2::Disconnected<br>" ).arg( portNum() ).arg( portType() ) );

    this->closePort();

};

void QAPRSAGWPORT::tcpClienterror (QAbstractSocket::SocketError socketError) {

    qDebug() << ( tr( "(%1)%2::Clienrerror<br>" ).arg( portNum() ).arg( portType() ) );

    this->closePort();
    setPortStatus( 2 ); //ошибка

    //start reconnect timer
    connect(reconnecttimer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));
    reconnecttimer->start( 30000 );

};

void QAPRSAGWPORT::onReconnectTimer() {

    qDebug() << ( tr( "(%1)%2::Try reconnect to AGW core<br>" ).arg( portNum() ).arg( portType() ) );

    reconnecttimer->stop();
    reconnecttimer->disconnect();
    this->openPort();

}

void QAPRSAGWPORT::tcpClientread() {

    QByteArray mesg;

    while (tcpClient->bytesAvailable()==0);

    mesg = tcpClient->readAll();

    //qDebug() << ( tr( "(%1)%2::Client read" ).arg( portNum() ).arg( portType() ) );

    if (agwStatus==0) { //если статус = 0 то идет соединение с AGW
        agwStatus=1;
        QByteArray Pkt;
        Pkt.fill(0, 36);
        Pkt[4]='G';//
        tcpClient->write( Pkt ); //посылаем запрос - "Port Information" для получения списка портов
    } else
    if (agwStatus==1) { //если статус = 1 то значит запрос мы посылали и пришедшие данные - есть список портов
        //чтение данных о списке портов AGW

        mesg = mesg.mid(36);

        qDebug() << ("("+QString::number( portNum() )+")"+portType()+tr("::AGW Ports information<br>") );
        qDebug() << ("("+QString::number( portNum() )+")"+portType()+tr("::Ports count = ")+mesg.left(mesg.indexOf(';'))+"<br>");

        if ( agwPort>( mesg.left(mesg.indexOf(';')) ).toInt() ) {
            qDebug() << ( tr( "(%1)%2::Ports number (%3) not support on AGW Core!</font><br>" ).arg( portNum() ).arg( portType() ).arg( agwPort ) );

            this->closePort();
            return ;
        }

        mesg = mesg.mid( mesg.indexOf( ';' ) + 1 );

        do  {

            qDebug() << ( tr( "(%1)%2::" ).arg( portNum() ).arg( portType() ) + ( mesg.left(mesg.indexOf(';')) ) );
            mesg = mesg.mid( mesg.indexOf( ';' ) + 1);

        } while ( mesg.indexOf( ';' ) !=-1 );

        agwStatus = 2;
        QByteArray Pkt;
        Pkt.fill(0, 36);
        Pkt[4]='k';
        tcpClient->write( Pkt ); //послать запрос на мониторинг данных в raw

        //теперь порт можно считать открытым
        setPortStatus( 0 ); //включен
    } else
    if (agwStatus==2) { //тут уже данные

        if ((uchar(mesg.data()[0])+1)==agwPort) {
            //если пакет для моего порта
            //!!!надо бы проверить на многопакетовость

            QString DataIn;
            DataIn = mesg.data();

            mesg = mesg.mid( 37 );
            emit RXPacket( portNum(), TRUE );
            emit RXPacket( portNum(), FALSE );

            int i;
            QString DestCall( "" );
            unsigned char DestSID;
            for( i=1; i<=6; i++ ) {
                if ( ( uchar( mesg.data()[i-1] ) >> 1 )!=' ' )
                    DestCall += uchar( mesg.data()[i-1] ) >> 1;
            };

            DestSID = ( uchar( mesg.data()[6] ) >> 1 ) & 15;
            if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

            int sh;
            sh = 7;
            QString SrcCall( "" );
            unsigned char SrcSID;
            do {
                for( i=1; i<=6; i++ ) {
                    if ( ( uchar( mesg.data()[sh+i-1] ) >> 1 )!=' ' )
                        SrcCall += uchar( mesg.data()[sh+i-1] ) >> 1;
                };
                SrcSID = ( uchar( mesg.data()[sh+6] ) >> 1 ) & 15;             //SID
                if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
                if ( ( ( uchar( mesg.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
                if ( ( ( uchar( mesg.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit
                sh += 7;
            } while ( ( ( uchar( mesg.data()[sh-1] ) ) & 1 ) !=1 );

            //replace first ',' of the Src to '>'
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';


            QString MsgText( "" );
            MsgText = mesg.mid( sh + 2 , mesg.count() - sh - 3 ); // +2 - skip Control byte and PID byte
                                                                         // -5 - skip ^^ and 2 byte of CRC-16

            QAPRSAXIPPacket Packet( DestCall, SrcCall, MsgText );
            emit TXAXPacket( portNum(), Packet );
            emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );

        }

    };

};


void QAPRSAGWPORT::sendAPRSPacket( QString To, QString From, QString MsgText ){
/*
    QAPRSAXIPPacket Packet( To, From, MsgText );

    QByteArray APRSPacket;

    APRSPacket.clear();
    APRSPacket.append( From.left( From.indexOf('>') ) + ">"+APRSCall+",TCPIP*:" + MsgText);

    APRSPacket.append( 13 );
    APRSPacket.append( 10 );

    tcpClient->write(APRSPacket);
    emit TXAPRSPacket( portNum(), To, From, MsgText );

    this->sendAXPacket( APRSPacket );
*/

    QAPRSAXIPPacket Packet( To, From, MsgText );

    if ( portStatus() == 0 ) {
        emit TXAPRSPacket( portNum(), To, From, MsgText );
        this->sendAXPacket( Packet );
    }

    //Packet.data()[0] = agwPort - 1;
    //log->insertPlainText("("+QString::number(PortNum)+")"+PortType+"::Packet= "+Packet.toHex()+"<br>");



}

void QAPRSAGWPORT::sendAXPacket( QByteArray AXPacket ) {
//а теперь тут надо сделать из ax пакета agw пакет

    QByteArray AGWHeader;
    //020000004b00000000000000000000000000000000000000000000002d0000000000000000+пакет без CRC, eof 0x0d
    //                                                         ^-длина пакета (pack len - 36)
    //         ^- 'K' - посылка raw пакета
    // ^-номер порта в ядре agw ( agwPort-1 )

    if ( portStatus() == 0 ) {
        AGWHeader.clear();
        AGWHeader.fill(0, 37);
        AGWHeader.data()[4] = 0x4b; //^- 'K' - посылка raw пакета

        //AXPacket = AXPacket.left( AXPacket.length() - 2 ); //delete CRC
        //берем AX пакет и отрезаем от него CRC
        AXPacket.remove( AXPacket.length() - 2, 2 );
        AXPacket.prepend( AGWHeader );

        AXPacket.data()[28] = (AXPacket.length() - 36);
        AXPacket.data()[29] = (AXPacket.length() - 36)>>8;

        emit TXPacket( portNum(), TRUE );
        tcpClient->write( AXPacket );
        emit TXPacket( portNum(), FALSE );
    }

}


// реализация KISS порта
QAPRSKISSPORT::QAPRSKISSPORT( QObject *parent ) {

    this->setPortType( "KISS" );
    this->setPortName( "n\\a" );
    this->setPortStatus( 1 );
    this->setRXPacket( FALSE );
    this->setTXPacket( FALSE );
    this->setBUSY( FALSE );

    ComReadtimer = new QTimer(this);

    ComName.clear();
    ComName.append( "COM1" );

#ifdef Q_OS_WIN
    ComSpeed = 9600;
#endif
    TXDelay = 40;
    TXTail = 4;
    SlotTime = 10;
    Persist = 64;
    FullDuplex = TRUE;
    ExitFromKISS = "192,255,192";

    Command1.clear();
    Command2.clear();
    Command3.clear();
    Command4.clear();

}

QAPRSKISSPORT::~QAPRSKISSPORT() {

    ComReadtimer->stop();
    ComReadtimer->disconnect();
    delete ComReadtimer;
    ComReadtimer = NULL;

}

void QAPRSKISSPORT::setParam( QString ParamName, QString ParamValue ) {

    if ( ParamName=="COM") {
                                       ComName.clear();
                                       ComName.append( ParamValue );
    };
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
    };
    if ( ParamName=="Command2") {
        Command2.clear();
        Command2.append( ParamValue );
        Command2.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command2.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command2.append( ctrlM );
    };
    if ( ParamName=="Command3") {
        Command3.clear();
        Command3.append( ParamValue );
        Command3.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command3.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command3.append( ctrlM );
    };
    if ( ParamName=="Command4") {
        Command4.clear();
        Command4.append( ParamValue );
        Command4.replace( QByteArray("^C"), QByteArray(1, ctrlC) );
        Command4.replace( QByteArray("^M"), QByteArray(1, ctrlM) );
        Command4.append( ctrlM );
    };

}

void QAPRSKISSPORT::openPort() {

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );

    setPortStatus( 0 ); //включен

    ComReadData.clear();

#ifdef Q_OS_WIN
    QString COMPortName;

    COMPortName.clear();
    COMPortName.append( "\\\\.\\" );
    COMPortName.append( ComName );

    qDebug() << ( tr( "(%1)%2::try to open COM port<br>" ).arg( portNum() ).arg( portType() ) );

    m_Handle = CreateFile( reinterpret_cast<const wchar_t *>( COMPortName.utf16() ), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    //m_Handle = CreateFile( (wchar_t *)(COMPortName.utf16()) , GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_Handle == INVALID_HANDLE_VALUE) {

        qDebug() << ( tr( "(%1)%2::Can't open COM port<br>" ).arg( portNum() ).arg( portType() ) );

        this->closePort();
        setPortStatus( 2 ); //ошибка
        return ;
    }

    TIMEOUT = 1000;
    COMMTIMEOUTS CommTimeOuts;

    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;

    if (!SetCommTimeouts(m_Handle, &CommTimeOuts)) {

        qDebug() << ( tr( "(%1)%2::Can't set COMM timeouts<br>" ).arg( portNum() ).arg( portType() ) );

        this->closePort();
        setPortStatus( 0 ); //включен
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

        qDebug() << ( tr( "(%1)%2::Can't set COMM state<br>" ).arg( portNum() ).arg( portType() ) );

        this->closePort();
        setPortStatus( 0 ); //включен
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

        /* ???????? ????? */
        fd = open("/dev/ttyd0", O_RDWR | O_NOCTTY | O_NDELAY);//!!!! "/dev/ttyd0" ????
        fcntl(fd, F_SETFL, FNDELAY);

        /* ????????? ??????? ????? */
        tcgetattr(fd, &options);

        /* ????????? ???????????????????? ?????, 1-????????? ??????? */
        options.c_cflag     |= (CLOCAL | CREAD);
        options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag     &= ~OPOST;
        options.c_cc[VMIN]  = 0;
        options.c_cc[VTIME] = 10;
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);
        /* ????????? ????? */
        tcsetattr(fd, TCSANOW, &options);

#endif

    //запускаем таймер для чтения статуса порта каждые 0.1с
    connect(ComReadtimer, SIGNAL(timeout()), this, SLOT(onComReadTimer()));
    ComReadtimer->start( 100 );

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

    setPortStatus( 1 ); //выключен
    ComReadtimer->stop();

    ComReadtimer->disconnect();

#ifdef Q_OS_WIN
    CloseHandle( m_Handle );
    m_Handle = 0;
#endif

#ifdef Q_OS_UNIX
        close(fd);
#endif

    qDebug() << ( tr( "(%1)%2::closePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSKISSPORT::updatePort() {

    this->closePort();
    this->openPort();

    qDebug() << ( tr( "(%1)%2::updatePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSKISSPORT::onComReadTimer() {

#ifdef Q_OS_WIN
//проверка, есть ли данные в буфере чтения порта
    QByteArray ComData;
    QByteArray OneMsg;

    COMSTAT ComStat;
    DWORD   e, aInQueue;
    if (ClearCommError(m_Handle, &e, &ComStat)) {
        aInQueue = ComStat.cbInQue;
        if (aInQueue>0) {

            static char *buffer = NULL;
            if(!buffer) { buffer = (char *)malloc(1024); };
            DWORD len = 1024;
            DWORD feedback = 0;
            ReadFile(m_Handle, buffer, len, &feedback, NULL);

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

    char buffer[1024];  /* ????? ????? */
    char *bufptr;      /* ??????? ?????? ? ?????? */
    int  nbytes;       /* ????? ??????????? ??????*/
    int feedback = 0;

    //чтение данных из порта
    bufptr = buffer;
    while ((nbytes = read(fd, bufptr, buffer + sizeof(buffer) - bufptr - 1)) > 0) {
        bufptr += nbytes;
        feedback += nbytes;
    };

    //если данные прочитаны, то добавляем их к буферу ComReadData
    if (feedback>0) {
        for(int i=0;i<feedback;i++) {
            ComReadData.append( buffer[i] );
        }
    }

    //если данные в буфере начинаются на 0xc0 и заканчиваются 0xc0 то чтение завершено
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

                    if ( ((ComData.data()[0])==char(0xC0)) and ((ComData.data()[1])==char(0x00)) ) {

                        ComData = ComData.mid( 2 ); //удаляем FEND и номер команды (0)
                        OneMsg = ComData.left( ComData.indexOf( 0xC0 ) );

                        emit RXPacket( portNum(), TRUE );

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
                            if ( ( ( uchar( OneMsg.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (??? ?????????? ??????)
                            sh += 7;
                        } while ( ( ( uchar( OneMsg.data()[sh-1] ) ) & 1 ) !=1 );


                        //replace first ',' of the Src to '>'
                        if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

                        QString MsgText( "" );
                        MsgText = OneMsg.mid( sh + 2, OneMsg.count() - sh - 3 ); // +2 - skip Control byte and PID byte
                                                                                     // -5 - skip ^^ and 2 byte of CRC-16 (NO IN KISS)
                        QAPRSAXIPPacket Packet( DestCall, SrcCall, MsgText );
                        emit RXAXPacket( portNum(), Packet );
                        emit RXPacket( portNum(), FALSE );
                        emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );

                    } else {

                        //log->insertHtml("("+QString::number(PortNum)+")"+PortType+"::ignore packet<br>");
                        //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
                        //emit ToLog("("+QString::number(PortNum)+")"+PortType+"::ignore packet<br>");
                        qDebug() << ( tr( "(%1)%2::ignore packet<br>" ).arg( portNum() ).arg( portType() ) );

                        ComData = ComData.mid( 2 ); //delete first FEND and command 0
                    }

                    ComData = ComData.mid( ComData.indexOf( 0xC0 ) + 1 );

                }

            }

}

void QAPRSKISSPORT::onComRead(QString DestCall, QString SrcCall, QString MsgText) {
//    emit RXPacket( PortNum, DestCall, SrcCall, MsgText );
}

void QAPRSKISSPORT::sendAPRSPacket( QString To, QString From, QString MsgText ){

    QAPRSAXIPPacket Packet( To, From, MsgText );

    if ( portStatus() == 0 ) {
        emit TXAPRSPacket( portNum(), To, From, MsgText );
        this->sendAXPacket( Packet );
    }

}

void QAPRSKISSPORT::sendAXPacket( QByteArray AXPacket ) {

    /*                  АХТУНГ!!!

      По стандарту KISS используется 4 спецсимвола. Они должны быть заменены,
      если данные совпадают с их кодами. При работе с APRS обычно данные
      текстовые, поэтому данные со спецкодами не совпадают - проблем нет.
      Если же порт будет использоваться впоследствии в пакетном режиме с
      пересылкой двоичных данных, то этот момент нужно будет исправить!!!

               Abbreviation            Description                    Hex value
                  FEND                 Frame  End                         C0
                  FESC                 Frame  Escape                      DB
                  TFEND                Transposed Frame End               DC
                  TFESC                Transposed Frame Escape            DD

     */

    QByteArray Packet;
    if ( portStatus() == 0 ) {
        Packet.clear();
        Packet.append( 0xC0 );                       //FEND
        Packet.append( char(0) );                    //KISS Command=0 - Data Frame
        Packet.append( AXPacket.left( AXPacket.length() - 2 ) );                   //AX25 PACKET
        Packet.append( 0xC0 );                       //FEND

        emit TXPacket( portNum(), TRUE );
        #ifdef Q_OS_WIN
            DWORD feedback = 0;
            WriteFile( m_Handle, Packet.data() , int(Packet.size()), &feedback, 0 );
        #endif

        #ifdef Q_OS_UNIX
            int feedback = 0;
            feedback = write(fd, Packet.data(), Packet.length());
        #endif
        emit TXPacket( portNum(), FALSE );
    }

}

// реализация FLDigi порта
QAPRSFLDIGIPORT::QAPRSFLDIGIPORT( QObject *parent ) {

    this->setPortType( "FLDIGI" );
    this->setPortName( "n\\a" );
    this->setPortStatus( 1 );
    this->setRXPacket( FALSE );
    this->setTXPacket( FALSE );
    this->setBUSY( FALSE );

    FLDIGIHost = "127.0.0.1";
    FLDIGIPort = "7322";

    tcpClient   = new QTcpSocket(this);
    reconnecttimer = new QTimer(this);

    qDebug() << "Port::Create(FLDigi)";

}

QAPRSFLDIGIPORT::~QAPRSFLDIGIPORT( ) {

    tcpClient->close();
    tcpClient->disconnect();
    delete tcpClient;
    tcpClient = NULL;

    reconnecttimer->stop();
    reconnecttimer->disconnect();
    delete reconnecttimer;
    reconnecttimer = NULL;

}

void QAPRSFLDIGIPORT::openPort() {

    datain.clear();

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );
    qDebug() << ( tr( "(%1)%2::Connecting to FLDigi<br>" ).arg( portNum() ).arg( portType() ) );

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected ()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpClientconnected ()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpClienterror(QAbstractSocket::SocketError)));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));

    tcpClient->connectToHost( FLDIGIHost, FLDIGIPort.toInt() );
    tcpClient->write( "<cmd>server</cmd>" );

    setPortStatus( 0 ); //включен

}

void QAPRSFLDIGIPORT::closePort() {

    setPortStatus( 1 ); //выключен
    tcpClient->abort();
    reconnecttimer->stop();

    tcpClient->disconnect();
    reconnecttimer->disconnect();

    qDebug() << ( tr( "(%1)%2::closePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSFLDIGIPORT::setParam( QString ParamName, QString ParamValue ) {

    if ( ParamName=="Host") FLDIGIHost = ParamValue;
    if ( ParamName=="Port") FLDIGIPort = ParamValue;

}

void QAPRSFLDIGIPORT::updatePort() {

    this->closePort();
    this->openPort();

    qDebug() << ( tr( "(%1)%2::updatePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSFLDIGIPORT::tcpClientconnected () {
};

void QAPRSFLDIGIPORT::tcpClientdisconnected () {

    qDebug() << ( tr( "(%1)%2::Disconnected<br>" ).arg( portNum() ).arg( portType() ) );

    this->closePort();

};

void QAPRSFLDIGIPORT::tcpClienterror (QAbstractSocket::SocketError socketError) {

    qDebug() << ( tr( "(%1)%2::Clienterror<br>" ).arg( portNum() ).arg( portType() ) );

    this->closePort();
    setPortStatus( 2 ); //ошибка

    //start reconnect timer
    connect(reconnecttimer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));
    reconnecttimer->start( 30000 );

};

void QAPRSFLDIGIPORT::tcpClientread() {

    QByteArray msg;
    QString mesg;
    QString MsgText("");
    QString SrcCall("");
    QString DestCall("");

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet</font><br>");

    while (tcpClient->bytesAvailable()==0);
    msg = tcpClient->readAll();

    datain.append( msg );

    if ( uchar(datain.data()[ datain.length() - 1 ]) == uchar(0x0d)) {
        //qDebug() << "Received ='" << datain << "'";
        msg.clear();
        msg.append( datain );

        while (msg.length()>0) {

            mesg = msg.left( msg.indexOf( 13 ) + 1 );

            if (mesg.length()>1) {

                DestCall = mesg.mid( mesg.indexOf(">") + 1, mesg.indexOf(":") - mesg.indexOf(">") - 1 );
                SrcCall = mesg.left( mesg.indexOf(">") );
                if (DestCall.indexOf( ',' )!=-1) {
                    SrcCall.append( ">" + DestCall.mid( DestCall.indexOf( ',' ) + 1 ) );
                    DestCall = DestCall.left( DestCall.indexOf( ',' ) );
                }
                if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';
                MsgText = mesg.mid( mesg.indexOf(":") + 1, mesg.length() - mesg.indexOf(":") - 2 );

                if ((DestCall.length()>0)and(DestCall.length()<9)and(SrcCall.length()>0)and(MsgText.length()>0)
                    and(!((DestCall==SrcCall)))) {
                    //qDebug() << "DestCall = " << DestCall;
                    //qDebug() << "SrcCall = " << SrcCall;
                    //qDebug() << "MsgText = " << MsgText;

                    //emit RXPacket( PortNum, DestCall, SrcCall, MsgText );
                    emit RXPacket( portNum(), TRUE );

                    QAPRSAXIPPacket Packet( DestCall, SrcCall, MsgText );
                    emit RXAXPacket( portNum(), Packet );
                    emit RXPacket( portNum(), FALSE );
                    emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );


                }
            }

            msg = msg.mid( msg.indexOf(13) + 1 );
        }

        datain.clear();
    }

    msg.clear();
    mesg.clear();
    DestCall.clear();
    SrcCall.clear();
    MsgText.clear();

};

void QAPRSFLDIGIPORT::onReconnectTimer() {

    qDebug() << ( tr( "(%1)%2::Try reconnect to AGW core<br>" ).arg( portNum() ).arg( portType() ) );

    reconnecttimer->stop();
    reconnecttimer->disconnect();

    this->openPort();

}


void QAPRSFLDIGIPORT::sendAPRSPacket( QString To, QString From, QString MsgText ){

    QAPRSAXIPPacket Packet( To, From, MsgText );

    QByteArray APRSPacket;
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );

    if ( portStatus() == 0 ) {
        APRSPacket.clear();
        APRSPacket.append( ">>>>>" );
        APRSPacket.append( QByteArray::fromHex("0d0a") );

        //Packet.append( 1 );
        APRSPacket.append( From.left( From.indexOf('>') ) + ">"+APRSCall()+",TCPIP*:" + MsgText);

        APRSPacket.append( QByteArray::fromHex("0d0a") );
        APRSPacket.append( QByteArray::fromHex("0d0a") );

        //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );
        //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::'" + Packet + "'<br>" );

        tcpClient->write(APRSPacket);

        emit TXAPRSPacket( portNum(), To, From, MsgText );

        this->sendAXPacket( Packet );
    }

}

void QAPRSFLDIGIPORT::sendAXPacket( QByteArray AXPacket ) {

    if ( portStatus() == 0 ) {
        emit TXPacket( portNum(), TRUE );
        emit TXAXPacket( portNum(), AXPacket );
        emit TXPacket( portNum(), FALSE );
    }

}

//реализация VASKA порта
QAPRSVASKAPORT::QAPRSVASKAPORT( QObject *parent ) {

    this->setPortType( "VASKA" );
    this->setPortName( "n\\a" );
    this->setPortStatus( 1 );
    this->setRXPacket( FALSE );
    this->setTXPacket( FALSE );
    this->setBUSY( FALSE );

    VASKAHost = "127.0.0.1";
    VASKAPort = "5001";

    tcpClient   = new QTcpSocket(this);
    reconnecttimer = new QTimer(this);

    qDebug() << "Port::Create(VASKA)";

}

QAPRSVASKAPORT::~QAPRSVASKAPORT( ) {

    tcpClient->close();
    tcpClient->disconnect();
    delete tcpClient;
    tcpClient = NULL;

    reconnecttimer->stop();
    reconnecttimer->disconnect();
    delete reconnecttimer;
    reconnecttimer = NULL;

}

void QAPRSVASKAPORT::openPort() {

    datain.clear();

    qDebug() << ( tr( "(%1)%2::openPort<br>" ).arg( portNum() ).arg( portType() ) );
    qDebug() << ( tr( "(%1)%2::Connecting to VASKA<br>" ).arg( portNum() ).arg( portType() ) );

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected ()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpClientconnected ()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpClienterror(QAbstractSocket::SocketError)));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));

    tcpClient->connectToHost( VASKAHost, VASKAPort.toInt() );

    setPortStatus( 0 ); //включен

}

void QAPRSVASKAPORT::closePort() {

    setPortStatus( 1 ); //выключен
    tcpClient->abort();
    reconnecttimer->stop();

    tcpClient->disconnect();
    reconnecttimer->disconnect();

    qDebug() << ( tr( "(%1)%2::closePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSVASKAPORT::setParam( QString ParamName, QString ParamValue ) {

    if ( ParamName=="Host") VASKAHost = ParamValue;
    if ( ParamName=="Port") VASKAPort = ParamValue;
    if ( ParamName=="Mode") VASKAMode = ParamValue;

}

void QAPRSVASKAPORT::updatePort() {

    this->closePort();
    this->openPort();

    qDebug() << ( tr( "(%1)%2::updatePort<br>" ).arg( portNum() ).arg( portType() ) );

}

void QAPRSVASKAPORT::tcpClientconnected () {
};

void QAPRSVASKAPORT::tcpClientdisconnected () {

    qDebug() << ( tr( "(%1)%2::Disconnected<br>" ).arg( portNum() ).arg( portType() ) );

    this->closePort();

};

void QAPRSVASKAPORT::tcpClienterror (QAbstractSocket::SocketError socketError) {

    qDebug() << ( tr( "(%1)%2::Clienterror<br>" ).arg( portNum() ).arg( portType() ) );

    this->closePort();
    setPortStatus( 2 ); //ошибка

    //start reconnect timer
    connect(reconnecttimer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));
    reconnecttimer->start( 30000 );

};

void QAPRSVASKAPORT::tcpClientread() {

    QByteArray msg;
    QString mesg;
    QString MsgText("");
    QString SrcCall("");
    QString DestCall("");

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet</font><br>");

    while (tcpClient->bytesAvailable()==0);
    msg = tcpClient->readAll();

    //datain.append( msg );

    qDebug() << "read VASKA " << msg;
    qDebug() << msg.toHex();

    if ( VASKAMode == "1" ) {
        //text mode

        if (msg.length()>1) {

            DestCall = msg.mid( msg.indexOf(">") + 1, msg.indexOf(":") - msg.indexOf(">") - 1 );
            SrcCall = msg.left( msg.indexOf(">") );
            if (DestCall.indexOf( ',' )!=-1) {
                SrcCall.append( ">" + DestCall.mid( DestCall.indexOf( ',' ) + 1 ) );
                DestCall = DestCall.left( DestCall.indexOf( ',' ) );
            }
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';
            MsgText = msg.mid( msg.indexOf(":") + 1, msg.length() - msg.indexOf(":") - 1 );

            if ((DestCall.length()>0)and(DestCall.length()<9)and(SrcCall.length()>0)and(MsgText.length()>0)
                and(!((DestCall==SrcCall)))) {
                //qDebug() << "DestCall = " << DestCall;
                //qDebug() << "SrcCall = " << SrcCall;
                //qDebug() << "MsgText = " << MsgText;

                //emit RXPacket( PortNum, DestCall, SrcCall, MsgText );
                emit RXPacket( portNum(), TRUE );

                QAPRSAXIPPacket Packet( DestCall, SrcCall, MsgText );
                emit RXAXPacket( portNum(), Packet );
                emit RXPacket( portNum(), FALSE );
                emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );

            }

        }

    }

    if ( VASKAMode == "0" ) {
        //bin mode

        if (msg.length()>1) {

            emit RXPacket( portNum(), TRUE );
            emit RXAXPacket( portNum(), msg );
            emit RXPacket( portNum(), FALSE );

            int i;
            DestCall.clear();
            unsigned char DestSID;
            for( i=1; i<=6; i++ ) {
                if ( ( uchar( msg.data()[i-1] ) >> 1 )!=' ' )
                    DestCall += uchar( msg.data()[i-1] ) >> 1;
            };

            DestSID = ( uchar( msg.data()[6] ) >> 1 ) & 15;
            if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

            int sh;
            sh = 7;
            SrcCall.clear();
            unsigned char SrcSID;
            do {
                for( i=1; i<=6; i++ ) {
                    if ( ( uchar( msg.data()[sh+i-1] ) >> 1 )!=' ' )
                        SrcCall += uchar( msg.data()[sh+i-1] ) >> 1;
                };
                SrcSID = ( uchar( msg.data()[sh+6] ) >> 1 ) & 15;             //SID
                if ( SrcSID>0 ) SrcCall += "-" + QString::number( SrcSID );
                if ( ( ( uchar( msg.data()[sh+6] ) >> 7 ) & 1) == 1) SrcCall += "*";  //H - retranslator bit
                if ( ( ( uchar( msg.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
                sh += 7;
            } while ( ( ( uchar( msg.data()[sh-1] ) ) & 1 ) !=1 );

            //заменяем первую запятую в списке позывных на '>'
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

            MsgText.clear();
            MsgText = msg.mid( sh + 2, msg.count() - sh - 5 ); // +2 - skip Control byte and PID byte

            emit RXAPRSPacket( portNum(), DestCall, SrcCall, MsgText );

            qDebug() << "DestCall = " << DestCall;
            qDebug() << "SrcCall = " << SrcCall;
            qDebug() << "MsgText = " << MsgText;


        }

    }

};

void QAPRSVASKAPORT::onReconnectTimer() {

    qDebug() << ( tr( "(%1)%2::Try reconnect to AGW core<br>" ).arg( portNum() ).arg( portType() ) );

    reconnecttimer->stop();
    reconnecttimer->disconnect();

    this->openPort();

}


void QAPRSVASKAPORT::sendAPRSPacket( QString To, QString From, QString MsgText ){

    QAPRSAXIPPacket Packet( To, From, MsgText );

    QByteArray APRSPacket;
    //log->insertHtml( "(" + QString::number(PortNum) + ")" + PortType + "::<font color='#880000'>TX Packet</font><br>" );

    if ( portStatus() == 0 ) {

        if ( VASKAMode == "1" ) {
            //text mode
            APRSPacket.clear();

            APRSPacket.append( From.left( From.indexOf('>') ) + ">"+APRSCall()+",TCPIP*:" + MsgText);

            tcpClient->write(APRSPacket);

            emit TXAPRSPacket( portNum(), To, From, MsgText );

            this->sendAXPacket( Packet );

        }

        if ( VASKAMode == "0" ) {
            //bin
            emit TXAPRSPacket( portNum(), To, From, MsgText );

            this->sendAXPacket( Packet );

        }

    }

}

void QAPRSVASKAPORT::sendAXPacket( QByteArray AXPacket ) {

    if ( portStatus() == 0 ) {

        if ( VASKAMode == "1" ) {
            //text mode
            emit TXPacket( portNum(), TRUE );
            emit TXAXPacket( portNum(), AXPacket );
            emit TXPacket( portNum(), FALSE );
        }

        if ( VASKAMode == "0" ) {
            // bin
            emit TXPacket( portNum(), TRUE );
            emit TXAXPacket( portNum(), AXPacket );

            tcpClient->write( AXPacket );

            emit TXPacket( portNum(), FALSE );
        }

    }

}
