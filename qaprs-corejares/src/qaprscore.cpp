#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QScrollBar>
#include <QDate>
#include <QTime>
#include "include/qaprscore.h"


QAPRSCore::QAPRSCore( QObject *parent )        
{

    fileLog.setFileName( "./general.log" );
    fileLog.open( QIODevice::Append );
    outLog.setDevice( &fileLog );

    filePacketLog.setFileName( "./packet.log" );
    filePacketLog.open( QIODevice::Append );
    outPacketLog.setDevice( &filePacketLog );

    for(int i=0;i<256;i++)
        Port[i]=NULL;

    agwPN = 1;
    AGWEmulatorActive = FALSE;

    tcpServer = new QTcpServer(this);

    tcpServerConnection = new QTcpSocket(this);

    coreActive = FALSE;

}

//эмулятор AGW CORE
void QAPRSCore::tcpServerNewConnection () {

    ToLog( tr("QAPRSCore::Incoming connection to the AGW Core simulator<br>") );

    tcpServerConnection = tcpServer->nextPendingConnection();

    connect(tcpServerConnection, SIGNAL( readyRead() ),
            this, SLOT( tcpServerRead() ) );

    connect(tcpServerConnection, SIGNAL( disconnected () ),
            this, SLOT( tcpServerDisconnect () ) );

    tcpServer->close(); //!!!! support only one connection

}

void QAPRSCore::AGWEmulatorStart( ){

    //запуск AGW эмулятора
    if ( AGWEmulatorActive == FALSE ) {
        //ToLog( "QAPRSCore::Ready for connection to the AGW Core simulator<br>" );
        tcpServer->listen( QHostAddress::QHostAddress("127.0.0.1"), AGWEmulatorPort);
        connect(tcpServer, SIGNAL( newConnection () ), this, SLOT(tcpServerNewConnection () ) );
        AGWEmulatorActive = TRUE;

    }

}


void QAPRSCore::AGWEmulatorStop( ){

    //остановка AGW эмулятора
    if ( AGWEmulatorActive == TRUE ) {

        tcpServer->close();
        tcpServer->disconnect();
        AGWEmulatorActive = FALSE;

        if (tcpServerConnection!=NULL) {
            tcpServerConnection->close();
            tcpServerConnection->disconnect();
            //tcpServerConnection->deleteLater();
        }

    }

}


void QAPRSCore::tcpServerDisconnect () {

    ToLog( tr("QAPRSCore::AGW user disconnect<br>") );

    tcpServerConnection->close();
    tcpServerConnection->disconnect();
    //tcpServerConnection->deleteLater();
    if ( AGWEmulatorActive == TRUE )
        tcpServer->listen(QHostAddress::QHostAddress("127.0.0.1"), AGWEmulatorPort);

}

