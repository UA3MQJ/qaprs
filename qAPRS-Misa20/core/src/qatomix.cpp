#include "../../core/include/qatomix.h"
#include "../../core/include/aprs.h"

QAtomix::QAtomix(QObject *parent) :
    QThread(parent)
{

    started = false;

    loadingMode = false;

    uid        = qrand() % 65535 + 1;

    iStorageType = -1;

    sysDBNeedDeleteSysPorts = false;
    sysDBNeedDeleteSysBeacons = false;

    SysVars     = NULL;
    SysPorts    = NULL;
    SysBeacons  = NULL;

    SysVars     = new QSysVars::QSysVars();
    SysSymbols  = new QAPRSSymbols::QAPRSSymbols();
    SysPackets  = new QAPRSPacketList::QAPRSPacketList();
    SysStations = new QAPRSstationList::QAPRSstationList();

    connect( this,         SIGNAL( SIGstationSetParam( QString, QString, QString ) ),
             SysStations,  SLOT( stationSetParam( QString, QString, QString ) ) );

    say( "I was born!" );

    sysDecoder   = QTextCodec::codecForName("UTF-8");
    sysDecoder16 = QTextCodec::codecForName("UTF-16");
    inData.clear();


}

void QAtomix::setAtomName( QString sAtomName ) {

    vatomName = sAtomName;
    say( "My name is " + vatomName );

}

void QAtomix::startSaveToStorage() {

    if ( iStorageType == 0 ) {

        if ( started==false ) {
            say("startSaveToStorage");
            this->start(QThread::HighPriority);
        } else {
            say("startSaveToStorage - already started");
        }

    } else {
        //say("startSaveToStorage - ignore (i'm not storage)");
    }

}

void QAtomix::setStorageType( int stp ) {

    //����� �������, ��� ������ �� ���� ����� �� �������
    //needSysPorts = false;

    //������, ���� ((. ������ ����� �������, ��� �������� ����� ���� ��� ����� ����������� ������ ��
    //��������� �� � ��� �� ��������� �� ����������. � ��� ����������� ������ �� �����
    //������������ ���� �� �� ������ set (��� ���� ����) � ���������� LOAD ��� ��������������
    //�������� ������
    //1.������� ������� ���� ����� ��� �� ��, ������� � ��������� �������
    //2.�������� ���� �����������. �������� �������� � �������� LOAD. �������� ������� � ���������
    //3.����� ��� set ��� weset


    if ( stp == 0 ) { //������� ������ � sql

        say( "I'm storage!" );

        SysPorts = new QSysPorts::QSysPorts();

        SysBeacons = new QSysBeacons::QSysBeacons();


        QSqlQuery query;
        QSqlQuery query2;

        qDebug() << "QSysDBase::QSysDBase.vars - Try load from storage";

        query.prepare( "select count(*) as cnt from vars" );
        query.exec();
        query.first();

        if ( query.isActive() ) {
            //���� �� ������
            qDebug() << "QSysDBase::QSysDBase.vars - Data is available";

            query.prepare( "select varname, varval from vars" );

            query.exec();
            while ( query.next() ) {
                this->DOweSysvarSet( query.value(0).toString(), query.value(1).toString() );
            }

            QSysVar  *tmp;

            //������ ��� ���������, ������� �� �� ���� ���������
            tmp = this->SysVars->first;

            while ( tmp != NULL ) {
                tmp->stored = true;

                tmp = tmp->next;
            }

        } else {
            //���� ������
            qDebug() << "QSysDBase::QSysDBase.vars - Data is not available. RECREATE AND SET VALUES BY DEFAULT";

            //������� ��������, �.�. �� ����� � �� ����
            query.exec( "drop table vars" );
            query.exec( "create table vars (varval varchar(100), varname varchar(50) )" );

            //������� � ����������� ���������� ��� � �����.
            //��������� ������ �� ���� ����� ���� �� ����
            //��� ����� �������� �� ���������
            this->DOweSysvarSet( "Call", "NOCALL" );
            this->DOweSysvarSet( "Name", "NONAME" );
            this->DOweSysvarSet( "QTHN", "QTH-Name" );
            this->DOweSysvarSet( "Lat", "00.00.00N" );
            this->DOweSysvarSet( "Lng", "000.00.00E" );
            this->DOweSysvarSet( "DefaultUnproto", "WIDE1-1,WIDE2-2" );
            this->DOweSysvarSet( "Beacon", "NAME, QTH-NAME" );
            this->DOweSysvarSet( "Symbol", "/I" );
            this->DOweSysvarSet( "APRSCall", "APZ012" );
            this->DOweSysvarSet( "APRSVersion", versionName );
            this->DOweSysvarSet( "RevPath", "0" );
            this->DOweSysvarSet( "qryAPRSTen", "0" );
            this->DOweSysvarSet( "qryAPRSDen", "0" );
            this->DOweSysvarSet( "qryAPRSHen", "0" );
            this->DOweSysvarSet( "qryAPRSMen", "0" );
            this->DOweSysvarSet( "qryAPRSOen", "0" );
            this->DOweSysvarSet( "qryAPRSPen", "0" );
            this->DOweSysvarSet( "qryAPRSSen", "0" );
            this->DOweSysvarSet( "AXCODEPAGE", "IBM 866" );
            this->DOweSysvarSet( "MYCALLS", "" );
            this->DOweSysvarSet( "MSGID", "0" );
        }

        qDebug() << "QSysDBase::QSysDBase.ports - Try load from storage";

        query.prepare( "select count(*) as cnt from ports" );
        query.exec();
        query.first();

        if ( query.isActive() ) {
            //���� �� ������
            qDebug() << "QSysDBase::QSysDBase.ports - Data is available";

            query.prepare( "select port_num, port_type_id, port_note from ports" );

            query.exec();
            while ( query.next() ) {
                this->DOweSysportSetParam( query.value(0).toInt(), "PORT_TYPE", query.value(1).toString() );
                this->DOweSysportSetParam( query.value(0).toInt(), "PORT_NAME", query.value(2).toString() );

                query2.prepare( "select par_code, par_value "
                                "from port_param_values "
                                "where port_num=(:p1)" );

                query2.bindValue( "p1", query.value(0).toInt() );
                query2.exec();

                while (query2.next()) {
                    this->DOweSysportSetParam( query.value(0).toInt(), parNameFromId( query.value(1).toInt(), query2.value(0).toInt() ) , query2.value(1).toString() );
                }

            }
            //� ��� �� ���� ���������

            //������ ��� ���������, ������� �� �� ���� ���������
            int i;

            for(i=0;i<256;i++) {
                if ( SysPorts->port[ i ] != NULL ) {
                    SysPorts->port[ i ]->stored = true;
                }
            }


        } else {
            //���� ������
            qDebug() << "QSysDBase::QSysDBase.ports - Data is not available. RECREATE TABLES";

            query.exec( "drop table port_types" );
            query.exec( "create table port_types (port_type_id int primary key, "
                        "port_type_note varchar(100), port_type_not varchar(15) ) " );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(0,'APRS Internet Server Connection', 'Inet')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(1,'KISS TNC', 'KISS')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(2,'AGW CORE', 'AGW')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(3,'FLEX CORE(!)', 'FLEX')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(4,'[X]-NET CORE(!)', 'XNET')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(5,'AXIP', 'AXIP')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(6,'FL Digi', 'FLDIGI')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(7,'VAska', 'VASKA')" );
            query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                        "values(-1,'n\\a', 'n\\a')" );

            //�����
            query.exec( "drop table ports" );
            query.exec( "create table ports (port_num int primary key, "
                        "port_type_id int, port_note varchar(50) ) " );

            query.exec( "drop table port_param_values; " );
            query.exec( "create table port_param_values (port_num int , "
                        "par_code int, par_value varchar(50) ); " );


        }

        qDebug() << "QSysDBase::QSysDBase.beacons - Try load from storage";

        query.prepare( "select count(*) as cnt from beacons" );
        query.exec();
        query.first();

        if ( query.isActive() ) {
            //���� �� ������
            qDebug() << "QSysDBase::QSysDBase.beacons - Data is available";

            //���������� �����           0          1      2    3     4    5      6        7        8        9        10
            query.prepare( " select beacon_num, port_num, sym, call, lat, lng, bunproto, btext, binterval, stext, sinterval "
                           " from beacons " );

            query.exec();
            while (query.next()) {

                this->DOweSysbeaconSetParam( query.value(0).toInt(), "PORT_NUM",      query.value(1).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "SYM",           query.value(2).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "CALL",          query.value(3).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "LAT",           query.value(4).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "LNG",           query.value(5).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "UNPROTO",       query.value(6).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "TEXT",          query.value(7).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "INTERVAL",      query.value(8).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "STATTEXT",      query.value(9).toString() );
                this->DOweSysbeaconSetParam( query.value(0).toInt(), "STATINTERVAL",  query.value(10).toString() );

            }

            //������ ��� ���������, ������� �� �� ���� ���������
            int i;

            for(i=0;i<256;i++) {
                if ( SysBeacons->beacon[ i ] != NULL ) {
                    SysBeacons->beacon[ i ]->stored = true;
                }
            }

        } else {
            //���� ������
            query.exec( "drop table beacons" );
            query.exec( "create table beacons (beacon_num int primary key, "
                        "port_num int, sym char(50), call char(12), "
                        "lat char(9), lng char(10), "
                        "bUnproto char(250), bText char(250), bInterval int, sText char(250), sInterval int ) " );

        }


        qDebug() << "QSysDBase::QSysDBase.packets - Try load from storage";

        query.prepare( "select count(*) as cnt from packets" );
        query.exec();
        query.first();

        if ( query.isActive() ) {
            //���� �� ������
            qDebug() << "QSysDBase::QSysDBase.packets - Data is available";

            //���������� ������
            query.prepare( " select k, DT, port_num, trx, pto, pfrom, pvia, message "
                           " from packets t1 "
                           " order by k asc " );

            query.exec();
            while (query.next()) {

                this->SysPackets->addPacket( query.value(0).toInt(), query.value(1).toString(), query.value(2).toInt(), query.value(3).toString(), query.value(4).toString(), query.value(5).toString(), query.value(6).toString(), query.value(7).toString() );

                //say( "load packet k="+query.value(0).toString()+" ..." );

                this->SysPackets->packet_Key = query.value(0).toInt();


                parseAPRSPacket( query.value(0).toInt(), query.value(2).toInt(),
                                 query.value(3).toString(), query.value(1).toString(),
                                 query.value(4).toString(), query.value(5).toString(), query.value(6).toString(),
                                 query.value(7).toString() );

            }

            say( "Packet Count = " + QString::number( this->SysPackets->packetCount ) );
            say( "Station Count = " + QString::number( this->SysStations->stationCount ) );

            QAPRSPacket  *tmp;

            //������ ��� ���������, ������� �� �� ���� ���������
            tmp = this->SysPackets->first;

            while ( tmp != NULL ) {
                tmp->stored = true;

                tmp = tmp->next;
            }

        } else {
            //���� ������
            qDebug() << "QSysDBase::QSysDBase.packets - Data is not available. RECREATE";

            //������� �������
            query.exec( "drop table packets" );
            query.exec( "create table packets (K integer primary key, "
                        "DT char(25), port_num int, trx char(10), PTo char(20),"
                        "PFrom char(20), PVia char(50), Message char(250) ) " );

        }




        //����� ��� ��� ��������� - ����� � ����������� � ��
        iStorageType = stp;
        startSaveToStorage();

        //�� ��������� �����
        this->weSysportOpenAll();
        this->weSysbeaconOpenAll();

    }

}

void QAtomix::say( QString msg ) {

    qDebug() << QDateTime::currentDateTime().toString( "dd.MM.yyyy hh:mm:ss.zzz" ) << " - QAtomix(" << atomName() << ")::" << msg;

};

