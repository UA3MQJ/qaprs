#include "include/mainwindowimpl.h"
#include "include/portoption.h"


//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f)
	: QMainWindow(parent, f)
{
    setupUi( this );
    qApp->setStyle( "plastique" );
    QApplication::setPalette( QApplication::style()->standardPalette() );

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon( QIcon(":/images/ico.jpg") );
    trayIcon->show();
    setWindowIcon( QIcon(":/images/ico.jpg") );

    loadOptionsFromFile();
    //sig

    //newBaseButton
    connect( newBaseButton,     SIGNAL( clicked() ),
             this,              SLOT( newBaseButtonClick() ) );

    connect( cancelButton,      SIGNAL( clicked() ),
             this,              SLOT( cancelButtonClick() ) );

    connect( saveButton,        SIGNAL( clicked() ),
             this,              SLOT( saveButtonClick() ) );

    connect( connectButton,     SIGNAL( clicked() ),
             this,              SLOT( connectButtonClick() ) );

    connect( disconnectButton,  SIGNAL( clicked() ),
             this,              SLOT( disconnectButtonClick() ) );

    connect( dbTypeComboBox,    SIGNAL( currentIndexChanged( int ) ),
             this,              SLOT( currentIndexChanged ( int ) ) );

    connect( addPortButton,     SIGNAL( clicked() ),
             this,              SLOT( addPortButtonClick() ) );

    connect( deletePortButton,  SIGNAL( clicked() ),
             this,              SLOT( deletePortButtonClick() ) );

    connect( editPortButton,    SIGNAL( clicked() ),
             this,              SLOT( editPortButtonClick() ) );

    connect( portsTableView,    SIGNAL( doubleClicked ( const QModelIndex & ) ),
             this,              SLOT( editPortButtonClick() ) );

    connect( upPortsButton,     SIGNAL( clicked() ),
             this,              SLOT( upAllPorts() ) );

    connect( downPortsButton,   SIGNAL( clicked() ),
             this,              SLOT( downAllPorts() ) );


    APRSCore = new QAPRSCore(this);
    APRSCore->log = logEdit;
    APRSCore->db = db;

    connect( APRSCore,   SIGNAL( TRXPacket() ),
             this,              SLOT( TRXPacket() ) );


}


void MainWindowImpl::newBaseButtonClick() {

    //logEdit->insertHtml( "QAPRSCore::Recreate system database<br>" );
    //logEdit->insertHtml( "QAPRSCore::Create test ports<br>" );
    APRSCore->ToLog( "QAPRSCore::Recreate system database<br>" );
    APRSCore->ToLog( "QAPRSCore::Create test ports<br>" );

    if (connectButton->isEnabled()) {
        DBName = baseNameEdit->text(); 
        db->setDatabaseName( DBName );
        db->open();
    }

    QSqlQuery query( *db );

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
                "values(-1,'n/a', 'N/A')" );

    query.exec( "drop table ports" );
    query.exec( "create table ports (port_num int primary key, "
                "port_type_id int, port_note varchar(50), port_call varchar(10), port_beacon_text varchar(20),"
                "port_beacon_interval int, port_unproto_address varchar(50),"
                "port_latitude varchar(20), port_longitude varchar(20), port_symbol varchar(3) ) " );