void QAPRSCore::tcpServerRead () {
	//чтение данных от клиента, подключенного к эмулятору

    QByteArray datagram;
    QByteArray mesg;
    QByteArray reply;
    int msglen;

    while (tcpServerConnection->bytesAvailable()==0);
    datagram = tcpServerConnection->readAll();

    //log->insertHtml( "QAPRSCore::AGW client request '"+datagram.toHex()+"'<br>" );

    while ( datagram.length()>0 ) {

        //log->insertPlainText( "QAPRSCore::AGW send raw length='" + QString::number( datagram.length() ) + "'\n" );
        //log->insertPlainText( "QAPRSCore::AGW send raw = '" + datagram.toHex() + "'\n" );


        if (uchar(datagram[4])=='R') {
            //посылаем "версию" ядра AGW
            ToLog( "QAPRSCore::AGW send ver num<br>" );

            reply = QByteArray::fromHex("000000005200000000000000000000000000000000000000000000000800000000000000d50700007f000000");

            tcpServerConnection->write( reply );
            datagram = datagram.mid( 36 );
        }

        else

        if (uchar(datagram[4])=='G') {
            //посылаем информацию о портах
            ToLog( tr("QAPRSCore::AGW send port info<br>") );

            reply = QByteArray::fromHex("000000004700000000204f6e20434f4d333a00415869503b006261757500000000000000");

            QString PortList;
            int     PortCount=0;
            PortList.clear();
            for(int i=0;i<256;i++)
              if (Port[i]!=NULL) {
                PortCount++;
                PortList.append( "Port => " + QString::number( Port[i]->PortNum ) + " " + Port[i]->PortName + " (" + Port[i]->PortType + ");" );
              }

            PortList.prepend( QString::number( PortCount ) + ";" );
            reply.append( PortList );
            reply[28] = ( reply.length() - 36 );

            tcpServerConnection->write( reply );
            datagram = datagram.mid( 36 );

        }

        else

        if (uchar(datagram[4])=='k') {
            ToLog( tr("QAPRSCore::AGW Activate reception of Frames in raw format<br>") );

            monitoringPackets = true;
            datagram = datagram.mid( 36 );
        }

        else

        if (uchar(datagram[4])=='K') {

            //log->insertHtml( "QAPRSCore::AGW Send Data in 'raw' AX.25 format<br>" );

            //log->insertPlainText( "QAPRSCore::AGW send raw length='" + QString::number( mesg.length() ) + "'\n" );
            //log->insertPlainText( "QAPRSCore::AGW send raw = '" + mesg.toHex() + "'\n" );

            //log->insertPlainText( "QAPRSCore::AGW packet = '" + mesg.mid(37) + "'\n" );
            //'82a0aa646a9ce0aa82669aa29460ae92888a66406703f03a5541334d41442020203a3131313131317b39330d'
            int qpnum = uchar(datagram.data()[0])+1;
            //log->insertHtml( "QAPRSCore::AGW Packet destination - port " + QString::number( qpnum ) + "<br>" );

            msglen = uchar(datagram.data()[28])+(uchar(datagram.data()[29]) << 8) ;

            mesg = datagram.mid( 37, msglen-1 );


            //log->insertPlainText( "QAPRSCore::msg length='" + QString::number( datagram.length() ) + "'\n" );
            //log->insertPlainText( "QAPRSCore::msg length!!!!!!!!!='" + QString::number( uint(msglen) ) + "'\n" );
            //log->insertPlainText( "QAPRSCore::msg raw = '" + datagram.toHex() + "'\n" );


            //decode packet
            int i;
            QString DestCall( "" );
            unsigned char DestSID;
            for( i=1; i<=6; i++ ) {
                if ( ( uchar( mesg.data()[i-1] ) >> 1 )!=' ' )
                    DestCall += uchar( mesg.data()[i-1] ) >> 1;
            };

            DestSID = ( uchar( mesg.data()[6] ) >> 1 ) & 15;
            if ( DestSID>0 ) DestCall += "-" + QString::number( DestSID );

            //log->insertHtml( "QAPRSCore::DestCall='" );
            //log->insertPlainText( DestCall + "'\n");

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
                if ( ( ( uchar( mesg.data()[sh+6] ) ) & 1) == 0) SrcCall += ",";    //HDLC bit (бит расширения адреса)
                sh += 7;
            } while ( ( ( uchar( mesg.data()[sh-1] ) ) & 1 ) !=1 );

            //replace first ',' of the Src to '>'
			//меняем первую запяту ',' на значек '>' в списке позывных, если это не так
            if (SrcCall.indexOf( ',' )!=-1) SrcCall[ SrcCall.indexOf( ',' ) ]='>';

            //log->insertHtml( "QAPRSCore::SrcCall='" );
            //log->insertPlainText( SrcCall + "'\n" );

            QString MsgText( "" );
            //log->insertHtml( "QAPRSCore::MsgText='" );
            MsgText = mesg.mid( sh + 2 , mesg.count() - sh - 3 ); // +2 - skip Control byte and PID byte
                                                                             // -5 - skip ^^ and 2 byte of CRC-16
            //log->insertPlainText( MsgText + "'\n");

            int pnum=0;
            for(int i=0;i<256;i++) {
                if (Port[i]!=NULL) {
                    pnum++;
                    if (pnum==qpnum) {
                        Port[i]->sendPacket( DestCall, SrcCall, MsgText );

                        //log->insertHtml( "(" + QString::number( i ) + ":" + QString::number( Port[i]->agwPortNum ) + ")" + Port[i]->PortType + "::<font color='#008800'>!!!!!!!!!!</font> "
                        //     "To: " + DestSID + " "
                        //     "From: " + SrcCall + " "
                        //     "MsgText: '" + MsgText + "'<br>" );
                        // log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

                    }
                }
            }

            datagram = datagram.mid( 36 + msglen );

        }

        else

        {
            ToLog( tr("QAPRSCore::unknown length='") + QString::number( datagram.length() ) + "'\n" );
            ToLog( tr("QAPRSCore::unknown raw = '") + datagram.toHex() + "'\n" );

            datagram = datagram.mid( 36 );
        }

    }

    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

    datagram.clear();
    mesg.clear();
    reply.clear();


}