//���������� ��������� �����
void QAtomix::beServer( int port ) {

    say( "I server. Port = " + QString::number( port ) );

    im = 0;

    tcpServer = new QTcpServer( this );
    tcpServerConnection = new QTcpSocket(this);


    connect( tcpServer, SIGNAL( newConnection() ),
             this, SLOT( tcpServerNewConnection() ) );

    tcpServer->listen( QHostAddress::QHostAddress("127.0.0.1"), port );

    tcpPort = port;

}

void QAtomix::beClient( QString address, int port ) {

    say( "I Client. To Address = " + address + " Port = " + QString::number( port ) );

    im = 1;

    tcpClient = new QTcpSocket();

    tcpClient->connectToHost( address, port );

    connect( tcpClient, SIGNAL( readyRead() ), this, SLOT( tcpClientread() ) );
    connect( tcpClient, SIGNAL( connected() ), this, SLOT( tcpClientconnected() ) );

    connect( tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
             this,      SLOT(SocketError(QAbstractSocket::SocketError)));


}

void QAtomix::SocketError(QAbstractSocket::SocketError socketError)
{
    //��� ���������� �� Fortune Client
    /*
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    getFortuneButton->setEnabled(true);
    */

    emit SIGSocketError();
}

void QAtomix::tcpServerNewConnection () {
//�������������

    say( "Connection with another atom" );

    tcpServerConnection = tcpServer->nextPendingConnection();

    connect(tcpServerConnection, SIGNAL( readyRead() ),
            this, SLOT( tcpServerRead() ) );

    connect(tcpServerConnection, SIGNAL( disconnected () ),
            this, SLOT( tcpServerDisconnect () ) );

    tcpServer->close(); //!!!! ������������ ������ ������� �����

}

void QAtomix::tcpServerDisconnect () {
//������������

    say( "DisConnection with another atom" );

    tcpServerConnection->close();
    tcpServerConnection->disconnect();

    tcpServer->listen( QHostAddress::QHostAddress("127.0.0.1"), tcpPort );

}

void QAtomix::tcpServerRead (){
//������ ���������

    say( "I hear the other atom" );

    QByteArray datagram;
    //while (tcpGUIServerConnection->bytesAvailable()==0);
    datagram = tcpServerConnection->readAll();

    inData.append( datagram );

    //���� ���� ������ �������� ������
    if ( inData.indexOf("\n")>-1 ) {

        while ( inData.indexOf("\n")>-1 ) {

            //����� � �������� �� ���������

            weThink( sysDecoder->toUnicode( inData.left( inData.indexOf("\n") - 1 ) ) );

            inData = inData.mid( inData.indexOf("\n") + 1 ); //��������

        }

    }

}

void QAtomix::tcpClientconnected() {

    say( "Connection to another atom" );

    loadingMode = true;

    this->weSysvarReq();

    this->weSysportReqPorts();

    this->weSysbeaconReqBeacons();

    this->weSyspacketsReq(); //����������� ������

    doThink( "ALL DATA LOADED?" );

}

void QAtomix::tcpClientread(){
    //������ ���������

        //say( "I hear the other atom" );

        QByteArray datagram;
        //while (tcpGUIServerConnection->bytesAvailable()==0);
        datagram = tcpClient->readAll();

        inData.append( datagram );

        //���� ���� ������ �������� ������
        if ( inData.indexOf("\n")>-1 ) {

            while ( inData.indexOf("\n")>-1 ) {

                //����� � �������� �� ���������

                weThink( sysDecoder->toUnicode( inData.left( inData.indexOf("\n") - 1 ) ) );

                inData = inData.mid( inData.indexOf("\n") + 1 ); //��������

            }

        }

    }

void QAtomix::weThink( QString thought ) {

    QString Tthought;

    Tthought = thought;

    //say( "Tthought='"+Tthought+"'" );
    if ( ( Tthought.left( 4 ) == "LOAD" ) && ( loadingMode == true ) ) {
        //�������� LOAD � �������� ������
        //��� ���� ��, ��� ���� � ��������� loading �������� ������
        //� �������� ���� ������������� ��� �������, �� ����� ��

        Tthought = Tthought.mid( 5 ); //�������� LOAD

    }
    //say( "Tthought='"+Tthought+"'" );

    QStringList cmd = Tthought.split(' ');

    //say( "We think - '" + Tthought + "'" );

    //������  � ����� ��������� ���. ���� ���� �����, ������� ��� �������
    //��� ����� ��������� ���� ����� ����� ������ ������:
    //������_������� ������� ��������=��������
    //
    //��������� ����������
    //SYSVAR SET VAR_NAME=VAR_VALUE
    //
    //�����
    //SYSPORT_X SET PARAM=VALUE
    //SYSPORT_X DO OPEN|CLOSE|DELETE
    //SYSPORT_X EVENT EV_NAME???

    //SYSVAR SET - ������
    if ( Tthought == "ALL DATA LOADED?" ) {
        if ( iStorageType == 0 )  {
            doThink( "ALL DATA LOADED!" );
        }
    } else

    if ( Tthought == "ALL DATA LOADED!" ) {
        if ( loadingMode == true ) {
            loadingMode = false;
            say( "loadingMode = false" );
            emit storeProgress( "ALL LOADED" );
            emit SIGallLoaded( );
        }
    } else

    //SYSVAR SET - ������
    if ( Tthought == "SYSVAR LOAD" ) {
        //���� ������� ����� ��������� ����������, �� ����� ���, ��� �� ����� ���� �� �������
        //!!! ���� ��!!! ����� ����, ������� storage ������
        this->DOweSysvarReq();
    } else

    if ( Tthought == "SYSPORT LOAD" ) {
        this->DOweSysportReqPorts();
    } else

    if ( Tthought == "SYSBEACON LOAD" ) {
        this->DOweSysbeaconReqBeacons();
    } else

    if ( Tthought == "PACKETS LOAD" ) {
        this->DOweSyspacketsReq();
    } else


//"LOAD SYSPORT_0 SET PORT_TYPE="0" PORT_NAME="test1""
    if ( (cmd.at(0).toUpper() == "SYSVAR") ||
         (cmd.at(0).toUpper().left( 8 ) == "SYSPORT_") ||
         (cmd.at(0).toUpper() == "SYSBEACON") ||
         (cmd.at(0).toUpper().left( 10 ) == "SYSBEACON_" ) ||
         (cmd.at(0).toUpper() == "PACKET")

         ) {

        QString SysObject; //��� ���������� �������
        QString SysObjectCommand; //�������
        QString SysObjectCommandParameters; //��������(�) �������
        int     paramCount = 0;
        QString paramNames[256];
        QString paramValues[256];

        if ( cmd.count() > 0 ) SysObject = cmd.at(0).toUpper();
        if ( cmd.count() > 1 ) SysObjectCommand = cmd.at(1);
        if ( cmd.count() > 2 ) {
            //SysObjectCommandParameters = cmd.at(2);
            //�����������! ����� split ��������� �� ���������.
            //������ �������� ����� �� ������ � �������� ��������� � ������ ����
            SysObjectCommandParameters = Tthought;
            SysObjectCommandParameters = SysObjectCommandParameters.mid( SysObjectCommandParameters.indexOf( " " ) + 1 );
            SysObjectCommandParameters = SysObjectCommandParameters.mid( SysObjectCommandParameters.indexOf( " " ) + 1 );

        }


        QString tParameters = SysObjectCommandParameters;
        QString tParName="";
        QString tParVal="";
        int tParState=0; //0 - ����� ��� ���������; 1 - ����� ��������;

        //���� ������� ����������. ������ �����
        while ( tParameters != "" ) {

            tParName = "";
            tParVal  = "";

            //����� ��� ���������
            while ( tParameters.left( 1 ) != "=" ) {
                tParName = tParName + tParameters.left( 1 );
                tParameters = tParameters.mid( 1 );
            }

            tParameters = tParameters.mid( 1 ); // ������� ���� =

            //qDebug() << "tParameters=" << tParameters;

            //�������� ��������� ���������� " � ��������� " � ���� ���� " � ���������, �� \"
            if ( tParameters.left( 1 ) == "\"" ) {
                tParState = 1;
                tParameters = tParameters.mid( 1 );
            }

            while ( tParameters.left( 1 ) != "\"" ) {
                if ( tParameters.left( 1 ) == "\\" ) {
                    tParameters = tParameters.mid( 1 );
                    tParVal = tParVal + tParameters.left( 1 );
                    tParameters = tParameters.mid( 1 );
                } else {
                    tParVal = tParVal + tParameters.left( 1 );
                    tParameters = tParameters.mid( 1 );
                }
            }

            //����� ���������� "
            if ( tParameters.left( 1 ) == "\"" ) {
                tParState = 0;
                tParameters = tParameters.mid( 2 );
            }


            paramNames[ paramCount ] = tParName;
            paramValues[ paramCount ] = tParVal;

            paramCount = paramCount + 1;

            //qDebug() << "tParameters=" << tParameters;
        }


        //say( "SysOBJ=" + SysObject );
        int i;
        for(i=0;i<paramCount;i++) {

            //SYSVARS
            if ( SysObject == "SYSVAR" ) {
                if ( SysObjectCommand == "SET" ) {
                    this->DOweSysvarSet( paramNames[ i ], paramValues[ i ] );
                    emit storeProgress( "SYSVAR SET " + paramNames[ i ] + "=" + paramValues[ i ] );
                }
            }

            //SYSPORTS
            if ( SysObject.left( 8 ) == "SYSPORT_" ) {

                int pnum=-1;
                bool allports = ( SysObject.mid( SysObject.indexOf( "_" ) + 1 ) == "ALL" );

                if ( allports == true ) {
                    if ( SysObjectCommand == "DO" ) {
                        if ( paramNames[ i ] == "OPEN" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysportOpenAll();
                            if ( paramValues[ i ] == "FALSE" ) this->DOweSysportCloseAll();
                        }
                    }
                } else {
                    pnum = SysObject.mid( SysObject.indexOf( "_" ) + 1 ).toInt();
                }

                emit storeProgress( "SYSPORT_" + QString::number( pnum ) );

                if ( pnum > -1 ) {

                    if ( SysObjectCommand == "SET" ) {
                        this->DOweSysportSetParam( pnum, paramNames[ i ], paramValues[ i ] );
                    }

                    if ( SysObjectCommand == "EVENT" ) {

                        if ( paramNames[ i ] == "STATUS" ) {
                            emit SIGweSysportChangeState( pnum, paramValues[ i ] );
                        }

                    }

                    if ( SysObjectCommand == "DO" ) {
                        if ( paramNames[ i ] == "OPEN" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysportOpen( pnum );
                            if ( paramValues[ i ] == "FALSE" ) this->DOweSysportClose( pnum );
                        }
                        if ( paramNames[ i ] == "DELETE" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysportDelete( pnum );
                        }
                        if ( paramNames[ i ] == "UPDATE" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysportUpdate( pnum );
                        }
                    }

                    if ( SysObjectCommand == "GET" ) {
                        if ( paramNames[ i ] == "STATUS" ) {
                            if ( paramValues[ i ] == "?" ) this->DOweSysportReqStatus( pnum );
                        }
                        if ( paramNames[ i ] == "PARAMS" ) {
                            if ( paramValues[ i ] == "?" ) this->DOweSysportReqParams( pnum );
                        }

                    }

                }

            }

            //SYSBEACONS
            if ( SysObject.left( 10 ) == "SYSBEACON_" ) {

                int bnum=-1;
                bool allports = ( SysObject.mid( SysObject.indexOf( "_" ) + 1 ) == "ALL" );

                if ( allports == true ) {
                    if ( SysObjectCommand == "DO" ) {
                        if ( paramNames[ i ] == "OPEN" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysbeaconOpenAll();
                            if ( paramValues[ i ] == "FALSE" ) this->DOweSysbeaconCloseAll();
                        }
                    }
                } else {
                    bnum = SysObject.mid( SysObject.indexOf( "_" ) + 1 ).toInt();
                }

                emit storeProgress( "SYSBEACON_" + QString::number( bnum ) );

                if ( bnum > -1 ) {

                    if ( SysObjectCommand == "SET" ) {
                        this->DOweSysbeaconSetParam( bnum, paramNames[ i ], paramValues[ i ] );
                    }

                    if ( SysObjectCommand == "EVENT" ) {

                        if ( paramNames[ i ] == "STATUS" ) {
                            emit SIGweSysbeaconChangeState( bnum, paramValues[ i ] );
                        }

                    }

                    if ( SysObjectCommand == "DO" ) {
                        if ( paramNames[ i ] == "OPEN" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysbeaconOpen( bnum );
                            if ( paramValues[ i ] == "FALSE" ) this->DOweSysbeaconClose( bnum );
                        }
                        if ( paramNames[ i ] == "DELETE" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysbeaconDelete( bnum );
                        }
                        if ( paramNames[ i ] == "UPDATE" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysbeaconUpdate( bnum );
                        }
                        if ( paramNames[ i ] == "SEND" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysbeaconSend( bnum );
                        }
                        if ( paramNames[ i ] == "SENDSTATUS" ) {
                            if ( paramValues[ i ] == "TRUE" ) this->DOweSysbeaconStatusSend( bnum );
                        }
                    }

                    if ( SysObjectCommand == "GET" ) {
                        if ( paramNames[ i ] == "STATUS" ) {
                            if ( paramValues[ i ] == "?" ) this->DOweSysbeaconReqStatus( bnum );
                        }
                        if ( paramNames[ i ] == "PARAMS" ) {
                            if ( paramValues[ i ] == "?" ) this->DOweSysbeaconReqParams( bnum );
                        }

                    }

                }

            }

        } //���� �������� ����������


        //SYSPORTS - ��������� �������� ���������
        if ( SysObject.left( 8 ) == "SYSPORT_" ) {

            int pnum=-1;
            QString To;
            QString From;
            QString Via;
            QString MsgText;
            QString ACK;

            bool allports = ( SysObject.mid( SysObject.indexOf( "_" ) + 1 ) == "ALL" );

            if ( allports == true ) {
               /* if ( SysObjectCommand == "DO" ) {
                    if ( paramNames[ 0 ] == "MSGSEND" ) {
                        if ( paramValues[ 0 ] == "TRUE" ) this->DOweSysportOpenAll();
                        if ( paramValues[ i ] == "FALSE" ) this->DOweSysportCloseAll();
                    }
                } */
            } else {
                pnum = SysObject.mid( SysObject.indexOf( "_" ) + 1 ).toInt();
                if ( ( paramNames[ 0 ] == "MSGSEND" ) && (paramValues[ 0 ] == "TRUE" ) ) {

                    if ( paramNames[ 1 ] == "TO" )       To       = paramValues[ 1 ];
                    if ( paramNames[ 2 ] == "FROM" )     From     = paramValues[ 2 ];
                    if ( paramNames[ 3 ] == "VIA" )      Via      = paramValues[ 3 ];
                    if ( paramNames[ 4 ] == "MTEXT" )    MsgText  = paramValues[ 4 ];
                    if ( paramNames[ 5 ] == "ACK" )      ACK      = paramValues[ 5 ];

                    /*
                    qDebug() << "PORT_NUM=" << pnum;
                    qDebug() << "To=" << To;
                    qDebug() << "From=" << From;
                    qDebug() << "Via=" << Via;
                    qDebug() << "ACK=" << ACK; */

                    DOweSendMessage( pnum, To, From, Via, MsgText, ACK=="TRUE" );

                }

            }

            emit storeProgress( "SYSPORT_" + QString::number( pnum ) );



        }

        //PACKETS
        if ( SysObject == "PACKET" ) {

            //'PACKET EVENT PORT_NUM="0" TX="TRUE"'
            if ( SysObjectCommand == "EVENT_TRX" ) {

                int pnum=-1;

                if ( paramNames[ 0 ] == "PORT_NUM" ) pnum = paramValues[ 0 ].toInt();

                if ( pnum != -1 ) {

                    if ( paramNames[ 1 ] == "TX" ) emit SIGweportTXPacket( pnum, ( paramValues[ 1 ]=="TRUE" ) );
                    if ( paramNames[ 1 ] == "RX" ) emit SIGweportRXPacket( pnum, ( paramValues[ 1 ]=="TRUE" ) );

                }

            }

            //"Tthought='PACKET EVENT_RX PORT_NUM="0"; K="28"; DATETIME="12.10.2010 14:00:32.189"; TO="APZ012"; FROM="NOCALL"; VIA="WIDE1-1,WIDE2-2"; MsgText="=0000.00N/00000.00EINAME, QTH-NAME {QAPRS}"'"
            if ( ( SysObjectCommand == "EVENT_TX" ) || ( SysObjectCommand == "EVENT_RX" ) )  {

                int     port_num = -1;
                int     packet_k = -1;
                QString TRX;
                QString DateTime;
                QString To;
                QString From;
                QString Via;
                QString MsgText;

                if ( SysObjectCommand == "EVENT_TX" ) TRX = "TX";
                if ( SysObjectCommand == "EVENT_RX" ) TRX = "RX";

                if ( paramNames[ 0 ] == "PORT_NUM" ) port_num = paramValues[ 0 ].toInt();
                if ( paramNames[ 1 ] == "K" )        packet_k = paramValues[ 1 ].toInt();
                if ( paramNames[ 2 ] == "DATETIME" ) DateTime = paramValues[ 2 ];
                if ( paramNames[ 3 ] == "TO" )       To       = paramValues[ 3 ];
                if ( paramNames[ 4 ] == "FROM" )     From     = paramValues[ 4 ];
                if ( paramNames[ 5 ] == "VIA" )      Via      = paramValues[ 5 ];
                if ( paramNames[ 6 ] == "MsgText" )  MsgText  = paramValues[ 6 ];

                emit storeProgress( "PACKET " + TRX + " K = " + QString::number( packet_k ) );


/*
                qDebug() << "PORT_NUM=" << port_num;
                qDebug() << "packet_k=" << packet_k;
                qDebug() << "DateTime=" << DateTime;
                qDebug() << "To=" << To;
                qDebug() << "From=" << From;
                qDebug() << "Via=" << Via;
                qDebug() << "MsgText=" << MsgText;
*/


                if ( TRX == "RX" ) {

                    this->SysPackets->addPacket( packet_k, DateTime, port_num, "RX", To, From, Via, MsgText );

                    emit SIGportRXAPRSPacket( packet_k, port_num, DateTime, To, From, Via, MsgText );
                    parseAPRSPacket( packet_k, port_num, "RX", DateTime, To, From, Via, MsgText );
                }

                if ( TRX == "TX" ) {
                    this->SysPackets->addPacket( packet_k, DateTime, port_num, "TX", To, From, Via, MsgText );

                    emit SIGportTXAPRSPacket( packet_k, port_num, DateTime, To, From, Via, MsgText );
                    parseAPRSPacket( packet_k, port_num, "TX", DateTime, To, From, Via, MsgText );
                }

                startSaveToStorage();

            }

        }




    }



    emit SIGweThink( thought );

}