/*
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(0,0,'APRS.NET', 'UA3MAD', 'ua3mad@mail.ru', 3000, '', '57.01.83N', '038.51.13E', '/I') " );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(1,1,'KISS (MIX COM3<->COM7)', 'UA3MAD', 'QRM', 3000, 'WIDE1-1,WIDE2-2', '57.01.83N', '038.51.13E', '\\-') " );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(2,2,'AGW PORT1 - 145 MHz', 'UA3MAD', 'ping', 3000, 'WIDE1-1,WIDE2-2', '57.01.83N', '038.51.13E', '/-')" );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(3,2,'AGW PORT2 - 430 MHz', 'UA3MAD', 'ping', 3000, 'WIDE1-1,WIDE2-2', '57.01.83N', '038.51.13E', '/-')" );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(4,5,'AXIP 1200 MHz host', 'UA3MAD', 'ua3mad@mail.ru', 5000, 'RK3MWI-3', '57.01.83N', '038.51.13E', '/I' )" );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(5,5,'AXIP RK3MWI host', 'UA3MAD', 'ua3mad@mail.ru', 3000, 'RK3MWI-2', '57.01.83N', '038.51.13E', '/I')" );
*/
    query.exec( "drop table port_param_names; " );
    query.exec( "create table port_param_names (par_code int, port_type_id int, par_name varchar(20) )" );
    //AXIP param names
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,5,'RXPort')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,5,'IPAddress')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,5,'TXPort')" );
    //APRS Internet Server
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,0,'Server1')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,0,'Server2')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,0,'Server3')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(4,0,'User')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(5,0,'Pass')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(6,0,'Filter')" );
    //AGW param names
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,2,'Host')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,2,'AGWPort')" );
    //KISS param names
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,1,'COM')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,1,'Speed')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,1,'Command1')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(4,1,'Command2')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(5,1,'Command3')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(6,1,'Command4')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(7,1,'TXDelay')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(8,1,'TXTail')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(9,1,'SlotTime')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(10,1,'Persist')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(11,1,'FullDuplex')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(12,1,'ExitFromKISS')" );



    query.exec( "drop table port_param_values; " );
    query.exec( "create table port_param_values (port_num int , "
                "par_code int, par_value varchar(50) ); " );
    /*
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,1,'127.0.0.1:14000') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,2,'www.aprs.org:14580') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,3,'www.aprs.qrz.ru:14580') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,4,'UA3MQJ') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,5,'-1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,6,'filter R/UA') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(1,1,'1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(1,2,'2400') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(2,1,'127.0.0.1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(2,2,'1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(3,1,'localhost') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(3,2,'2') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(4,1,'65000') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(4,2,'127.0.0.1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(4,3,'65001') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(5,1,'60000') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(5,2,'192.168.0.103') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(5,3,'60000') " );
*/
    query.exec( "drop table packets" );
    query.exec( "create table packets (K int, "
                "DT datetime, port_num int, trx varchar(10), PTo varchar(20),"
                "PFrom varchar(20), PVia varchar(50), Message varchar(250) ) " );


    query.exec( "drop table messages" );
    query.exec( "create table messages (packet_K int, MTo varchar(10),Message varchar(250), Mess_ID varchar(6) )" );
    //query.exec( "insert into  messages (packet_K, MTo, Message, Mess_ID ) values(0, 'UA3MM', 'Test message', '001') " );

    if ( connectButton->isEnabled() ) db->close();

    plainModel.setQuery( portsQuery );

}

void MainWindowImpl::requeryPorts() {

    plainModel.setQuery( portsQuery );
    portsTableView->setModel( &plainModel );

    portsTableView->selectRow( 0 );

    portsTableView->setColumnWidth(  0,  25 );
    portsTableView->setColumnWidth(  1,  40 );
    portsTableView->setColumnWidth(  2, 100 );
    portsTableView->setColumnWidth(  3, 100 );
    portsTableView->setColumnWidth(  4,  60 );
    portsTableView->setColumnWidth(  8,  60 );
    portsTableView->setColumnWidth(  9,  65 );
    portsTableView->setColumnWidth( 10,  40 );

}

void MainWindowImpl::requeryPackets() {

    packetsModel.setQuery( packetsQuery );
    packetTableView->setModel( &packetsModel );

    packetTableView->selectRow( 0 );

    packetTableView->setColumnWidth(  0,  50 );
    packetTableView->setColumnWidth(  1,  70 );
    packetTableView->setColumnWidth(  2,  55 );
    packetTableView->setColumnWidth(  3,  50 );
    packetTableView->setColumnWidth(  4,  60 );
    packetTableView->setColumnWidth(  5,  30 );
    packetTableView->setColumnWidth(  6,  70 );
    packetTableView->setColumnWidth(  7,  70 );
    packetTableView->setColumnWidth(  8,  170 );
    packetTableView->setColumnWidth(  9,  300 );

}