/////////////////////
QAPRSPort *QAPRSCore::createPort( QString PortType ) {

    QAPRSPort *newPort;
    newPort = NULL;
    if ( PortType=="AXIP" )     newPort = new QAPRSAXIPPORT(this);
    if ( PortType=="APRS Internet Server Connection" ) newPort = new QAPRSINTERNETPORT(this);
    if ( PortType=="AGW CORE" ) newPort = new QAPRSAGWPORT(this);
    if ( PortType=="KISS TNC" ) newPort = new QAPRSKISSPORT(this);
    if ( PortType=="FL Digi" ) newPort = new QAPRSFLDIGIPORT(this);

    newPort->APRSCall = this->APRSCall;

	return newPort;

}

void QAPRSCore::createPorts() {

    QSqlQuery query("select p2.port_type_note, p1.* "
                    "from ports p1 left join port_types p2 "
                    "on p1.port_type_id=p2.port_type_id "
                    "where p1.port_type_id in (0,1,2,5,6) "
                    "order by p1.port_num");
    agwPN = 1;
    while (query.next()) {

         int     port_num             = query.value( query.record().indexOf("port_num") ).toInt();
         int     port_type_id         = query.value( query.record().indexOf("port_type_id") ).toInt();

         //создаем порт. если был, то сначала прибиваем 
         if (Port[port_num]!=NULL) {
             Port[port_num]->closePort();
             Port[port_num]->disconnect();
             //Port[port_num]->deleteLater();
             delete Port[port_num];
             Port[port_num]=NULL;
         }
         Port[port_num] = this->createPort( query.value( query.record().indexOf("port_type_note") ).toString() );

         //устанавливаем параметры, специфичные для данного типа порта
         QSqlQuery query2;
         query2.exec( "select p2.par_name, p1.par_value "
                      "from port_param_values p1 "
                      "left join port_param_names p2 on (p1.par_code=p2.par_code) "
                      "where p1.port_num="+QString::number(port_num)+" "
                      "and p2.port_type_id="+QString::number(port_type_id)+" " );


         while (query2.next())
            Port[port_num]->setParam( query2.value( query2.record().indexOf("par_name") ).toString(), query2.value( query2.record().indexOf("par_value") ).toString() );

         //устанавливаем стандартные для всех типов портов параметры
         Port[port_num]->PortNum = port_num;
         Port[port_num]->agwPortNum = agwPN++;

         Port[port_num]->PortName = query.value( query.record().indexOf("port_note") ).toString();
         Port[port_num]->setParam( "Call", query.value( query.record().indexOf("port_call") ).toString() );                     //port_call
         Port[port_num]->setParam( "BeaconText", query.value( query.record().indexOf("port_beacon_text") ).toString() );       //port_beacon_text
         Port[port_num]->setParam( "BeaconInterval", query.value( query.record().indexOf("port_beacon_interval") ).toString() );             //port_beacon_interval
         Port[port_num]->setParam( "UnprotoAddress", query.value( query.record().indexOf("port_unproto_address") ).toString() );   //port_unproto_address
         Port[port_num]->setParam( "Latitude", query.value( query.record().indexOf("port_latitude") ).toString() );              //port_latitude
         Port[port_num]->setParam( "Longitude", query.value( query.record().indexOf("port_longitude") ).toString() );            //port_longitude
         Port[port_num]->setParam( "Symbol", query.value( query.record().indexOf("port_symbol") ).toString() );                      //port_symbol


         connect(Port[port_num], SIGNAL( RXPacket(int, QString, QString, QString) ),
            this, SLOT( RXPacket (int, QString, QString, QString) ) );

         connect(Port[port_num], SIGNAL( TXPacket(int, QString, QString, QString) ),
            this, SLOT( TXPacket (int, QString, QString, QString) ) );

         connect(Port[port_num], SIGNAL( ToLog(QString) ),
            this, SLOT( ToLog (QString) ) );

         connect(Port[port_num], SIGNAL( ChangeState(int) ),
            this, SLOT( CorePortChangeState (int) ) );

         Port[port_num]->openPort(); //!! открыть порт



         this->sendAGWPortInfo();

    }

}