void QAtomix::doThink( QString thought ) {
//��������

    weThink( thought );

    if ( im == 0 ) {

        tcpServerConnection->write( sysDecoder->fromUnicode( thought ) + (QByteArray)("\r\n") );

    } else {

        tcpClient->write( sysDecoder->fromUnicode( thought ) + (QByteArray)("\r\n") );

    }


}

void QAtomix::portRXPacket( int pNum,  bool isRX ) {
//���� ����� ������
    this->weportRXPacket( pNum, isRX );
}

void QAtomix::weportRXPacket( int pNum,  bool isRX ) {
//���� ����� ������
    if ( isRX ) {
        //doThink( "EVENT RXPACKET PORT_NUM=" + QString::number( pNum )+"; RX=TRUE;" );
        doThink( "PACKET EVENT_TRX PORT_NUM=\"" + QString::number( pNum )+"\" RX=\"TRUE\"" );
    } else {
        //doThink( "EVENT RXPACKET PORT_NUM=" + QString::number( pNum )+"; RX=FALSE;" );
        doThink( "PACKET EVENT_TRX PORT_NUM=\"" + QString::number( pNum )+"\" RX=\"FALSE\"" );
    }

}

void QAtomix::portTXPacket( int pNum,  bool isRX ){
//���� �������� ������
    this->weportTXPacket( pNum, isRX );
}

void QAtomix::weportTXPacket( int pNum, bool isRX ) {
//���� �������� ������
    if ( isRX ) {
        //doThink( "EVENT TXPACKET PORT_NUM=" + QString::number( pNum )+"; TX=TRUE;" );
        doThink( "PACKET EVENT_TRX PORT_NUM=\"" + QString::number( pNum )+"\" TX=\"TRUE\"" );
    } else {
        //doThink( "EVENT TXPACKET PORT_NUM=" + QString::number( pNum )+"; TX=FALSE;" );
        doThink( "PACKET EVENT_TRX PORT_NUM=\"" + QString::number( pNum )+"\" TX=\"FALSE\"" );
    }
}

void QAtomix::portRXAPRSPacket( int pNum,  QString To, QString From, QString MsgText ){

    this->weportRXAPRSPacket( pNum, To, From, MsgText );

}

void QAtomix::portTXAPRSPacket( int pNum,  QString To, QString From, QString MsgText ){

    this->weportTXAPRSPacket( pNum, To, From, MsgText );

}

void QAtomix::weportRXAPRSPacket( int pNum,  QString To, QString From, QString MsgText ) {

        //qDebug() << "portRXAPRSPacket" << pNum << To << From << MsgText;
        QDateTime PacketDateTime;
        QString Via;
        QString ShFrom;
        int tk;

        PacketDateTime = QDateTime::currentDateTime();

        if (From.indexOf('>')!=-1) {
            Via = From.mid( From.indexOf('>') + 1 );
            ShFrom = From.left( From.indexOf('>') );
        } else {
            ShFrom = From;
            Via = "";
        }

        tk = this->SysPackets->packet_Key = this->SysPackets->packet_Key + 1;

        //parseAPRSPacket( this->SysPackets->packet_Key, pNum, "RX", PacketDateTime, To, From, MsgText );
        //'PACKET EVENT PORT_NUM="0" RX="FALSE"'
        doThink( "PACKET EVENT_RX PORT_NUM=\"" + QString::number( pNum ) +
                 "\" K=\"" + QString::number( tk ) +
                 "\" DATETIME=\"" + PacketDateTime.toString( "dd.MM.yyyy hh:mm:ss.zzz" ) +
                 "\" TO=\"" + To +
                 "\" FROM=\"" + ShFrom +
                 "\" VIA=\"" + Via +
                 "\" MsgText=\"" + trPar( MsgText ) + "\"" );

}

void QAtomix::weportTXAPRSPacket( int pNum,  QString To, QString From, QString MsgText ) {

    QDateTime PacketDateTime;
    QString Via;
    QString ShFrom;
    int tk;

    PacketDateTime = QDateTime::currentDateTime();

    if (From.indexOf('>')!=-1) {
        Via = From.mid( From.indexOf('>') + 1 );
        ShFrom = From.left( From.indexOf('>') );
    } else {
        ShFrom = From;
        Via = "";
    }

    tk = this->SysPackets->packet_Key = this->SysPackets->packet_Key + 1;

    //parseAPRSPacket( this->SysPackets->packet_Key, pNum, "RX", PacketDateTime, To, From, MsgText );
    doThink( "PACKET EVENT_TX PORT_NUM=\"" + QString::number( pNum ) +
             "\" K=\"" + QString::number( tk ) +
             "\" DATETIME=\"" + PacketDateTime.toString( "dd.MM.yyyy hh:mm:ss.zzz" ) +
             "\" TO=\"" + To +
             "\" FROM=\"" + ShFrom +
             "\" VIA=\"" + Via +
             "\" MsgText=\"" + trPar( MsgText ) + "\"" );

}