void MainWindowImpl::requeryMessages() {

    messagesModel.setQuery( messagesQuery );
    messageTableView->setModel( &messagesModel );
    
    messageTableView->selectRow( 0 );
    
    messageTableView->setColumnWidth(  0,  50 );
    messageTableView->setColumnWidth(  1,  70 );
    messageTableView->setColumnWidth(  2,  55 );
    messageTableView->setColumnWidth(  3,  50 );
    messageTableView->setColumnWidth(  4,  30 );
    messageTableView->setColumnWidth(  5,  70 );
    messageTableView->setColumnWidth(  6,  70 );
    messageTableView->setColumnWidth(  7, 230 );
    messageTableView->setColumnWidth(  8,  50 );

}

void MainWindowImpl::connectButtonClick() {

    APRSCore->ToLog( "QAPRSCore::Connecting to system database<br>" );

    disconnectButton ->setEnabled( TRUE );
    connectButton    ->setEnabled( FALSE );
    deletePortButton ->setEnabled( TRUE );
    addPortButton    ->setEnabled( TRUE );
    editPortButton   ->setEnabled( TRUE );
    upPortsButton    ->setEnabled( TRUE );
    downPortsButton  ->setEnabled( FALSE );

    DBName = baseNameEdit->text();
    db->setDatabaseName( DBName );
    db->open();


    portsQuery = "select ports.port_num as PN, ports.port_type_id as PTID, port_types.port_type_note as PType, "
               "ports.port_note as PNote, ports.port_call as PCall, "
               "ports.port_beacon_text as 'Bacon Text', ports.port_beacon_interval as 'Beacon Interval', "
               "ports.port_unproto_address as 'Unproto Address', "
               "ports.port_latitude as Lat, ports.port_longitude as Lng, ports.port_symbol as PSym "
               "from ports left join port_types on "
               "ports.port_type_id=port_types.port_type_id "
               "order by ports.port_num";


    portsTableView->setModel( &plainModel );
    requeryPorts();

    packetsQuery = "select p1.K, substr(p1.DT,9,2)||\".\"||substr(p1.DT,6,2)||\".\"||substr(p1.DT,1,4) as DATA, substr(p1.DT,12,8) as TIME, "
                   "\"(\"||p1.port_num||\") \"||(p3.port_type_not) as Port, p2.port_note as PNote, p1.TRX as TRX, p1.PTo, p1.PFrom, p1.PVia, p1.Message "
                   "from packets p1 left join ports p2 on p1.port_num = p2.port_num "
                   "left join port_types p3 on p3.port_type_id = p2.port_type_id "
                   "order by K desc";

    packetTableView->setModel( &packetsModel );
    requeryPackets();

    messagesQuery = "select p1.packet_K as K, substr(p2.DT,9,2)||\".\"||substr(p2.DT,6,2)||\".\"||substr(p2.DT,1,4) as DATA, substr(p2.DT,12,8) as TIME, "
                    "\"(\"||p2.port_num||\") \"||(p4.port_type_not) as Port, p2.TRX as TRX, p2.PFrom, p1.MTo, p1.Message, p1.Mess_ID as MSGID "
                    "from messages p1 left join packets p2 on p1.packet_K=p2.K "
                    "left join ports p3 on p3.port_num=p2.port_num "
                    "left join port_types p4 on p4.port_type_id = p3.port_type_id "
                    "order by p1.packet_K desc ";

    messageTableView->setModel( &messagesModel );
    requeryMessages();

}

void MainWindowImpl::disconnectButtonClick() {

    APRSCore->ToLog( "QAPRSCore::Disconnectiong from system database<br>" );
    disconnectButton ->setEnabled( FALSE );
    connectButton    ->setEnabled( TRUE );
    deletePortButton ->setEnabled( FALSE );
    addPortButton    ->setEnabled( FALSE );
    editPortButton   ->setEnabled( FALSE );
    upPortsButton    ->setEnabled( FALSE );
    downPortsButton  ->setEnabled( FALSE );
    db->close();
    plainModel.setQuery( portsQuery ); //это для того, чтобы обнулить грид, но вылезет ошибка - не подсоединены к БД
    //down ports
    logEdit->insertHtml( "QAPRSCore::All port(s) is down<br>" );
    APRSCore->closeAllPorts();
}