void QAPRSCore::closePorts() {

    for(int i=0;i<256;i++) {
        if (Port[i]!=NULL) {
            Port[i]->closePort();
            Port[i]->disconnect();
            //Port[i]->deleteLater();
            delete Port[i];
            Port[i]=NULL;
        }
    }

    this->sendAGWPortInfo();

}

void QAPRSCore::RXPacket (int PortNum, QString To, QString From, QString MsgText) {

    //log->insertHtml( "(" + QString::number( PortNum ) + ":" + QString::number( Port[PortNum]->agwPortNum ) + ")" + Port[PortNum]->PortType + "::<font color='#008800'>RX Packet</font> "
    // "To: " + To + " "
    // "From: " + From + " "
    // "MsgText: '" + MsgText + "'<br>" );
    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );
    QString MTo;
    QString MMsg;
    QString MID;

    QSqlQuery query;

    query.prepare( "select max(K)+1 from packets" );
    query.exec();
    query.first();
    int next_K = query.value( 0 ).toInt();

    QString Via;

    if (From.indexOf('>')!=-1) {
        Via = From.mid( From.indexOf('>') + 1 );
        From = From.left( From.indexOf('>') );
    } else {
        Via = "";
    }


    query.prepare( "insert into packets ( K, DT, port_num, trx, PTo, PFrom, PVia, Message ) "
                "values( :p1,:p2,:p3,:p4,:p5,:p6,:p7,:p8  )" );
    query.bindValue(":p1", next_K);
    query.bindValue(":p2", QDate::currentDate().toString( "yyyy-MM-dd " )+ QTime::currentTime().toString( "hh:mm:ss.zzz" ) );
    query.bindValue(":p3", PortNum);
    query.bindValue(":p4", "RX");
    query.bindValue(":p5", To);
    query.bindValue(":p6", From);
    query.bindValue(":p7", Via);
    query.bindValue(":p8", MsgText);
    query.exec();

    //сохраняем пакет в текстовый файл
    outPacketLog
    << "\"" << next_K << "\";"
    << "\"" << QDate::currentDate().toString( "yyyy-MM-dd " )+ QTime::currentTime().toString( "hh:mm:ss.zzz" ) << "\";"
    << "\"" << PortNum << "\";"
    << "\"" << Port[PortNum]->PortType << "\";"
    << "\"" << Port[PortNum]->PortName << "\";"
    << "\"RX\";"
    << "\"" << To << "\";"
    << "\"" << From << "\";"
    << "\"" << Via << "\";"
    << "\"" << MsgText << "\""
    << endl;


    //если пакет является сообщением, то...
    if (MsgText.left(1)==":") {

        MTo = MsgText.mid(1, 9).trimmed();
        if ( MsgText.indexOf("{")==-1 ) {
            //пакет не имеет ID
            MMsg = MsgText.mid(11);
            MID = "";
        } else {
            MMsg = MsgText.mid(11, MsgText.length() - 11 - ( MsgText.length() - MsgText.indexOf("{") ));
            MID = MsgText.mid( MsgText.indexOf("{")+1 );
        }


        query.prepare( "insert into messages ( packet_K, MTo, Message, Mess_ID ) "
                    "values( :p1,:p2,:p3,:p4  )" );
        query.bindValue(":p1", next_K);
        query.bindValue(":p2", MTo );
        query.bindValue(":p3", MMsg );
        query.bindValue(":p4", MID );
        query.exec();


        if (MTo==Port[PortNum]->Call) {
            //если сообщение для меня

            //если это сообщение - пинг, то отвечаем (надо бы пока заремарить)
			if (MMsg=="?APRST") {
                QString Ping;

                Ping.clear();
                Ping.append( From );
                Ping = Ping.leftJustified(9, ' ');
                Ping.append(":");
                Ping.prepend(":");
                Ping.append( "Path - " + From + ">");
                Ping.append( To );
                if (Via.length()>0) {
                    Ping.append( "," + Via + "*" );
                }

                Port[PortNum]->sendPacket( APRSCall, Port[PortNum]->Call, Ping );

            }
        }

        emit TRXMessage();

    }


    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + Port[PortNum]->PortType + "::Test messsssssssssggggg <br>");
    //MsgText.append("!!!");
    QString FromAVia;
    FromAVia = From;
    if (Via!="") FromAVia.append( ">" + Via );
    QAPRSAGWRXPacket Packet( To, FromAVia, MsgText );

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + Port[PortNum]->PortType + "::<br> "
    // "To: " + To + " <br>"
    // "From: " + From+">"+Via + " <br>"
    // "MsgText: '" + MsgText + "'<br>" );
    //log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );


    Packet.data()[0] = Port[PortNum]->agwPortNum -1 ;


    //log->insertHtml( "!!!!!!!!!!!!!!!!!!!!!!!!!Snd packet = '" + Packet.toHex() + "'<br>" );
    tcpServerConnection->write( Packet );

    query.prepare( "select count(*) as cnt from stations "
                   "where  call = ( :p1 )" );
    query.bindValue(":p1", From);
    query.exec();

    query.next();

    int cnt = query.value(0).toInt();

    if ( cnt==0 ) {

        query.prepare( "insert into stations ( call, LH ) "
                       "values ( :p1, :p2 )" );
        query.bindValue(":p1", From);
        query.bindValue(":p2", QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" ) );
        query.exec();

    } else {

        query.prepare( "update stations set LH=( :p1 ) "
                       "where call=( :p2 ) " );
        query.bindValue(":p1", QDateTime::currentDateTime().toString( "yyyy-MM-dd HH:mm:ss" ) );
        query.bindValue(":p2", From);
        query.exec();

    }


    //декодирование информации о позиции станции
    QString StLat, StLng;
    if ( MsgText.left(1) == "=" or MsgText.left(1) == "!" ) {

        //если используется обычный формат
        if ( ( ( MsgText.mid( 8, 1 ) == "N" ) or ( MsgText.mid( 8, 1 ) == "S" ) ) and
             ( ( MsgText.mid( 18, 1 ) == "W" ) or ( MsgText.mid( 18, 1 ) == "E" ) ) ) {

            StLat = MsgText.mid( 1, 2 ) + "." + MsgText.mid( 3, 2 ) + "." + MsgText.mid( 6, 3 );
            StLng = MsgText.mid( 10, 3 ) + "." + MsgText.mid( 13, 2 ) + "." + MsgText.mid( 16, 3 );

            //qreal lenn = (calc_distance_law_of_cosines( lat2real( this->Lat ), long2real( this->Lng ),
            //                                            lat2real( StLat ), long2real( StLng ) ));

            //qreal gradd = (calc_angle( lat2real( this->Lat ), long2real( this->Lng ),
            //                          lat2real( StLat ), long2real( StLng )
            //                                              ));

            //qDebug() << "Angle=" << gradd;

            //query.prepare( "update stations set sym=( :p1 ), lat=( :p2 ), lng=( :p3 ), grid=( :p4 ), stcomment=( :p5 ), "
            //               "via=( :p6 ), length=( :p7 ), deg=( :p8 ) "
            //               "where call=( :p9 ) " );
            query.prepare( "update stations set sym=( :p1 ), lat=( :p2 ), lng=( :p3 ), grid=( :p4 ), stcomment=( :p5 ), "
                           "via=( :p6 ) "
                           "where call=( :p7 ) " );
            query.bindValue(":p1", MsgText.mid( 9, 1 ) + MsgText.mid( 19, 1 ) );
            query.bindValue(":p2", StLat );
            query.bindValue(":p3", StLng );
            query.bindValue(":p4", real2qthloc( lat2real( StLat ), long2real( StLng ) ) );
            query.bindValue(":p5", MsgText.right( MsgText.length() - 20 ) ); //msg - pos = comment
            query.bindValue(":p6", Via );
            //query.bindValue(":p7", lenn / 1000 );
            //query.bindValue(":p8", gradd );
            query.bindValue(":p7", From );
            query.exec();
        } else {
            //если используется упакованный формат
            QByteArray CompPos;
            CompPos.clear();
            CompPos.append( MsgText.mid( 1, 13 ) );
            //for(int i=0;i<CompPos.length();i++)
            //    CompPos.data()[i] = CompPos.data()[i] - 33;


            qreal x1, x2, x3, x4, y1, y2, y3, y4;
            y1 = uint(CompPos.data()[1]);
            y2 = uint(CompPos.data()[2]);
            y3 = uint(CompPos.data()[3]);
            y4 = uint(CompPos.data()[4]);
            x1 = uint(CompPos.data()[5]);
            x2 = uint(CompPos.data()[6]);
            x3 = uint(CompPos.data()[7]);
            x4 = uint(CompPos.data()[8]);

            //StLat = real2lat ( 1 );
            //StLng = real2long( 1 );
            //qDebug() << ( ( 90 - ((y1-33) * 91 * 91 * 91 + (y2-33) * 91 * 91 + ((y3-33) * 91) + (y4-33))/380926)/ 90 );
            //qDebug() << ( ( -180 + ((x1-33) * 91 * 91 * 91 + (x2-33) * 91 * 91 + ((x3-33) * 91) + (x4-33))/190463)/180 );
            StLat = real2lat ( ( -90 + ((y1-33) * 91 * 91 * 91 + (y2-33) * 91 * 91 + ((y3-33) * 91) + (y4-33))/380926)/ 90 );
            StLng = real2long( ( -180 + ((x1-33) * 91 * 91 * 91 + (x2-33) * 91 * 91 + ((x3-33) * 91) + (x4-33))/190463)/180 );

            query.prepare( "update stations set sym=( :p1 ), lat=( :p2 ), lng=( :p3 ), grid=( :p4 ), stcomment=( :p5 ), "
                           "via=( :p6 ) "
                           "where call=( :p7 ) " );
            query.bindValue(":p1", CompPos.mid( 0, 1 ) + CompPos.mid( 9, 1 ) );
            query.bindValue(":p2", StLat );
            query.bindValue(":p3", StLng );
            query.bindValue(":p4", real2qthloc( lat2real( StLat ), long2real( StLng ) ) );
            query.bindValue(":p5", MsgText.right( MsgText.length() - 14 ) ); //msg - pos = comment
            query.bindValue(":p6", Via );
            query.bindValue(":p7", From );
            query.exec();

            CompPos.clear();

        }

        emit UpdateStationList();

    }


    emit TRXPacket();

    Packet.clear();

    qDebug()<<"core rx";
}