void QAtomix::portChangeState( int pNum,  int state ){
//����� ������� ����� (0-�������/1-��������/2-������);

    this->DOweSysportReqStatus( pNum ); //� ������ �� ��� ������� �� �����

}

void QAtomix::parseAPRSPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString From, QString Via, QString MsgText ) {


    QString msg;

    QString MTo;
    QString MMsg;
    QString MID;
    //QString Via;
    QString ShFrom;

    ShFrom = From;

    //if (From.indexOf('>')!=-1) {
    //    Via = From.mid( From.indexOf('>') + 1 );
    //    ShFrom = From.left( From.indexOf('>') );
    //} else {
    //    ShFrom = From;
    //    Via = "";
    //}


    //this->SysStations->stationSetParam( ShFrom, "LH", DateTime/*.toString( "dd.MM.yyyy hh:mm:ss.zzz" )*/ );
    //this->SysStations->stationSetParam( ShFrom, "via", Via );
    emit SIGstationSetParam( ShFrom, "LH", DateTime );
    emit SIGstationSetParam( ShFrom, "via", Via );

    msg.clear();

    //���� ����� �������� ����������, ��...
    if (MsgText.left(1)==":") {

        MTo = MsgText.mid(1, 9).trimmed();
        if ( MsgText.indexOf("{")==-1 ) {
            //����� �� ����� ID
            MMsg = MsgText.mid(11);
            MID = "";
        } else {
            MMsg = MsgText.mid(11, MsgText.length() - 11 - ( MsgText.length() - MsgText.indexOf("{") ));
            MID = MsgText.mid( MsgText.indexOf("{")+1 );
        }

        //����� � ����
        //QSqlQuery query;
        //query.prepare( "insert into messages ( packet_K, MTo, Message, Mess_ID ) "
        //            "values( :p1,:p2,:p3,:p4  )" );
        //query.bindValue(":p1", K );
        //query.bindValue(":p2", MTo );
        //query.bindValue(":p3", MMsg );
        //query.bindValue(":p4", MID );
        //query.exec();


        //�����! �� ����� ������ ������ doThink � ������ �������� ���������, ������� ��� �������
        //������ ���� ��� �������� � �������� ������. ������ ���� ������ ��
        //��� �������� � �������� ��������� � ���� ������, �� ��������� ����� 2:
        //�� ������� ����� � �� �������
        //�������� ��������� �������
        //if ( TRX == "RX" ) msg.append( "EVENT RXAPRSMESSAGE " );
        //if ( TRX == "TX" ) msg.append( "EVENT TXAPRSMESSAGE " );
        //
        //
        //msg.append( "PORT_NUM=" + QString::number( pNum ) +
        //            "; K=" + QString::number( K ) +
        //            "; DATETIME=\"" + DateTime/*.toString( "dd.MM.yyyy hh:mm:ss.zzz" )*/ +
        //            "\"; TO=\"" + MTo +
        //            "\"; FROM=\"" + ShFrom +
        //            "\"; MID=\"" + MID +
        //            "\"; MsgText=\"" + MMsg + "\"" );

        //doThink( msg );

        if ( TRX == "RX" ) {
            emit RCVRXAPRSMessage( pNum, K, DateTime, MTo, ShFrom, MMsg, MID );
        }
        if ( TRX == "TX" ) {
            emit RCVTXAPRSMessage( pNum, K, DateTime, MTo, ShFrom, MMsg, MID );
        }

    }


    if ( ( MsgText.left(1) == "=" ) || ( MsgText.left(1) == "!" ) )
        parseAPRSPositionPacket( K, pNum, TRX, DateTime, To, From, Via, MsgText );

    if ( ( MsgText.left(1) == ">" ) )
        parseAPRSStatusPacket( K, pNum, TRX, DateTime, To, From, Via, MsgText );


    //���� � ��� �� useStorage, �� �������� �� �������������� ������
    //� ����� �� �������� �� ���, �.�. �� �� ������ ����������!!!


    //���� iStorageType = 0 �� �� ��� ��������� � ��� ������ ��� ���������
    //� ������ �� ���������� ������ ����� ��������


    if ( ( iStorageType == 0 ) && ( this->SysVars->getVal( "MYCALLS" ).indexOf( MTo + ";" ) > -1 ) ) {
        //���� ��������� ���

        //���� MID �� ������, �� ����� ��������� �������������
        if ( MID != "" ) {

            QString Msg;

            Msg = ":" + ShFrom.leftJustified( 9, ' ' ) + ":ack" + MID;

            if ( this->SysPorts->port[ pNum ]->portType() == "Inet" ) {
                this->SysPorts->port[ pNum ]->sendAPRSPacket( this->SysVars->getVal( "APRSCall" ) , MTo + ">TCPIP*", Msg );
            } else {
                QString TTFrom = MTo;
                if ( this->SysVars->getVal( "DefaultUnproto" ) != "" ) MTo = MTo + ">" + this->SysVars->getVal( "DefaultUnproto" );

                this->SysPorts->port[ pNum ]->sendAPRSPacket( this->SysVars->getVal( "APRSCall" ), TTFrom, Msg );
            }

        }

        if ( ( this->SysVars->getVal( "qryAPRSPen" ) == "0" )&& ( MMsg == "?APRSP" ) ) {

            if ( this->SysBeacons != NULL ) {

                int i;

                for(i=0;i<256;i++) {

                    if ( this->SysBeacons->beacon[ i ] != NULL ) {
                        if ( ( this->SysBeacons->beacon[ i ]->call() == MTo) &&( this->SysBeacons->beacon[ i ]->portNum() == pNum ) ) {

                            this->SysBeacons->beacon[ i ]->sendBeacon();

                        }

                    }

                }

            }

        } //qryAPRSPen

        if ( ( this->SysVars->getVal( "qryAPRSSen" ) == "0" ) && ( MMsg == "?APRSS" ) ) {

            if ( this->SysBeacons != NULL ) {

                int i;

                for(i=0;i<256;i++) {

                    if ( this->SysBeacons->beacon[ i ] != NULL ) {
                        if ( ( this->SysBeacons->beacon[ i ]->call() == MTo) &&( this->SysBeacons->beacon[ i ]->portNum() == pNum ) ) {

                            this->SysBeacons->beacon[ i ]->sendStatus();

                        }

                    }

                }

            }

        } //qryAPRSSen

        if ( ( this->SysVars->getVal( "qryAPRSTen" ) == "0" ) && ( ( MMsg == "?APRST" ) || ( MMsg == "?PING?" ) ) ) {

            if ( this->SysBeacons != NULL ) {

                //���� ���� ������������, �� ���� ���, ��� ����� �� TCPIP*


                if ( this->SysPorts->port[ pNum ]->portType() == "Inet" ) {

                    QString Path;

                    Path.clear();
                    Path.append( ShFrom );
                    Path = Path.leftJustified(9, ' ');
                    Path.append(":");
                    Path.prepend(":");
                    Path.append( "Path - " + ShFrom + ">" + To );
                    if (Via.length()>0) {
                        Path.append( "," + Via.left( Via.indexOf( "*" ) + 1 ) );
                    }


                    this->SysPorts->port[ pNum ]->sendAPRSPacket( this->SysVars->getVal( "APRSCall" ) , MTo + ">TCPIP*", Path );

                } else {

                    QString Path;
                    QString TTFrom;
                    QString TTVia;
                    QString TTViaCall;
                    QString ActPath;
                    bool    digi;
                    int     pcnt;

                    Path.clear();
                    Path.append( ShFrom );
                    Path = Path.leftJustified(9, ' ');
                    Path.append(":");
                    Path.prepend(":");
                    Path.append( "Path - " + ShFrom + ">" + To );
                    //if (Via.length()>0) {
                    //    Path.append( "," + Via.left( Via.indexOf( "*" ) + 1 ) );
                    //}

                    ActPath = "";
                    qDebug() << "Via=" << Via;
                    TTVia = Via;
                    digi  = false;
                    pcnt  = 0;

                    if ( TTVia != "" ) {

                        //��� ���������� ��������� ��������� ������ �� ����� ������ CALL

                        do {

                            if ( TTVia.indexOf( "," ) == -1 ) {
                                TTViaCall = TTVia;
                                TTVia = "";
                            } else {

                                TTViaCall = TTVia.mid( TTVia.lastIndexOf( "," ) + 1 );
                                TTVia = TTVia.left( TTVia.lastIndexOf( "," ) );
                            }

                            if ( isDigi( TTViaCall ) ) {
                                digi = true;
                                if ( TTViaCall.right( 1 ) == "*" ) TTViaCall = TTViaCall.left( TTViaCall.length() - 1 );
                            }



                            if ( digi ) {
                                //qDebug() << "TTViaCall=" << TTViaCall << "DIGIPIATED";
                                if ( ( TTViaCall.left( 5 ).toUpper() == "TRACE" ) and ( TTViaCall.length() > 5 ) ) {
                                    //������� ���� �������� ��� ���������� � ����
                                } else {
                                    //�����  - ���������
                                    ActPath.prepend( TTViaCall + ","  );
                                    pcnt = pcnt + 1;
                                }
                            } else {
                                //qDebug() << "TTViaCall=" << TTViaCall << "no DIGI";
                            }


                        } while ( TTVia !="" );


                        if ( pcnt > 0 ) ActPath.data()[ ActPath.length() - 1 ] = '*';
                        //qDebug() << "ActPath=" << ActPath;

                    }

                    if ( ActPath != "" ) {
                        Path.append( "," + ActPath );
                    }


                    TTFrom = MTo;
                    if ( this->SysVars->getVal( "DefaultUnproto" ) != "" ) MTo = MTo + ">" + this->SysVars->getVal( "DefaultUnproto" );

                    this->SysPorts->port[ pNum ]->sendAPRSPacket( this->SysVars->getVal( "APRSCall" ), TTFrom, Path );

                }

            }

        } //qryAPRSTen

    }

}