void MainWindowImpl::upAllPorts() {
    //up ports
    APRSCore->ToLog( "QAPRSCore:: Try to up of " + QString::number( plainModel.rowCount() ) + " port(s)<br>" );
    APRSCore->createPorts();
    upPortsButton    ->setEnabled( FALSE );
    downPortsButton  ->setEnabled( TRUE );
}

void MainWindowImpl::downAllPorts() {
    //down ports
    APRSCore->ToLog( "QAPRSCore::All port(s) is down<br>" );
    APRSCore->closeAllPorts();
    upPortsButton    ->setEnabled( TRUE );
    downPortsButton  ->setEnabled( FALSE );

}


void MainWindowImpl::saveButtonClick() {

    saveOptionsToFile();

}

void MainWindowImpl::cancelButtonClick() {

    loadOptionsFromFile();

}

void MainWindowImpl::deletePortButtonClick() {

    QSqlQuery query( *db );

    int port_num;	

    port_num = plainModel.record( portsTableView->currentIndex().row() ).value( "PN" ).toInt();

    query.exec( "delete from ports where port_num = " + QString::number( port_num ) + " " );
    query.exec( "delete from port_param_values where port_num = " + QString::number( port_num ) + " " );

    requeryPorts();
}

void MainWindowImpl::addPortButtonClick() {

    QSqlQuery query( *db );

    int next_n;	

    query.prepare( "select max(port_num)+1 from ports" );
    query.exec();
    query.first();
    next_n = query.value( 0 ).toInt();

    query.exec( "insert into ports (port_num, port_type_id, port_note) "
                "values(" + QString::number( next_n ) + ",-1,'n\\a')" );

    requeryPorts();
    requeryPorts();
    portsTableView->selectRow( plainModel.rowCount()-1 );
    editPortButtonClick();

}

void MainWindowImpl::loadOptionsFromFile() {

    QFile file( "./config.ini" );

    file.open( QIODevice::ReadOnly );
    QTextStream in( &file );
    
    DBType = in.readLine();
    DBType = DBType.right( DBType.length() - 7 ); // length("DBType=")=7

    DBName = in.readLine();
    DBName = DBName.right( DBName.length() - 7 ); // length("DBName=")=7

    UName  = in.readLine();
    UName  = UName.right( UName.length() - 6 ); // length("UName=")=6

    UPass  = in.readLine();
    UPass  = UPass.right( UPass.length() - 6 ); // length("UPass=")=6

    Host   = in.readLine();
    Host   = Host.right( Host.length() - 5 ); // length("Host=")=5
    
    Port   = in.readLine();
    Port   = Port.right( Port.length() - 5 ); // length("Port=")=5

    Call    = in.readLine();
    Call    = Call.right( Call.length() - 5 ); // length("Call=")=5

    Name    = in.readLine();
    Name    = Name.right( Name.length() - 5 ); // length("Name=")=5

    QTHN    = in.readLine();
    QTHN    = QTHN.right( QTHN.length() - 5 ); // length("QTHN=")=5

    Lat     = in.readLine();
    Lat     = Lat.right( Lat.length() - 4 ); // length("Lat=")=4

    Lng     = in.readLine();
    Lng     = Lng.right( Lng.length() - 4 ); // length("Lng=")=4

    Unproto = in.readLine();
    Unproto = Unproto.right( Unproto.length() - 8 ); // length("Unproto=")=8

    Beacon = in.readLine();
    Beacon = Beacon.right( Beacon.length() - 7 ); // length("Beacon=")=7

    Symbol = in.readLine();
    Symbol = Symbol.right( Symbol.length() - 7 ); // length("Symbol=")=7


    file.close();

    baseNameEdit ->setText( DBName );
    userEdit     ->setText( UName );
    passEdit     ->setText( UPass );
    hostEdit     ->setText( Host );
    portBox      ->setValue( Port.toInt() );
    callEdit     ->setText( Call );
    nameEdit     ->setText( Name );
    qthnameEdit  ->setText( QTHN );
    latEdit      ->setText( Lat );
    lngEdit      ->setText( Lng );
    unprotoEdit  ->setText( Unproto );
    beacontextEdit->setText( Beacon );
    symbolEdit   ->setText( Symbol );

    dbTypeComboBox->setCurrentIndex( DBType.toInt() );
    
    disconnectButton->setEnabled( FALSE );
    userEdit->setEnabled( DBType.toInt()!=0 );
    passEdit->setEnabled( DBType.toInt()!=0 );
    hostEdit->setEnabled( DBType.toInt()!=0 );
    portBox ->setEnabled( DBType.toInt()!=0 );

}