void QAPRSCore::TXPacket (int PortNum, QString To, QString From, QString MsgText) {

    QSqlQuery query;

    query.prepare( "select max(K)+1 from packets" );
    query.exec();
    query.first();
    int next_K = query.value( 0 ).toInt();
    //int next_K;

    QString Via;

    if (From.indexOf('>')!=-1) {
        Via = From.mid( From.indexOf('>') + 1 );
        From = From.left( From.indexOf('>') );
    } else {
        Via = "";
    }

    query.prepare( "insert into packets ( K, DT, port_num, trx, PTo, PFrom, PVia, Message ) "
                   "values( :p1,:p2,:p3,:p4,:p5,:p6,:p7,:p8  );" );
    query.bindValue(":p1", next_K );
    query.bindValue(":p2", QDate::currentDate().toString( "yyyy-MM-dd " )+ QTime::currentTime().toString( "hh:mm:ss.zzz" ) );
    query.bindValue(":p3", PortNum);
    query.bindValue(":p4", "TX");
    query.bindValue(":p5", To);
    query.bindValue(":p6", From);
    query.bindValue(":p7", Via);
    query.bindValue(":p8", MsgText);
    query.exec( );


    //сохраняем пакет в лог файл
    outPacketLog
    << "\"" << next_K << "\";"
    << "\"" << QDate::currentDate().toString( "yyyy-MM-dd " )+ QTime::currentTime().toString( "hh:mm:ss.zzz" ) << "\";"
    << "\"" << PortNum << "\";"
    << "\"" << Port[PortNum]->PortType << "\";"
    << "\"" << Port[PortNum]->PortName << "\";"
    << "\"TX\";"
    << "\"" << To << "\";"
    << "\"" << From << "\";"
    << "\"" << Via << "\";"
    << "\"" << MsgText << "\""
    << endl;


    //есл сообщение
    if (MsgText.left(1)==":") {
        QString MTo;
        QString MMsg;
        QString MID;

        MTo = MsgText.mid(1, 9).trimmed();
        if ( MsgText.indexOf("{")==-1 ) {
            //без ID
            MMsg = MsgText.mid(11);
            MID = "";
        } else {
            MMsg = MsgText.mid(11, MsgText.length() - 11 - ( MsgText.length() - MsgText.indexOf("{") ));
            MID = MsgText.mid( MsgText.indexOf("{")+1 );
        }

        query.exec( "insert into messages ( packet_K, MTo, Message, Mess_ID ) "
                    "values( :p1,:p2,:p3,:p4  )" );
        query.bindValue(":p1", next_K);
        query.bindValue(":p2", MTo );
        query.bindValue(":p3", MMsg );
        query.bindValue(":p4", MID );
        query.exec();

        emit TRXMessage();

    }

    emit TRXPacket();
    qDebug()<<"core tx";
}