void QAtomix::parseAPRSPositionPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString From, QString Via, QString MsgText ) {

    //QString Via;
    QString ShFrom;
    QString tmp;

    //if (From.indexOf('>')!=-1) {
    //    Via = From.mid( From.indexOf('>') + 1 );
    //    ShFrom = From.left( From.indexOf('>') );
    //} else {
    //    ShFrom = From;
    //    Via = "";
    //}
    ShFrom = From;

    //stationCall( ShFrom ).call = ShFrom;stationCall( ShFrom )
    //this->SysStations->stationSetParam( ShFrom, "LH", DateTime/*.toString( "dd.MM.yyyy hh:mm:ss.zzz" )*/ );
    //this->SysStations->stationSetParam( ShFrom, "via", Via );

    emit SIGstationSetParam( ShFrom, "LH", DateTime );
    emit SIGstationSetParam( ShFrom, "via", Via );



    //���������� ����������
    QString StLat, StLng;
    //qDebug() << MsgText;
    if ( ( MsgText.left(1) == "=" ) || ( MsgText.left(1) == "!") ) {

        /*

        ���� N ��� S � W ��� �
        � ��� �� ����� ����, �� ��� ������������� ������ */

        QString symbol;
        QString table;//or digi symb

        symbol = MsgText.mid( 19, 1 );
        table  = MsgText.mid(  9, 1 );

        if ( ( ( MsgText.mid( 8, 1 ).toUpper() == "N" ) || ( MsgText.mid( 8, 1 ).toUpper() == "S" ) ) &&
             ( ( MsgText.mid( 18, 1 ).toUpper() == "W" ) || ( MsgText.mid( 18, 1 ).toUpper() == "E" ) ) &&
             ( MsgText.mid( 5, 1 ) == "." ) && ( MsgText.mid( 15, 1 ) == "." )
            ) {

            StLat = MsgText.mid( 1, 2 ) + "." + MsgText.mid( 3, 2 ) + "." + MsgText.mid( 6, 3 );
            StLng = MsgText.mid( 10, 3 ) + "." + MsgText.mid( 13, 2 ) + "." + MsgText.mid( 16, 3 );

            //this->SysStations->stationSetParam( ShFrom, "sym", MsgText.mid( 9, 1 ) + MsgText.mid( 19, 1 ) );
            //this->SysStations->stationSetParam( ShFrom, "lat", StLat );
            //this->SysStations->stationSetParam( ShFrom, "lng", StLng );
            //this->SysStations->stationSetParam( ShFrom, "stcomment", MsgText.right( MsgText.length() - 20 ) );
            //this->SysStations->stationSetParam( ShFrom, "grid", real2qthloc( lat2real( StLat ), long2real( StLng ) ) );

            emit SIGstationSetParam( ShFrom, "sym", MsgText.mid( 9, 1 ) + MsgText.mid( 19, 1 ) );
            emit SIGstationSetParam( ShFrom, "lat", StLat );
            emit SIGstationSetParam( ShFrom, "lng", StLng );
            emit SIGstationSetParam( ShFrom, "stcomment", MsgText.right( MsgText.length() - 20 ) );
            emit SIGstationSetParam( ShFrom, "grid", real2qthloc( lat2real( StLat ), long2real( StLng ) ) );


            //������� ������ ���� ���������� ��� ��� ����
            emit RCVStationPositionInfo( ShFrom,
                                         MsgText.mid( 9, 1 ) + MsgText.mid( 19, 1 ),
                                         DateTime,
                                         StLat,
                                         StLng,
                                         real2qthloc( lat2real( StLat ), long2real( StLng ) ),
                                         Via,
                                         MsgText.right( MsgText.length() - 20 ) );

        } else {
            //����������� ������
            QByteArray CompPos;
            CompPos.clear();
            CompPos.append( MsgText.mid( 1, 13 ) );

            qreal x1, x2, x3, x4, y1, y2, y3, y4;
            y1 = uint(CompPos.data()[1]);
            y2 = uint(CompPos.data()[2]);
            y3 = uint(CompPos.data()[3]);
            y4 = uint(CompPos.data()[4]);
            x1 = uint(CompPos.data()[5]);
            x2 = uint(CompPos.data()[6]);
            x3 = uint(CompPos.data()[7]);
            x4 = uint(CompPos.data()[8]);

            StLat = real2lat ( ( -90 + ((y1-33) * 91 * 91 * 91 + (y2-33) * 91 * 91 + ((y3-33) * 91) + (y4-33))/380926)/ 90 );
            StLng = real2long( ( -180 + ((x1-33) * 91 * 91 * 91 + (x2-33) * 91 * 91 + ((x3-33) * 91) + (x4-33))/190463)/180 );

            //this->SysStations->stationSetParam( ShFrom, "sym", CompPos.mid( 0, 1 ) + CompPos.mid( 9, 1 ) );
            //this->SysStations->stationSetParam( ShFrom, "lat", StLat );
            //this->SysStations->stationSetParam( ShFrom, "lng", StLng );
            //this->SysStations->stationSetParam( ShFrom, "stcomment", MsgText.right( MsgText.length() - 14 ) );
            //this->SysStations->stationSetParam( ShFrom, "grid", real2qthloc( lat2real( StLat ), long2real( StLng ) ) );

            emit SIGstationSetParam( ShFrom, "sym", CompPos.mid( 0, 1 ) + CompPos.mid( 9, 1 ) );
            emit SIGstationSetParam( ShFrom, "lat", StLat );
            emit SIGstationSetParam( ShFrom, "lng", StLng );
            emit SIGstationSetParam( ShFrom, "stcomment", MsgText.right( MsgText.length() - 14 ) );
            emit SIGstationSetParam( ShFrom, "grid", real2qthloc( lat2real( StLat ), long2real( StLng ) ) );


            //�����! �� ����� ������ ������ doThink � ������ �������� ���������, ������� ��� �������
            //������ ���� ��� �������� � �������� ������. ������ ���� ������ ��
            //��� �������� � �������� ��������� � ���� ������, �� ��������� ����� 2:
            //�� ������� ����� � �� �������
            //tmp.clear();
            //tmp.append( "EVENT STATION POSITION " );
            //tmp.append( "CALL=\"" + ShFrom
            //            + "\"; SYM=\"" + CompPos.mid( 0, 1 ) + CompPos.mid( 9, 1 )
            //            + "\"; LH=\"" + DateTime/*.toString( "dd.MM.yyyy hh:mm:ss.zzz" )*/
            //            + "\"; LAT=\"" + StLat
            //            + "\"; LNG=\"" + StLng
            //            + "\"; GRID=\"" + real2qthloc( lat2real( StLat ), long2real( StLng ) )
            //            + "\"; VIA=\"" + Via
            //            + "\"; STCOMMENT=\"" + MsgText.right( MsgText.length() - 14 )
            //            + "\"" );
            //doThink( tmp );
            //������� ������ ���� ���������� ��� ��� ����
            emit RCVStationPositionInfo( ShFrom,
                                         CompPos.mid( 0, 1 ) + CompPos.mid( 9, 1 ),
                                         DateTime,
                                         StLat,
                                         StLng,
                                         real2qthloc( lat2real( StLat ), long2real( StLng ) ),
                                         Via,
                                         MsgText.right( MsgText.length() - 14 ) );

        }

    }

}

void QAtomix::parseAPRSStatusPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString From, QString Via, QString MsgText ) {

    //QString Via;
    QString ShFrom;
    QString tmp;

    //if (From.indexOf('>')!=-1) {
    //    Via = From.mid( From.indexOf('>') + 1 );
    //    ShFrom = From.left( From.indexOf('>') );
    //} else {
    //    ShFrom = From;
    //    Via = "";
    //}

    ShFrom = From;

    //stationCall( ShFrom ).call = ShFrom;stationCall( ShFrom )
    //this->SysStations->stationSetParam( ShFrom, "LH", DateTime/*.toString( "dd.MM.yyyy hh:mm:ss.zzz" )*/ );
    //this->SysStations->stationSetParam( ShFrom, "via", Via );
    //this->SysStations->stationSetParam( ShFrom, "ststatus", MsgText );

    emit SIGstationSetParam( ShFrom, "LH", DateTime );
    emit SIGstationSetParam( ShFrom, "via", Via );
    emit SIGstationSetParam( ShFrom, "ststatus", MsgText );


    //�����! �� ����� ������ ������ doThink � ������ �������� ���������, ������� ��� �������
    //������ ���� ��� �������� � �������� ������. ������ ���� ������ ��
    //��� �������� � �������� ��������� � ���� ������, �� ��������� ����� 2:
    //�� ������� ����� � �� �������
    //tmp.append( "EVENT STATION STATUS " );
    //tmp.append( "CALL=\"" + ShFrom + "\"; " );
    //tmp.append( "TEXT=\"" + MsgText+ "\"" );
    //doThink( tmp );

    emit RCVStationStatus( ShFrom, MsgText );

}


void QAtomix::beaconChangeState( int bNum,  int state ) {
//����� ������� ����� (0-�������/1-��������);

    this->DOweSysbeaconReqStatus( bNum );

}

void QAtomix::beaconSendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText ) {
//������� �����

    //qDebug() << "beaconSendBeacon bNum=" << bNum << " pNum=" << pNum << " To=" << To << " From=" << From << " MsgText=" << MsgText;
    if ( this->SysPorts->port[ pNum ] ) {
        this->SysPorts->port[ pNum ]->sendAPRSPacket( To, From, MsgText );
    }

}

//������ � ���������� �����������
void QAtomix::weSysvarSet( QString VarName, QString VarVal ) {

    doThink( "SYSVAR SET " + VarName + "=\"" + trPar( VarVal ) + "\"" );

}

void QAtomix::weSysvarReq() {

    doThink( "SYSVAR LOAD" );

}

void QAtomix::DOweSysvarSet( QString VarName, QString VarVal ) {

    if ( SysVars != NULL ) {
        //say( "SysVars set " + VarName + "=\"" + trPar( VarVal ) + "\"" );
        SysVars->setVal( VarName, VarVal );

        startSaveToStorage();

    }

    emit SIGweSysvarSet( VarName, VarVal );

}

void QAtomix::DOweSysvarReq() {

    if ( iStorageType == 0 ) {
        if ( SysVars != NULL ) {

            if ( SysVars->first != NULL ) {

                say( "I known SYSVARS!" );

                QSysVar *tmpvar;
                tmpvar = SysVars->first;
                while ( tmpvar != NULL ) {

                    doThink( "LOAD SYSVAR SET " + tmpvar->name + "=\"" + trPar( tmpvar->value ) + "\"" );
                    tmpvar = tmpvar->next;

                }


            } else {

                say( "I dont known SYSVARS!" );

            }

        }
    }

}

//������ � �������
//=============================================================================================================
//������� ��� �����
void QAtomix::weSysportOpenAll( ) {

    doThink( "SYSPORT_ALL DO OPEN=\"TRUE\"" );

}

//������� ��� �����
void QAtomix::weSysportCloseAll( ) {

    doThink( "SYSPORT_ALL DO OPEN=\"FALSE\"" );

}

//��������� ��� �����
//������������ ����� ������ ��������� ������: �����/���/�������� � ��� �� ������������� ������
void QAtomix::weSysportReqPorts( ) {

    doThink( "SYSPORT LOAD" );

}

//�������� ���������� �����
void QAtomix::weSysportOpen( int PortNum ) {

    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " OPEN" );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " DO OPEN=\"TRUE\"" );

}

//�������� ���������� �����
void QAtomix::weSysportClose( int PortNum ) {

    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " CLOSE" );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " DO OPEN=\"FALSE\"" );

}

//�������� ���������� �����
void QAtomix::weSysportDelete( int PortNum ) {

    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " DELETE" );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " DO DELETE=\"TRUE\"" );

}

//���������� ���������� �����
void QAtomix::weSysportUpdate( int PortNum ) {

    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " UPDATE" );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " DO UPDATE=\"TRUE\"" );

}

//��������� ��������� �����
void QAtomix::weSysportSetParam( int PortNum, QString ParName, QString ParVal ) {

    if ( ( ParName != "" ) && ( ParVal != "" ) )
    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " SET " + ParName + "=" + ParVal );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " SET " + ParName + "=\"" + trPar( ParVal ) + "\"" );

}

//��������� ��� ��������� ����
void QAtomix::weSysportReqParams( int PortNum ) {

    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " SET" );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " GET PARAMS=\"?\"" );

}

//��������� ������ �����
void QAtomix::weSysportReqStatus( int PortNum ) {

    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " GET STATE" );
    doThink( "SYSPORT_" + QString::number( PortNum ) + " GET STATUS=\"?\"" );


}


//������ � �������
//==========================================================================================================
//��������� ��������� �����
void QAtomix::weSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum )  + " SET " + ParName + "=" + ParVal );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " SET " + ParName + "=\"" + trPar( ParVal ) + "\"" );

    //qDebug() << ( "SYSBEACON_" + QString::number( BcknNum ) + " SET " + ParName + "=\"" + trPar( ParVal ) + "\"" );

}

//�������� ���������� �����
void QAtomix::weSysbeaconOpen( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " OPEN" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " DO OPEN=\"TRUE\"" );

}

//�������� ���������� �����
void QAtomix::weSysbeaconClose( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " CLOSE" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " DO OPEN=\"FALSE\"" );

}

//�������� ���������� �����
void QAtomix::weSysbeaconDelete( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " DELETE" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " DO DELETE=\"TRUE\"" );

}