void MainWindowImpl::saveOptionsToFile() {

    QFile file( "./config.ini" );

    file.open( QIODevice::WriteOnly );
    QTextStream out( &file );
    
    out << "DBType=" << ( QString::number( dbTypeComboBox->currentIndex() ) ) << endl;

    out << "DBName=" << ( baseNameEdit->text() ) << endl;

    out << "UName="  << ( userEdit->text() ) << endl;

    out << "UPass="  << ( passEdit->text() ) << endl;

    out << "Host="   << ( hostEdit->text() ) << endl;
    
    out << "Port="   << ( QString::number( portBox->value() ) ) << endl;

    out << "Call="   << ( callEdit->text() ) << endl;

    out << "Name="   << ( nameEdit->text() ) << endl;

    out << "QTHN="   << ( qthnameEdit->text() ) << endl;

    out << "Lat="    << ( latEdit->text() ) << endl;

    out << "Lng="    << ( lngEdit->text() ) << endl;

    out << "Unproto="<< ( unprotoEdit->text() ) << endl;

    out << "Beacon="   << ( beacontextEdit->text() ) << endl;

    out << "Symbol="   << ( symbolEdit->text() ) << endl;


    file.close();
}

void MainWindowImpl::currentIndexChanged ( int index ) {

    DBType = QString::number( index );

    //if dbtype=SQLITE then disable edits user, pass, host, port
      userEdit->setEnabled( DBType.toInt()!=0 );
      passEdit->setEnabled( DBType.toInt()!=0 );
      hostEdit->setEnabled( DBType.toInt()!=0 );
      portBox ->setEnabled( DBType.toInt()!=0 );
 
}