void QAPRSCore::ToLog (QString Text) {

    log->insertHtml( (QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss") + " " + Text );
    outLog << "\"" << ( (QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss") ) << "\";\"" << Text << "\""<< endl;
    log->verticalScrollBar()->setSliderPosition( log->verticalScrollBar()->maximum() );

}

void QAPRSCore::updatePort( int pnum ) {

    //qDebug()<<"Core Port update "<<pnum;

    QSqlQuery query;

    bool portCreated=FALSE;

    query.prepare( "select p2.port_type_note, p1.* "
                   "from ports p1 left join port_types p2 "
                   "on p1.port_type_id=p2.port_type_id "
                   "where p1.port_type_id in (0,1,2,5,6) "
                   "and p1.port_num=( :p1 )");

    query.bindValue(":p1", pnum );
    query.exec();
    query.next();

    int port_num             = query.value( query.record().indexOf("port_num") ).toInt();
    int port_type_id         = query.value( query.record().indexOf("port_type_id") ).toInt();


    if (Port[port_num]==NULL) {
        //qDebug()<<"Create port "<<pnum;
        Port[port_num] = this->createPort( query.value( query.record().indexOf("port_type_note") ).toString() );
        portCreated = TRUE;
    } else
    if (Port[port_num]!=NULL) {
        if ( Port[port_num]->PortType != query.value( query.record().indexOf("port_type_note") ).toString() ) {
            Port[port_num]->closePort();
            Port[port_num]->disconnect();
            //Port[port_num]->deleteLater();
            delete Port[port_num];
            Port[port_num] = NULL;
            Port[port_num] = this->createPort( query.value( query.record().indexOf("port_type_note") ).toString() );
            portCreated = TRUE;
        }
    }

    if ( Port[port_num] != NULL ) {

        //задаем специфические для данного типа параметры порта
        QSqlQuery query2;
        query2.exec( "select p2.par_name, p1.par_value "
                     "from port_param_values p1 "
                     "left join port_param_names p2 on (p1.par_code=p2.par_code) "
                     "where p1.port_num="+QString::number(port_num)+" "
                     "and p2.port_type_id="+QString::number(port_type_id)+" " );


        while (query2.next())
            Port[port_num]->setParam( query2.value( query2.record().indexOf("par_name") ).toString(), query2.value( query2.record().indexOf("par_value") ).toString() );

        //задаем стандартные параметры

        Port[port_num]->PortName = query.value( query.record().indexOf("port_note") ).toString();
        Port[port_num]->setParam( "Call", query.value( query.record().indexOf("port_call") ).toString() );                     //port_call
        Port[port_num]->setParam( "BeaconText", query.value( query.record().indexOf("port_beacon_text") ).toString() );       //port_beacon_text
        Port[port_num]->setParam( "BeaconInterval", query.value( query.record().indexOf("port_beacon_interval") ).toString() );             //port_beacon_interval
        Port[port_num]->setParam( "UnprotoAddress", query.value( query.record().indexOf("port_unproto_address") ).toString() );   //port_unproto_address
        Port[port_num]->setParam( "Latitude", query.value( query.record().indexOf("port_latitude") ).toString() );              //port_latitude
        Port[port_num]->setParam( "Longitude", query.value( query.record().indexOf("port_longitude") ).toString() );            //port_longitude
        Port[port_num]->setParam( "Symbol", query.value( query.record().indexOf("port_symbol") ).toString() );                      //port_symbol

        if (portCreated == TRUE) {
            Port[port_num]->PortNum = port_num;
            Port[port_num]->agwPortNum = agwPN++;


            connect(Port[port_num], SIGNAL( RXPacket(int, QString, QString, QString) ),
                    this, SLOT( RXPacket (int, QString, QString, QString) ) );

            connect(Port[port_num], SIGNAL( TXPacket(int, QString, QString, QString) ),
                    this, SLOT( TXPacket (int, QString, QString, QString) ) );

            connect(Port[port_num], SIGNAL( ToLog(QString) ),
                    this, SLOT( ToLog (QString) ) );

            connect(Port[port_num], SIGNAL( ChangeState(int) ),
                    this, SLOT( CorePortChangeState (int) ) );

            Port[port_num]->openPort();


        }
        else if (portCreated == FALSE) {
            Port[port_num]->updatePort();
        }

        this->sendAGWPortInfo();


    } 
}

void QAPRSCore::deletePort( int pnum ) {

    //qDebug()<<"Core Port delete "<<pnum;

    if ( Port[pnum]!=NULL ) {

        Port[pnum]->closePort();
        Port[pnum]->disconnect();
        //Port[pnum]->deleteLater();
        delete Port[pnum];
        Port[pnum] = NULL;

        this->sendAGWPortInfo();

    }

}

void QAPRSCore::CorePortChangeState(int pnum) {

    //qDebug() << "Port " << pnum << " state change";
    emit PortChangeState( pnum );

}

void QAPRSCore::sendAGWPortInfo( ) {
//послать клиенту эмулятора AGW сигнал об обновлении списка портов

    if ( AGWEmulatorActive == TRUE ) {

        QByteArray reply;
        reply = QByteArray::fromHex("000000004700000000204f6e20434f4d333a00415869503b006261757500000000000000");

        QString PortList;
        int     PortCount=0;
        PortList.clear();

        for(int i=0;i<256;i++)
            if (Port[i]!=NULL) {
                PortCount++;
                PortList.append( "Port => " + QString::number( Port[i]->PortNum ) + " " + Port[i]->PortName + " (" + Port[i]->PortType + ");" );
            }

        PortList.prepend( QString::number( PortCount ) + ";" );
        reply.append( PortList );
        reply[28] = ( reply.length() - 36 );

        tcpServerConnection->write( reply );

    }

}