//�������� ���������� �����
void QAtomix::weSysbeaconSend( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " SEND" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " DO SEND=\"TRUE\"" );

}

//�������� ������� ���������� �����
void QAtomix::weSysbeaconStatusSend( int BcknNum ) {

    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " DO SENDSTATUS=\"TRUE\"" );

}

//���������� ���������� �����
void QAtomix::weSysbeaconUpdate( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " UPDATE" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " DO UPDATE=\"TRUE\"" );

}

//������������ ����� ������ ��������� ������
void QAtomix::weSysbeaconReqBeacons( ) {

    doThink( "SYSBEACON LOAD" );

}

//��������� ��� ��������� �����
void QAtomix::weSysbeaconReqParams( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " SET" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " GET PARAMS=\"?\"" );

}

//��������� ������ �����
void QAtomix::weSysbeaconReqStatus( int BcknNum ) {

    //doThink( "SYSBEACON BCN_NUM=" + QString::number( BcknNum ) + " GET STATE" );
    doThink( "SYSBEACON_" + QString::number( BcknNum ) + " GET STATUS=\"?\"" );

}

//������� ��� �����
void QAtomix::weSysbeaconOpenAll( ) {

    //doThink( "SYSBEACON ALL OPEN" );
    doThink( "SYSBEACON_ALL DO OPEN=\"TRUE\"" );

}

//������� ��� �����
void QAtomix::weSysbeaconCloseAll( ) {

    //doThink( "SYSBEACON ALL CLOSE" );
    doThink( "SYSBEACON_ALL DO OPEN=\"FALSE\"" );

}

//��������� ��� ������
void QAtomix::weSyspacketsReq( ) {
//PACKETS GET LAST 100
    doThink( "PACKETS LOAD" );

}

void QAtomix::weSendMessage( int portNum, QString To, QString From, QString Via, QString MsgText, bool ACK ) {

    QString ackNeed = "FALSE";

    if ( ACK == true ) {
        ackNeed = "TRUE";
    }

    doThink( "SYSPORT_" + QString::number( portNum ) + " DO MSGSEND=\"TRUE\""
             " TO=\"" + To + "\""
             " FROM=\"" + From + "\""
             " VIA=\"" + Via + "\""
             " MTEXT=\"" + trPar( MsgText ) + "\""
             " ACK=\"" + ackNeed + "\"");

 /*   say( "SYSPORT_" + QString::number( portNum ) + " DO MSGSEND=\"TRUE\""
             " TO=\"" + To + "\""
             " FROM=\"" + From + "\""
             " VIA=\"" + Via + "\""
             " MTEXT=\"" + trPar( MsgText ) + "\""
             " ACK=\"" + ackNeed + "\""); */

}

//==========================================================================================================
void QAtomix::DOweSysportOpenAll( ) {
//� ��� ��� ��������� �������� ������, ������ ������ ���, ��� �� �����
    if ( SysPorts != NULL ) {

        int i;

        for(i=0;i<256;i++) {

            if ( SysPorts->port[ i ] != NULL ) {

                SysPorts->port[ i ]->openPort();

            }

        }

    }

}

void QAtomix::DOweSysportCloseAll( ) {

    if ( SysPorts != NULL ) {

        int i;

        for(i=0;i<256;i++) {

            if ( SysPorts->port[ i ] != NULL ) {

                SysPorts->port[ i ]->closePort();

            }

        }

    }

}

void QAtomix::DOweSysportOpen( int PortNum ) {

    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ PortNum ] != NULL ) {

            SysPorts->port[ PortNum ]->openPort();

        }

    }

}

void QAtomix::DOweSysportClose( int PortNum ) {

    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ PortNum ] != NULL ) {

            SysPorts->port[ PortNum ]->closePort();

        }

    }

}

void QAtomix::DOweSysportDelete( int PortNum ) {

    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ PortNum ] != NULL ) {

            SysPorts->port[ PortNum ]->closePort();
            SysPorts->port[ PortNum ]->disconnect();
            delete SysPorts->port[ PortNum ];
            SysPorts->port[ PortNum ] = NULL;


            //doThink( "EVENT CHANGESTATE PORT_NUM=" + QString::number( PortNum ) + "; STATUS=DELETED;" );
            this->weSysportReqStatus( PortNum );

            sysDBNeedDeleteSysPorts = true; //������� ����, ��� ���� ���-�� ���������
            startSaveToStorage();


        }

    }

}

void QAtomix::DOweSysportUpdate( int PortNum ) {

    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ PortNum ] != NULL ) {

            SysPorts->port[ PortNum ]->updatePort();

            this->weSysportReqStatus( PortNum );

        }

    }

}

void QAtomix::DOweSysportReqPorts() {

    if ( iStorageType == 0 ) {

        say( "doing SYSPORT LOAD" );
        if ( SysPorts != NULL ) {

            int i;

            for(i=0;i<256;i++) {

                if ( SysPorts->port[ i ] != NULL ) {

                    //this->weSysportSetParam( i, "PORT_TYPE", SysPorts->port[ i ]->portTypeID() );
                    //his->weSysportSetParam( i, "PORT_NAME", SysPorts->port[ i ]->portName() );

                    //this->weSysportReqStatus( i );

                    //if ( ( ParName != "" ) && ( ParVal != "" ) )
                    //doThink( "SYSPORT PORT_NUM=" + QString::number( PortNum ) + " SET " + ParName + "=" + ParVal );

                    doThink( "LOAD SYSPORT_" + QString::number( i )
                             + " SET PORT_TYPE=\"" + SysPorts->port[ i ]->portTypeID()
                             + "\" PORT_NAME=\"" + trPar( SysPorts->port[ i ]->portName() ) + "\"" );

                    QString status;
                        switch ( SysPorts->port[ i ]->portStatus() ) {
                                case 0 :
                                          status = "ON";
                                          break;
                                case 1 :
                                          status = "OFF";
                                          break;
                                case 2 :
                                          status = "ERROR";
                                          break;
                        };

                    doThink( "LOAD SYSPORT_" + QString::number( i )
                             + " EVENT STATUS=\"" + status + "\"" );


                }

            }

        }

    }

}

void QAtomix::DOweSysportSetParam( int PortNum, QString ParName, QString ParVal ) {

    //say( "DOweSysportSetParam PortNum="+QString::number( PortNum )+" "+ParName+"="+ParVal );

    emit SIGweSysportSetParam( PortNum, ParName, ParVal );

    if ( SysPorts != NULL ) {

        if ( ParName=="PORT_TYPE" ) {

            if ( SysPorts->port[ PortNum ] == NULL) {

                if ( ParVal=="5" ) SysPorts->port[ PortNum ] = new QAPRSAXIPPORT();
                if ( ParVal=="0" ) SysPorts->port[ PortNum ] = new QAPRSINTERNETPORT();
                if ( ParVal=="2" ) SysPorts->port[ PortNum ] = new QAPRSAGWPORT();
                if ( ParVal=="1" ) SysPorts->port[ PortNum ] = new QAPRSKISSPORT();
                if ( ParVal=="6" ) SysPorts->port[ PortNum ] = new QAPRSFLDIGIPORT();
                if ( ParVal=="7" ) SysPorts->port[ PortNum ] = new QAPRSVASKAPORT();

                connect ( SysPorts->port[ PortNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
                connect ( SysPorts->port[ PortNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
                connect ( SysPorts->port[ PortNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );
                connect ( SysPorts->port[ PortNum ], SIGNAL( RXAPRSPacket( int, QString, QString, QString ) ), this, SLOT( portRXAPRSPacket( int, QString, QString, QString ) ) );
                connect ( SysPorts->port[ PortNum ], SIGNAL( TXAPRSPacket( int, QString, QString, QString ) ), this, SLOT( portTXAPRSPacket( int, QString, QString, QString ) ) );

            } else

                if ( SysPorts->port[ PortNum ] != NULL) {
                    //���� ��� ����� �� ���������
                    if ( SysPorts->port[ PortNum ]->portTypeID() != ParVal ) {

                        SysPorts->port[ PortNum ]->closePort();
                        SysPorts->port[ PortNum ]->disconnect();
                        delete SysPorts->port[ PortNum ];
                        SysPorts->port[ PortNum ] = NULL;

                        if ( ParVal=="5" ) SysPorts->port[ PortNum ] = new QAPRSAXIPPORT();
                        if ( ParVal=="0" ) SysPorts->port[ PortNum ] = new QAPRSINTERNETPORT();
                        if ( ParVal=="2" ) SysPorts->port[ PortNum ] = new QAPRSAGWPORT();
                        if ( ParVal=="1" ) SysPorts->port[ PortNum ] = new QAPRSKISSPORT();
                        if ( ParVal=="6" ) SysPorts->port[ PortNum ] = new QAPRSFLDIGIPORT();
                        if ( ParVal=="7" ) SysPorts->port[ PortNum ] = new QAPRSVASKAPORT();

                        connect ( SysPorts->port[ PortNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
                        connect ( SysPorts->port[ PortNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
                        connect ( SysPorts->port[ PortNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );
                        connect ( SysPorts->port[ PortNum ], SIGNAL( RXAPRSPacket( int, QString, QString, QString ) ), this, SLOT( portRXAPRSPacket( int, QString, QString, QString ) ) );
                        connect ( SysPorts->port[ PortNum ], SIGNAL( TXAPRSPacket( int, QString, QString, QString ) ), this, SLOT( portTXAPRSPacket( int, QString, QString, QString ) ) );

                    }

                }

                if ( SysPorts->port[ PortNum ] != NULL) {
                    SysPorts->port[ PortNum ]->setPortNum( PortNum );
                    SysPorts->port[ PortNum ]->SysVars = this->SysVars;
                }


        } else {

            if ( SysPorts->port[ PortNum ] != NULL) {

                //say( "sysport["+QString::number( PortNum )+"].setpar("+ParName+","+ParVal+")" );
                SysPorts->port[ PortNum ]->setParam( ParName, ParVal );

            }

        }

        startSaveToStorage();

    }

}

void QAtomix::DOweSysportReqParams( int PortNum ) {

    //say( "DOweSysportReqParams PortNum="+QString::number( PortNum ) );

    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ PortNum ] != NULL ) {

            //this->weSysportSetParam( PortNum, "PORT_TYPE", SysPorts->port[ PortNum ]->portTypeID() );
            //this->weSysportSetParam( PortNum, "PORT_NAME", SysPorts->port[ PortNum ]->portName() );
            doThink( "LOAD SYSPORT_" + QString::number( PortNum ) +
                     " SET PORT_TYPE=\"" + SysPorts->port[ PortNum ]->portTypeID() + "\"" );

            doThink( "LOAD SYSPORT_" + QString::number( PortNum ) +
                     " SET PORT_NAME=\"" + trPar( SysPorts->port[ PortNum ]->portName() ) + "\"" );

            int i;
            for(i=0;i<( SysPorts->port[ PortNum ]->getParamCount() );i++) {
                //this->weSysportSetParam( PortNum, SysPorts->port[ PortNum ]->getParamName( i ), SysPorts->port[ PortNum ]->getParamValue( i ) );
                doThink( "LOAD SYSPORT_" + QString::number( PortNum ) +
                         " SET " + SysPorts->port[ PortNum ]->getParamName( i ) +
                         "=\"" + trPar( SysPorts->port[ PortNum ]->getParamValue( i ) ) + "\"" );

            }

        }

    }

}

void QAtomix::DOweSysportReqStatus( int PortNum ) {

    //say( "DOweSysportReqStatus PortNum="+QString::number( PortNum ) );

     //�������� � �������� ������ ���� ���� ���� �����. ������ ��� ���� ����� DELETED,
    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ PortNum ] != NULL ) {
            QString status;

            switch ( SysPorts->port[ PortNum ]->portStatus() ) {
                    case 0 :
                              status = "ON";
                              break;
                    case 1 :
                              status = "OFF";
                              break;
                    case 2 :
                              status = "ERROR";
                              break;
            };

            //doThink( "EVENT CHANGESTATE PORT_NUM=" + QString::number( PortNum ) + "; STATUS=" + status + ";" );
            doThink( "SYSPORT_" + QString::number( PortNum )
                     + " EVENT STATUS=\"" + status + "\"" );
        } else {
            //doThink( "EVENT CHANGESTATE PORT_NUM=" + QString::number( PortNum ) + "; STATUS=DELETED;" );
            doThink( "SYSPORT_" + QString::number( PortNum )
                     + " EVENT STATUS=\"DELETED\"" );
        }

    }


}
//===========================================================================================================
//������ � �������
//
void QAtomix::DOweSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal ) {

    emit SIGweSysbeaconSetParam( BcknNum, ParName, ParVal );

    //qDebug() << "!!!!!!!!!! DOweSysbeaconSetParam BcknNum=" << BcknNum << " ParName=" << ParName << " ParVal=" << ParVal;

    if ( SysBeacons != NULL ) {

        //say("SysBeacons != NULL");

        if ( SysBeacons->beacon[ BcknNum ] == NULL ) {

            //say("create beacon");

            SysBeacons->beacon[ BcknNum ] = new QAPRSBeacon();
            SysBeacons->beacon[ BcknNum ]->setBeaconNum( BcknNum );
            SysBeacons->beacon[ BcknNum ]->SysVars = this->SysVars;

            connect ( SysBeacons->beacon[ BcknNum ], SIGNAL( changeState( int, int ) ),
                      this,                          SLOT( beaconChangeState( int, int ) ) );

            connect ( SysBeacons->beacon[ BcknNum ], SIGNAL( sendBeacon( int,  int, QString, QString, QString ) ),
                      this,                          SLOT( beaconSendBeacon( int,  int, QString, QString, QString ) ) );

            this->weSysbeaconReqStatus( BcknNum );

        }

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->setParam( ParName, ParVal );

        }


        startSaveToStorage();

    }

}

void QAtomix::DOweSysbeaconOpen( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->openBeacon();

        }

    }

}