void MainWindowImpl::editPortButtonClick() {

    PortoptionDialog win;
    int Index;
    QSqlQuery query( *db );

    Index = plainModel.record( portsTableView->currentIndex().row() ).value( "PTID" ).toInt();

    win.portTypeBox         ->setCurrentIndex ( Index+1 );
    win.portNumEdit         ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "PN" ).toString() );
    win.portNameEdit        ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "PNote" ).toString() );
    win.portCallEdit        ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "PCall" ).toString() );
    win.portLatEdit         ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "Lat" ).toString() );
    win.portLngEdit         ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "Lng" ).toString() );
    win.portUnprotoEdit     ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "Unproto Address" ).toString() );
    win.portBeaconTextEdit  ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "Bacon Text" ).toString() );
    win.portBeaconIntervalEdit->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "Beacon Interval" ).toString() );

    //currentText
    //win.portSymbolEdit      ->setText ( plainModel.record( portsTableView->currentIndex().row()).value( "PSym" ).toString() );

        QByteArray Sym;
        int symindex;
        Sym.clear();
        Sym.append( plainModel.record( portsTableView->currentIndex().row()).value( "PSym" ).toString() );
        symindex = (uchar( Sym.data()[1] )-33) << 1;
        if (Sym.data()[0]=='\\') symindex+=1;
        win.portSymbolEdit->setCurrentIndex( symindex );




    if (Index==-1) {
        win.portCallEdit        ->setText ( callEdit->text() );
        win.portLatEdit         ->setText ( latEdit->text() );
        win.portLngEdit         ->setText ( lngEdit->text() );
        win.portUnprotoEdit     ->setText ( unprotoEdit->text() );
        win.portSymbolEdit      ->setCurrentIndex( 25 ); //Home (HF)
        win.portBeaconTextEdit  ->setText ( beacontextEdit->text() );
        win.portBeaconIntervalEdit->setText( "0" );
    }

    query.exec( "select * from port_param_values "
                "where port_num=" + plainModel.record( portsTableView->currentIndex().row() ).value( "PN" ).toString() );

    while ( query.next() ) {
            if ( Index==0 ) { //APRS INTERNET SERVER
              switch ( query.value( 1 ).toInt() ) {
              case 1 :
                        win.hostNameEdit ->setText( query.value( 2 ).toString() );
			break;
              case 2 :
                        win.hostName2Edit->setText( query.value( 2 ).toString() );
			break;
              case 3 :
                        win.hostName3Edit->setText( query.value( 2 ).toString() );
			break;
              case 4 :
                        win.userEdit     ->setText( query.value( 2 ).toString() );
			break;
              case 5 :
                        win.passEdit     ->setText( query.value( 2 ).toString() );
			break;
              case 6 :
                        win.APRSFilter   ->setText( query.value( 2 ).toString() );
			break;
              };
	    };

            if ( Index==1 ) { //KISS
                  switch ( query.value( 1 ).toInt() ) {
              case 1 :
                        //win.COMPortNumBox  ->setCurrentIndex( win.COMPortNumBox  ->findText( query.value( 2 ).toString() ) );
                        win.COMPortNumBox->setEditText( query.value( 2 ).toString() );
			break;
              case 2 :
                        win.COMPortSpeedBox->setCurrentIndex( win.COMPortSpeedBox->findText( query.value( 2 ).toString() ) );
                        break;
              case 3 :
                        win.KISSComm1Edit->setText( query.value( 2 ).toString() );
                        break;
              case 4 :
                        win.KISSComm2Edit->setText( query.value( 2 ).toString() );
                        break;
              case 5 :
                        win.KISSComm3Edit->setText( query.value( 2 ).toString() );
                        break;
              case 6 :
                        win.KISSComm4Edit->setText( query.value( 2 ).toString() );
                        break;
              case 7 :
                        win.TXDelayEdit->setText( query.value( 2 ).toString() );
                        break;
              case 8 :
                        win.TXTailEdit->setText( query.value( 2 ).toString() );
                        break;
              case 9 :
                        win.SlotTimeEdit->setText( query.value( 2 ).toString() );
                        break;
              case 10 :
                        win.PersistEdit->setText( query.value( 2 ).toString() );
                        break;
              case 11 :
                        win.FullDuplexCheckBox->setChecked( query.value( 2 ).toString()=="T" );
                        break;
              case 12 :
                        win.KISSCommEXITEdit->setText( query.value( 2 ).toString() );
                        break;
                    };
            };

            if ( Index==2 ) { //AGW
                  switch ( query.value( 1 ).toInt() ) {
              case 1 :
                        win.AGWHostEdit->setText( query.value( 2 ).toString() );
			break;
              case 2 :
                        win.AGWPortBox ->setValue( query.value( 2 ).toInt() );
			break;
		  };
	    };

            if ( Index==5 ) { //AXIP
                  switch ( query.value( 1 ).toInt() ) {
              case 1 :
                        win.AXIPRXPortBox ->setValue( query.value( 2 ).toInt() );
			break;
              case 2 :
                        win.AXIPTXHostEdit->setText( query.value( 2 ).toString() );
			break;
              case 3 :
                        win.AXIPTXPortBox ->setValue( query.value( 2 ).toInt() );
			break;
		  };
	    };

    };

    Index = portsTableView->currentIndex().row();
    win.exec(); 
    requeryPorts();
    portsTableView->selectRow( Index );
}

void MainWindowImpl::TRXPacket() {

    if (!(fromzePacketList->isChecked())) requeryPackets();

    messagesModel.setQuery( messagesQuery );
    messageTableView->selectRow( 0 );

}