void QAtomix::DOweSysbeaconClose( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->closeBeacon();

        }

    }

}

void QAtomix::DOweSysbeaconDelete( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->closeBeacon();
            SysBeacons->beacon[ BcknNum ]->disconnect();
            delete SysBeacons->beacon[ BcknNum ];
            SysBeacons->beacon[ BcknNum ] = NULL;


            this->weSysbeaconReqStatus( BcknNum );

            sysDBNeedDeleteSysBeacons = true; //������� ����, ��� ���� ���-�� ���������
            startSaveToStorage();


        }

    }

}

void QAtomix::DOweSysbeaconSend( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->sendBeacon();

        }

    }

}

void QAtomix::DOweSysbeaconStatusSend( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->sendStatus();

        }

    }

}

void QAtomix::DOweSysbeaconUpdate( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            SysBeacons->beacon[ BcknNum ]->updateBeacon();

        }

    }

}

void QAtomix::DOweSysbeaconReqBeacons( ) {

    if ( SysBeacons != NULL ) {

        int i;

        for(i=0;i<256;i++) {

            if ( SysBeacons->beacon[ i ] != NULL ) {

                doThink( "LOAD SYSBEACON_" + QString::number( i )
                         + " SET"
                         + " PORT_NUM=\"" +     trPar( SysBeacons->beacon[ i ]->getParam( "PORT_NUM" ) ) + "\""
                         + " SYM=\"" +          trPar( SysBeacons->beacon[ i ]->getParam( "SYM" ) ) + "\""
                         + " CALL=\"" +         trPar( SysBeacons->beacon[ i ]->getParam( "CALL" ) ) + "\""
                         + " LAT=\"" +          trPar( SysBeacons->beacon[ i ]->getParam( "LAT" ) ) + "\""
                         + " LNG=\"" +          trPar( SysBeacons->beacon[ i ]->getParam( "LNG" ) ) + "\""
                         + " UNPROTO=\"" +      trPar( SysBeacons->beacon[ i ]->getParam( "UNPROTO" ) ) + "\""
                         + " TEXT=\"" +         trPar( SysBeacons->beacon[ i ]->getParam( "TEXT" ) ) + "\""
                         + " INTERVAL=\"" +     trPar( SysBeacons->beacon[ i ]->getParam( "INTERVAL" ) ) + "\""
                         + " STATTEXT=\"" +     trPar( SysBeacons->beacon[ i ]->getParam( "STATTEXT" ) ) + "\""
                         + " STATINTERVAL=\"" + trPar( SysBeacons->beacon[ i ]->getParam( "STATINTERVAL" ) ) + "\""
                       );


                QString status;
                    switch ( SysBeacons->beacon[ i ]->beaconStatus() ) {
                            case 0 :
                                      status = "ON";
                                      break;
                            case 1 :
                                      status = "OFF";
                                      break;

                    };

                doThink( "LOAD SYSBEACON_" + QString::number( i )
                         + " EVENT STATUS=\"" + status + "\"" );

            }

        }

    }

}

void QAtomix::DOweSysbeaconReqParams( int BcknNum ) {

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {

            int i;
            for(i=0;i<( SysBeacons->beacon[ BcknNum ]->getParamCount() );i++) {

                //this->weSysbeaconSetParam( BcknNum, SysBeacons->beacon[ BcknNum ]->getParamName( i ), SysBeacons->beacon[ BcknNum ]->getParamValue( i ) );

                doThink( "LOAD SYSBEACON_" + QString::number( BcknNum ) +
                             " SET " + SysBeacons->beacon[ BcknNum ]->getParamName( i ) +
                             "=\"" + trPar( SysBeacons->beacon[ BcknNum ]->getParamValue( i ) ) + "\"" );


            }

        }

    }

}

void QAtomix::DOweSysbeaconReqStatus( int BcknNum ) {

    say("DOweSysbeaconReqStatus");

    if ( SysBeacons != NULL ) {

        if ( SysBeacons->beacon[ BcknNum ] != NULL ) {
            QString status;

            switch ( SysBeacons->beacon[ BcknNum ]->beaconStatus() ) {
                    case 0 :
                              status = "ON";
                              break;
                    case 1 :
                              status = "OFF";
                              break;
            };

            //doThink( "EVENT CHANGESTATE BCN_NUM=" + QString::number( BcknNum ) + "; STATUS=" + status + ";" );
            doThink( "SYSBEACON_" + QString::number( BcknNum )
                     + " EVENT STATUS=\"" + status + "\"" );
        } else {
            //doThink( "EVENT CHANGESTATE BCN_NUM=" + QString::number( BcknNum ) + "; STATUS=DELETED;" );
            doThink( "SYSBEACON_" + QString::number( BcknNum )
                     + " EVENT STATUS=\"DELETED\"" );
        }

    }

}

void QAtomix::DOweSysbeaconOpenAll( ) {

    if ( SysBeacons != NULL ) {

        int i;

        for(i=0;i<256;i++) {

            if ( SysBeacons->beacon[ i ] != NULL ) {

                SysBeacons->beacon[ i ]->openBeacon();

            }

        }

    }

}


void QAtomix::DOweSysbeaconCloseAll( ) {

    if ( SysBeacons != NULL ) {

        int i;

        for(i=0;i<256;i++) {

            if ( SysBeacons->beacon[ i ] != NULL ) {

                SysBeacons->beacon[ i ]->closeBeacon();

            }

        }

    }

}

void QAtomix::DOweSyspacketsReq( ) {

    if ( ( im == 0 ) && ( this->SysPackets != NULL ) ) {

        QAPRSPacket *packetptr;

        packetptr = this->SysPackets->first;

        while ( packetptr!=NULL ) {

            //if ( packetptr->TRX=="RX" )
            //    this->weportRXAPRSPacket( packetptr->Port_Num, packetptr->PTo, packetptr->PFrom+">"+packetptr->PVia, packetptr->Message );
//
            //if ( packetptr->TRX=="TX" )
            //    this->weportTXAPRSPacket( packetptr->Port_Num, packetptr->PTo, packetptr->PFrom+">"+packetptr->PVia, packetptr->Message );
            /*
            qDebug() << "k=" << packetptr->K;
            qDebug() << "pnum=" << packetptr->Port_Num;
            qDebug() << "DT=" << packetptr->DT;
            qDebug() << "trx=" << packetptr->TRX;
            qDebug() << "from=" << packetptr->PFrom;
            qDebug() << "to=" << packetptr->PTo;
            qDebug() << "via=" << packetptr->PVia;
            qDebug() << "message=" << packetptr->Message;
            qDebug() << "----------------------------";

            doThink( "LOAD APRSPACKET PORT_NUM=" + QString::number( packetptr->Port_Num ) +
                     "; K=" + QString::number( packetptr->K ) +
                     "; DATETIME=\"" + packetptr->DT +
                     "\"; TRX=\"" + packetptr->TRX +
                     "\"; TO=\"" + packetptr->PTo +
                     "\"; FROM=\"" + packetptr->PFrom +
                     "\"; VIA=\"" + packetptr->PVia +
                     "\"; MsgText=\"" + packetptr->Message + "\""); */

            doThink( "LOAD PACKET EVENT_" + packetptr->TRX + " PORT_NUM=\"" + QString::number( packetptr->Port_Num ) +
                     "\" K=\"" + QString::number( packetptr->K ) +
                     "\" DATETIME=\"" + packetptr->DT +
                     "\" TO=\"" + packetptr->PTo +
                     "\" FROM=\"" + packetptr->PFrom +
                     "\" VIA=\"" + packetptr->PVia +
                     "\" MsgText=\"" + trPar( packetptr->Message ) + "\"" );

            packetptr = packetptr->next;

        }

        //doThink( "LOAD APRSPACKET ALL" );

    }

}

void QAtomix::DOweSendMessage( int portNum, QString To, QString From, QString Via, QString MsgText, bool ACK ) {

    QString Msg;
    QString MTo;

    MTo = To;

    Msg = ":" + MTo.leftJustified( 9, ' ' ) + ":" + MsgText;

    if ( ACK == true ) {
        Msg = Msg + "{" + this->SysVars->getVal( "MSGID" );
        this->SysVars->setVal( "MSGID", QString::number( (this->SysVars->getVal( "MSGID" ).toInt() + 1 ) ) );
    }


/*
    qDebug() << "portNum = " << portNum;
    qDebug() << "To = " << To;
    qDebug() << "From = " << From;
    qDebug() << "Via = " << Via;
    qDebug() << "MsgText = " << MsgText;
    qDebug() << "ACK = " << ACK; */

    if ( SysPorts != NULL ) {

        if ( SysPorts->port[ portNum ] != NULL ) {

                if ( this->SysPorts->port[ portNum ]->portType() == "Inet" ) {
                    this->SysPorts->port[ portNum ]->sendAPRSPacket( this->SysVars->getVal( "APRSCall" ) , From + ">TCPIP*", Msg );
                    say( "send = " + Msg );
                } else {
                    if ( Via != "" ) MTo = MTo + ">" + Via;
                    this->SysPorts->port[ portNum ]->sendAPRSPacket( this->SysVars->getVal( "APRSCall" ), From, Msg );
                    say( "send = " + Msg );
                }

        }

    }

}

//===========================================================================================================
//����. � �������� ������������ ������ ��� ������� ������ ��������� � ��������� ��
void QAtomix::run() {

    say( "QAtomix::run() start ");
    started = true;

    if ( iStorageType == 0 ) {

        say( "QAtomix::run() iStorageType == 0 ");

        bool    var_saved; //������� ����, ��� ���-�� ��������� �� ������ �� �������

        var_saved = true;

        while ( var_saved == true ) {

            //say( "QAtomix::run() while ( var_saved == true ) ");

            var_saved = false; //���� ������ �� ��������� ��� ����������, �� ������

            //��������� ����������
            QSysVar *varptr;
            varptr = SysVars->first;

            while ( varptr!=NULL ) {

                //say( "QAtomix::run() while ( varptr!=NULL ) ");

                //say( "QAtomix::run() sysvar " + varptr->name + "=" + varptr->value );

                if ( varptr->stored == false ) {

                    var_saved = true;

                    varptr->stored = true;

                    QSqlQuery query;

                    qDebug() << "QAtomix::run() save " << varptr->name << "=" << varptr->value;
                    emit storeProgress( "Sysvar save " + varptr->name + "=" + varptr->value );


                    query.prepare( "update vars set varval = (:p1) where varname = (:p2)" );
                    query.bindValue( "p1", varptr->value );
                    query.bindValue( "p2", varptr->name );
                    query.exec();

                    if ( query.numRowsAffected() == 0 ) {
                        QSqlQuery query2;
                        query2.prepare( "insert into vars(varval, varname) "
                                        "values((:p1),(:p2)) " );
                        query2.bindValue( "p1", varptr->value );
                        query2.bindValue( "p2", varptr->name );

                        query2.exec();
                    }

                }

                varptr = varptr->next;

            }

            //��� �������� ������ �����
            if ( sysDBNeedDeleteSysPorts == true ) {

                say("sysDBNeedDeleteSysPorts == true");

                while ( sysDBNeedDeleteSysPorts == true ) {

                    sysDBNeedDeleteSysPorts = false;
                    int i;
                    for(i=0;i<256;i++) {

                        if ( this->SysPorts->port[ i ] == NULL ) {

                            QSqlQuery query;

                            query.prepare( "delete from ports "
                                           "where port_num=(:p1) " );
                            query.bindValue( "p1", i );
                            query.exec();
                            if ( query.numRowsAffected() > 0 ) {
                                //� ��� �� ���������� �����
                                query.prepare( "delete from port_param_values "
                                               "where port_num=(:p1) " );
                                query.bindValue( "p1", i );
                                query.exec();

                                say( "delete port " + QString::number( i ) );
                                sysDBNeedDeleteSysPorts = true;

                                var_saved = true;

                            }                            

                        }

                    }

                }

            }

            //��� ��������� ���������� ������


                int i;
                for(i=0;i<256;i++) {

                    if ( this->SysPorts->port[ i ] != NULL ) {

                        if ( this->SysPorts->port[ i ]->stored == false ) {

                            int j;
                            QSqlQuery query;

                            this->SysPorts->port[ i ]->stored = true;

                            var_saved = true;

                            //���� � ��� �����
                            query.prepare( "select count(*) "
                                           "from ports "
                                           "where port_num=(:p1) " );

                            query.bindValue( "p1", i );

                            query.exec();
                            query.first();

                            if ( query.value(0).toString() != "0" ) {
                                //���� � ���� ����
                                //��������� ��������� �� ���
                                query.prepare( "select port_type_id "
                                               "from ports "
                                               "where port_num=(:p1) " );

                                query.bindValue( "p1", i );

                                query.exec();
                                query.first();

                                if ( query.value(0).toString() !=
                                     this->SysPorts->port[ i ]->portTypeID() ) {

                                    //������ ��� �����
                                    query.prepare( "update ports set port_type_id = (:p1) "
                                                   "where port_num=(:p2) " );

                                    query.bindValue( "p1", this->SysPorts->port[ i ]->portTypeID() );
                                    query.bindValue( "p2", i );

                                    query.exec();

                                    //� ��� �� ������� ���������
                                    query.prepare( "delete from port_param_values "
                                                   "where port_num=(:p1) " );
                                    query.bindValue( "p1", i );
                                    query.exec();

                                }
                            } else {
                                //����� � ���� ���
                                //���������
                                qDebug() << "!!!!!!!!!!!!insert into ports";

                                query.prepare( "insert into ports(port_num, port_type_id, port_note) "
                                               "values( (:p1), (:p2), (:p3) ) " );

                                query.bindValue( "p1", i );
                                query.bindValue( "p2", this->SysPorts->port[ i ]->portTypeID() );
                                query.bindValue( "p3", this->SysPorts->port[ i ]->portName() );

                                query.exec();

                            }

                            query.prepare( "update ports set port_note = (:p1) "
                                           "where port_num=(:p2) " );

                            query.bindValue( "p1", this->SysPorts->port[ i ]->portName() );
                            query.bindValue( "p2", i );
                            query.exec();


                            for(j=0;j<(this->SysPorts->port[ i ]->getParamCount());j++) {

                                //������� ���������� �����
                                query.prepare( "update port_param_values set par_value = (:p1) "
                                               "where par_code = (:p2) and port_num=(:p3) " );

                                query.bindValue( "p1", this->SysPorts->port[ i ]->getParamValue( j ) );
                                query.bindValue( "p2", j + 1 );
                                query.bindValue( "p3", i );
                                query.exec();


                                //say( "!!!rows affected " + QString::number( query.numRowsAffected() ) );

                                if ( query.numRowsAffected() != 0 ) {
                                    //say("set port_param_values UPDATE");
                                    //emit storeProgress( "Recreate port_param_values UPDATE" );
                                }

                                if ( query.numRowsAffected() == 0 ) {
                                    QSqlQuery query2;
                                    query2.prepare( "insert into port_param_values(port_num, par_code, par_value) "
                                                    "values((:p1),(:p2),(:p3)) " );
                                    query2.bindValue( "p1", i );
                                    query2.bindValue( "p2", j + 1 );
                                    query2.bindValue( "p3", this->SysPorts->port[ i ]->getParamValue( j ) );

                                    query2.exec();
                                    //say("set port_param_values INSERT");
                                    //emit storeProgress( "Recreate port_param_values INSERT" );

                                }

                            }

                        }

                    }

                }




            //��� �������� ������ �����
            if ( sysDBNeedDeleteSysBeacons == true ) {

                say("sysDBNeedDeleteSysBeacons == true");

                while ( sysDBNeedDeleteSysBeacons == true ) {

                    sysDBNeedDeleteSysBeacons = false;
                    int i;
                    for(i=0;i<256;i++) {

                        if ( this->SysBeacons->beacon[ i ] == NULL ) {

                            QSqlQuery query;

                            query.prepare( "delete from beacons "
                                           "where beacon_num=(:p1) " );
                            query.bindValue( "p1", i );
                            query.exec();

                            sysDBNeedDeleteSysBeacons = true;

                            var_saved = true;

                        }

                    }

                }

            }



                for(i=0;i<256;i++) {

                    if ( this->SysBeacons->beacon[ i ] != NULL ) {

                        //say( "this->SysBeacons->beacon[ "+QString::number(i)+" ] != NULL" );

                        if ( this->SysBeacons->beacon[ i ]->stored == false ) {

                            this->SysBeacons->beacon[ i ]->stored = true;

                            var_saved = true;

                            //���� � ��� �����
                            QSqlQuery query;

                            query.prepare( "select count(*) "
                                           "from beacons "
                                           "where beacon_num=(:p1) " );

                            query.bindValue( "p1", i );

                            query.exec();
                            query.first();

                            if ( query.value(0).toString() == "0" ) {
                                //����� � ���� ���
                                //���������

                                //                                         0          1      2    3     4    5      6        7        8        9        10
                                query.prepare( " insert into beacons( beacon_num, port_num, sym, call, lat, lng, bunproto, btext, binterval, stext, sinterval )"
                                               " values( (:p0), (:p1), (:p2), (:p3), (:p4), (:p5), (:p6), (:p7), (:p8), (:p9), (:p10) ) " );

                                query.bindValue( "p0", i );
                                query.bindValue( "p1", this->SysBeacons->beacon[ i ]->getParam( "PORT_NUM" ) );
                                query.bindValue( "p2", this->SysBeacons->beacon[ i ]->getParam( "SYM" ) );
                                query.bindValue( "p3", this->SysBeacons->beacon[ i ]->getParam( "CALL" ) );
                                query.bindValue( "p4", this->SysBeacons->beacon[ i ]->getParam( "LAT" ) );
                                query.bindValue( "p5", this->SysBeacons->beacon[ i ]->getParam( "LNG" ) );
                                query.bindValue( "p6", this->SysBeacons->beacon[ i ]->getParam( "UNPROTO" ) );
                                query.bindValue( "p7", this->SysBeacons->beacon[ i ]->getParam( "TEXT" ) );
                                query.bindValue( "p8", this->SysBeacons->beacon[ i ]->getParam( "INTERVAL" ) );
                                query.bindValue( "p9", this->SysBeacons->beacon[ i ]->getParam( "STATTEXT" ) );
                                query.bindValue( "p10", this->SysBeacons->beacon[ i ]->getParam( "STATINTERVAL" ) );

                                query.exec();

                            } else {

                                QSqlQuery query;

                                query.prepare( "update beacons set "
                                               "port_num = (:p1), "
                                               "sym = (:p2), "
                                               "call = (:p3), "
                                               "lat = (:p4), "
                                               "lng = (:p5), "
                                               "bUnproto = (:p6), "
                                               "bText = (:p7), "
                                               "bInterval = (:p8), "
                                               "sText = (:p9), "
                                               "sInterval = (:p10) "
                                               "where beacon_num=(:p11) " );


                                query.bindValue( "p1", this->SysBeacons->beacon[ i ]->getParam( "PORT_NUM" ) );
                                query.bindValue( "p2", this->SysBeacons->beacon[ i ]->getParam( "SYM" ) );
                                query.bindValue( "p3", this->SysBeacons->beacon[ i ]->getParam( "CALL" ) );
                                query.bindValue( "p4", this->SysBeacons->beacon[ i ]->getParam( "LAT" ) );
                                query.bindValue( "p5", this->SysBeacons->beacon[ i ]->getParam( "LNG" ) );
                                query.bindValue( "p6", this->SysBeacons->beacon[ i ]->getParam( "UNPROTO" ) );
                                query.bindValue( "p7", this->SysBeacons->beacon[ i ]->getParam( "TEXT" ) );
                                query.bindValue( "p8", this->SysBeacons->beacon[ i ]->getParam( "INTERVAL" ) );
                                query.bindValue( "p9", this->SysBeacons->beacon[ i ]->getParam( "STATTEXT" ) );
                                query.bindValue( "p10", this->SysBeacons->beacon[ i ]->getParam( "STATINTERVAL" ) );
                                query.bindValue( "p11", i );

                                query.exec();

                            }

                        }

                    }

                }



                QAPRSPacket *packetptr;

                packetptr = this->SysPackets->first;

                while ( packetptr!=NULL ) {

                    if ( packetptr->stored == false ) {

                        var_saved = true;

                        packetptr->stored = true;

                        QSqlQuery query;

                        //qDebug() << "QAtomix::run() save packet N=" << packetptr->K;
                        emit storeProgress( "Sysport save packet N=" + QString::number( packetptr->K ) );

                        query.prepare( "insert into packets( k, DT, port_num, trx, PTo, PFrom, PVia, Message ) "
                                       "values( (:p1),(:p2),(:p3),(:p4),(:p5),(:p6),(:p7),(:p8) ) " );

                        query.bindValue( "p1", packetptr->K );
                        query.bindValue( "p2", packetptr->DT );
                        query.bindValue( "p3", packetptr->Port_Num );
                        query.bindValue( "p4", packetptr->TRX );
                        query.bindValue( "p5", packetptr->PTo );
                        query.bindValue( "p6", packetptr->PFrom );
                        query.bindValue( "p7", packetptr->PVia );
                        query.bindValue( "p8", packetptr->Message );

                        query.exec();

                    }

                    packetptr = packetptr->next;

                }




        }


        say( "QAtomix::run() All saved ");
        emit storeProgress( "All saved" );
    }

    started = false;
    say( "QAtomix::run() end ");

}
