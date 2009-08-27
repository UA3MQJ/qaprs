#include "include/mainwindowimpl.h"
#include "include/portoption.h"

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f)
	: QMainWindow(parent, f)
{

    setupUi( this );
    qApp->setStyle( "plastique" );

    QApplication::setPalette( QApplication::style()->standardPalette() );

    //setWindowIcon( QIcon(":/images/ico.jpg") );

    loadOptionsFromFile();

    //qDebug() << tr("Start");

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

    connect( upPortButton,      SIGNAL( clicked() ),
             this,              SLOT( upPort() ) );

    connect( downPortButton,    SIGNAL( clicked() ),
             this,              SLOT( downPort() ) );

    connect( tabWidget,         SIGNAL( currentChanged( int ) ),
             this,              SLOT( changeTab( int ) ) );

    connect( AGWcheckBox,       SIGNAL( stateChanged ( int ) ),
             this,              SLOT( AGWOnOff( ) ) );


    APRSCore = new QAPRSCore(this);
    APRSCore->log = logEdit;
    APRSCore->db = db;

//    APRSCore->AGWEmulatorStart();

    portsModel.APRSCore = APRSCore;

    PacketsWindow.APRSCore = APRSCore;
    PacketsWindow.db = db;
    MessagesWindow.APRSCore = APRSCore;
    MessagesWindow.db = db;
    StationsWindow.APRSCore = APRSCore;
    StationsWindow.db = db;
    StationsWindow.stationsModel.APRSCore = APRSCore;

    connect(PacketsWindow.APRSCore,   SIGNAL( TRXPacket() ),
            &PacketsWindow,           SLOT( TRXPacket() ) );

    connect(MessagesWindow.APRSCore,   SIGNAL( TRXMessage() ),
            &MessagesWindow,           SLOT( UpdateMessageList() ) );

    connect( StationsWindow.APRSCore ,   SIGNAL( UpdateStationList() ),
             &StationsWindow,           SLOT( UpdateStationList() ) );

    connect( APRSCore,   SIGNAL( PortChangeState(int) ),
             this,       SLOT( UpdatePortsState(int) ) );



    createActions();
    createTrayIcon();


}

MainWindowImpl::~MainWindowImpl( ) {

    delete APRSCore;

}

void MainWindowImpl::closeEvent(QCloseEvent *event) {

    qDebug()<<"Close event";
    hide();
    event->ignore();

}

void MainWindowImpl::hideEvent ( QHideEvent * event ) {

   // hide();

}

void MainWindowImpl::createActions() {

    showGenOptionAction = new QAction(tr("&Options"), this);
    connect(showGenOptionAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    showPacketsAction = new QAction(tr("&Packets"), this);
    connect(showPacketsAction, SIGNAL(triggered()), this, SLOT(showPackets()));

    showMessagesAction = new QAction(tr("&Messages"), this);
    connect(showMessagesAction, SIGNAL(triggered()), this, SLOT(showMessages()));

    showStationsAction = new QAction(tr("&Stations"), this);
    connect(showStationsAction, SIGNAL(triggered()), this, SLOT(showStations()));

}

void MainWindowImpl::createTrayIcon() {

 //   trayIcon = new QSystemTrayIcon(this);
 //   trayIcon->setIcon( QIcon(":/images/ico.jpg") );
 //   trayIcon->show();

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showPacketsAction);
    trayIconMenu->addAction(showMessagesAction);
    trayIconMenu->addAction(showStationsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(showGenOptionAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon( QIcon(":/images/ico.png") );
    trayIcon->show();
}


void MainWindowImpl::newBaseButtonClick() {

    QApplication::setOverrideCursor(Qt::WaitCursor);

    APRSCore->ToLog( "QAPRSCore::Recreate system database<br>" );
    APRSCore->ToLog( "QAPRSCore::Create test ports<br>" );

    if (connectButton->isEnabled()) {
        DBName = baseNameEdit->text(); 
        db->setDatabaseName( DBName );
        db->open();
    }

    QSqlQuery query( *db );

    //port types table and data
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
                "values(-1,'n\\a', 'n\\a')" );

    //ports table
    query.exec( "drop table ports" );
    query.exec( "create table ports (port_num int primary key, "
                "port_type_id int, port_note varchar(50), port_call varchar(10), port_beacon_text varchar(20),"
                "port_beacon_interval int, port_unproto_address varchar(50),"
                "port_latitude varchar(20), port_longitude varchar(20), port_symbol varchar(3) ) " );

    //ports demo data
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

    //port parameters name table and data
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
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(7,0,'Proxy')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(8,0,'PAuth')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(9,0,'PName')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(10,0,'PPort')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(11,0,'PUser')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(12,0,'PPass')" );

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

    //port params table
    query.exec( "drop table port_param_values; " );
    query.exec( "create table port_param_values (port_num int , "
                "par_code int, par_value varchar(50) ); " );
    //ports param demo data
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

    //symbol table
    query.exec( "drop table symbols" );
    query.exec( "create table symbols (sym varchar(2), comment varchar(25) )" );
    //data

    query.exec( "insert into symbols ( sym, comment ) values( '\\!', 'Emergency' ); " );
    query.exec( "insert into symbols ( sym, comment ) values( '\\\"', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\#', 'No. Digi' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\$', 'Bank' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\%', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\&', 'No. Diam'd' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\\'', 'Crash site' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\(', 'Cloudy' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\)', 'MEO' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\*', 'Snow' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\+', 'Church' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\,', 'Girl Scout' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\-', 'Home (HF)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\.', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\/', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\0', 'No. Circle' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\1', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\2', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\3', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\4', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\5', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\6', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\7', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\8', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\9', 'Petrol Stn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\:', 'Hail' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\;', 'Park' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\<', 'Advisory' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\=', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\>', 'No. Car' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\?', 'Info Kiosk' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\@', 'Hurricane' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\A', 'No. Box' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\B', 'Snow blwng' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\C', 'Coast G'rd' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\D', 'Drizzle' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\E', 'Smoke' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\F', 'Fr'ze Rain' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\G', 'Snow Shwr' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\H', 'Haze' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\I', 'Rain Shwr' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\J', 'Lightning' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\K', 'Kenwood' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\L', 'Lighthouse' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\M', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\N', 'Nav Buoy' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\O', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\P', 'Parking' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\Q', 'Quake' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\R', 'Restaurant' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\S', 'Sat/Pacsat' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\T', 'T'storm' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\U', 'Sunny' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\V', 'VORTAC' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\W', 'No. WXS' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\X', 'Pharmacy' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\Y', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\Z', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\[', 'Wall Cloud' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\\\', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\]', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\^', 'No. Plane' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\_', 'No. WX Stn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\`', 'Rain' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\a', 'RSGB,ARRL' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\b', 'Dust blwng' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\c', 'No. CivDef' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\d', 'DX Spot' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\e', 'Sleet' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\f', 'Funnel Cld' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\g', 'Gale' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\h', 'HAM store' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\i', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\j', 'WorkZone' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\k', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\l', 'Area Locns' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\m', 'Milepost' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\n', 'No. Triang' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\o', 'Circle sm' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\p', 'Part Cloud' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\q', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\r', 'Restrooms' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\s', 'No. Boat' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\t', 'Tornado' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\u', 'No. Truck' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\v', 'No. Van' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\w', 'Flooding' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\x', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\y', 'Sky Warn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\z', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\{', 'Fog' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\|', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\}', 'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '\\~', 'No Symbol' );" );

    query.exec( "insert into symbols ( sym, comment ) values( '/!',	'Police Stn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/\"',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/#',	'Digi' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/$',	'Phone' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/%',	'DX Cluster' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/&',	'HF Gateway' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/\'',	'Plane sm' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/(',	'Cloudy' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/)',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/*',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/+',	'Red Cross' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/,',	'Boy Scout' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/-',	'Home' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/.',	'X' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '//',	'Dot' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/0',	'Circle (0)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/1',	'Circle (1)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/2',	'Circle (2)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/3',	'Circle (3)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/4',	'Circle (4)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/5',	'Circle (5)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/6',	'Circle (6)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/7',	'Circle (7)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/8',	'Circle (8)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/9',	'Circle (9)' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/:',	'Fire' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/;',	'Campground' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/<',	'Motorcycle' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/=',	'Rail Eng.' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/>',	'Car' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/?',	'File svr' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/@',	'HC Future' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/A',	'Aid Stn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/B',	'BBS' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/C',	'Canoe' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/D',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/E',	'Eyeball' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/F',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/G',	'Grid Squ.' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/H',	'Hotel' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/I',	'Tcp/ip' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/J',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/K',	'School' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/L',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/M',	'MacAPRS' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/N',	'NTS Stn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/O',	'Balloon' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/P',	'Police' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/Q',	'TBD' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/R',	'Rec Veh',le' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/S',	'Shuttle' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/T',	'SSTV' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/U',	'Bus' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/V',	'ATV' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/W',	'WX Service' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/X',	'Helo' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/Y',	'Yacht' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/Z',	'WinAPRS' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/[',	'Jogger' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/\',	'Triangle' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/]',	'PBBS' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/^',	'Plane lrge' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/_',	'WX Station' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/`',	'Dish Ant.' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/a',	'Ambulance' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/b',	'Bike' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/c',	'ICP' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/d',	'Garage' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/e',	'Horse' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/f',	'Fire Truck' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/g',	'Glider' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/h',	'Hospital' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/i',	'IOTA' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/j',	'Jeep' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/k',	'Truck' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/l',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/m',	'Repeater' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/n',	'Node' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/o',	'EOC' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/p',	'Rover' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/q',	'Grid squ.' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/r',	'Antenna' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/s',	'Power Boat' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/t',	'Truck Stop' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/u',	'Truck 18wh' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/v',	'Van' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/w',	'Water Stn' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/x',	'XAPRS' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/y',	'Yagi' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/z',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/{',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/|',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/}',	'No Symbol' );" );
    query.exec( "insert into symbols ( sym, comment ) values( '/~',	'No Symbol' );" );

    //packets table
    query.exec( "drop table packets" );
    query.exec( "create table packets (K integer primary key, "
                "DT datetime, port_num int, trx char(10), PTo char(20),"
                "PFrom char(20), PVia char(50), Message char(80) ) " );

    //messages table
    query.exec( "drop table messages" );
    query.exec( "create table messages (packet_K int, MTo varchar(10),Message varchar(250), Mess_ID varchar(6) )" );
    //demo data
    //query.exec( "insert into  messages (packet_K, MTo, Message, Mess_ID ) values(0, 'UA3MM', 'Test message', '001') " );

    //station list table
    query.exec( "drop table stations" );
    //query.exec( "create table stations (call varchar(10), sym varchar(2), lat varchar(9), lng varchar(10), grid varchar(7), Via varchar(50), length float, deg float, LH DateTime, StComment varchar(100) )" );
    //length and deg for station no need for save. this parameters must be calculated
    query.exec( "create table stations (call varchar(10), sym varchar(2), lat varchar(9), lng varchar(10), grid varchar(7), Via varchar(50), LH DateTime, StComment varchar(100) )" );

    //demo data
    //query.exec( "insert into stations (call, sym, lat, lng, grid, length, deg, LH, Via ) "
    //            "values('UA3MAD', '/-', '58.01.84N', '038.51.13E', 'KO98KA', 0, 0, '2009-01-01 08:00:00', 'APU25N,RR3MD')" );


    this->disconnectButtonClick();

    QApplication::restoreOverrideCursor();

    QMessageBox msgBox;
    msgBox.setText("The system database created.");
    msgBox.exec();

}

void MainWindowImpl::requeryPorts() {

    int saveIndex;

    saveIndex = portsTableView->currentIndex().row();

    if ( this->tabWidget->currentIndex() == 1 ) {
        //!!!
        isrequeringPorts = TRUE;

        portsTableView->setModel( NULL );
        portsModel.setQuery( portsQuery );
        //portsModel.setHeaderData(5, Qt::Horizontal, "ColName" );
        portsTableView->setModel( &portsModel );

        if ( saveIndex == -1 ) saveIndex = 0;
        portsTableView->selectRow( saveIndex );

        portsTableView->setColumnWidth(  0,  25 );
        portsTableView->setColumnWidth(  1,  40 );
        portsTableView->setColumnWidth(  2,  70 );
        portsTableView->setColumnWidth(  3, 100 );
        portsTableView->setColumnWidth(  4,  60 );
        portsTableView->setColumnWidth(  8,  60 );
        portsTableView->setColumnWidth(  9,  67 );
        portsTableView->setColumnWidth( 10,  40 );
        portsTableView->setColumnWidth( 11,  40 );

        portsTableView->hideColumn( 1 ); //hide PTID

        isrequeringPorts = FALSE;

    }

}


void MainWindowImpl::connectButtonClick() {

    APRSCore->ToLog( "QAPRSCore::Connecting to system database<br>" );

    APRSCore->Lat = latEdit->text();
    APRSCore->Lng = lngEdit->text();

    APRSCore->AGWEmulatorPort = AGWPort->text().toInt( );

    disconnectButton ->setEnabled( TRUE );
    connectButton    ->setEnabled( FALSE );
    deletePortButton ->setEnabled( TRUE );
    addPortButton    ->setEnabled( TRUE );
    editPortButton   ->setEnabled( TRUE );
    addPortButton    ->setEnabled( TRUE );
    editPortButton   ->setEnabled( TRUE );
    upPortsButton    ->setEnabled( TRUE );
    downPortsButton  ->setEnabled( TRUE );
    upPortButton     ->setEnabled( TRUE );
    downPortButton   ->setEnabled( TRUE );

    DBName = baseNameEdit->text();
    db->setDatabaseName( DBName );
    db->open();

    APRSCore->coreActive = TRUE;


    PacketsWindow.requeryPackets();
    MessagesWindow.requeryMessages();
    StationsWindow.requeryStations();

    portsQuery = "select ports.port_num as PN, ports.port_type_id as PTID, port_types.port_type_note as PType, "
                 "ports.port_note as PNote, ports.port_call as PCall, "
                 "ports.port_beacon_text as 'Bacon Text', ports.port_beacon_interval as 'Beacon Interval', "
                 "ports.port_unproto_address as 'Unproto Address', "
                 "ports.port_latitude as Lat, ports.port_longitude as Lng, ports.port_symbol as PSym, NULL as Act "
                 "from ports left join port_types on "
                 "ports.port_type_id=port_types.port_type_id "
                 "order by ports.port_num";

    requeryPorts();

    this->upAllPorts();
    if ( AGWcheckBox->isChecked() ) APRSCore->AGWEmulatorStart();

}

void MainWindowImpl::upAllPorts() {

    //up ports
    APRSCore->ToLog( "QAPRSCore::Try to up of " + QString::number( portsModel.rowCount() ) + " port(s)<br>" );
    APRSCore->createPorts();
    //upPortsButton    ->setEnabled( FALSE );
    //downPortsButton  ->setEnabled( TRUE );
    requeryPorts();

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
    upPortButton     ->setEnabled( FALSE );
    downPortButton   ->setEnabled( FALSE );
    db->close();


    portsTableView->setModel( NULL );

    //down all ports
    logEdit->insertHtml( "QAPRSCore::All port(s) is down<br>" );
    if (APRSCore->AGWEmulatorActive == TRUE)
        APRSCore->AGWEmulatorStop();
    APRSCore->closePorts();

    APRSCore->coreActive = FALSE;

}

void MainWindowImpl::downAllPorts() {

    //down ports
    APRSCore->ToLog( "QAPRSCore::All port(s) is down<br>" );
    APRSCore->closePorts();
    //upPortsButton    ->setEnabled( TRUE );
    //downPortsButton  ->setEnabled( FALSE );
    requeryPorts();

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

    port_num = portsModel.record( portsTableView->currentIndex().row() ).value( "PN" ).toInt();

    query.exec( "delete from ports where port_num = " + QString::number( port_num ) + " " );
    query.exec( "delete from port_param_values where port_num = " + QString::number( port_num ) + " " );

    requeryPorts();

    APRSCore->deletePort( port_num );

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
    portsTableView->selectRow( portsModel.rowCount()-1 );
    editPortButtonClick();

}

void MainWindowImpl::loadOptionsFromFile() {
//!!!!!!!!!!!
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

    AGWEN = in.readLine();
    AGWEN = AGWEN.right( AGWEN.length() - 6 ); // length("AGWEn=")=6

    AGWP = in.readLine();
    AGWP = AGWP.right( AGWP.length() - 8 ); // length("AGWPort=")=8

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
    AGWcheckBox  ->setChecked( AGWEN=="T" );
    AGWPort->setValue( AGWP.toInt( ) );

    dbTypeComboBox->setCurrentIndex( DBType.toInt() );
    
    disconnectButton->setEnabled( FALSE );
    userEdit->setEnabled( DBType.toInt()!=0 );
    passEdit->setEnabled( DBType.toInt()!=0 );
    hostEdit->setEnabled( DBType.toInt()!=0 );
    portBox ->setEnabled( DBType.toInt()!=0 );

}

void MainWindowImpl::saveOptionsToFile() {
//!!!!!!!!!!!!!!!!!
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

    if ( AGWcheckBox->isChecked() ) {
        out << "AGWEn=T" << endl;
    } else {
        out << "AGWEn=F" << endl;
    }

    out << "AGWPort="   << ( AGWPort->text() ) << endl;

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

    connect( &win,     SIGNAL( savePort( int ) ),
             this,    SLOT( savePortButtonClick( int ) ) );

    Index = portsModel.record( portsTableView->currentIndex().row() ).value( "PTID" ).toInt();

    win.portTypeBox         ->setCurrentIndex ( Index+1 );
    win.portNumEdit         ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "PN" ).toString() );
    win.portNameEdit        ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "PNote" ).toString() );
    win.portCallEdit        ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "PCall" ).toString() );
    win.portLatEdit         ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "Lat" ).toString() );
    win.portLngEdit         ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "Lng" ).toString() );
    win.portUnprotoEdit     ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "Unproto Address" ).toString() );
    win.portBeaconTextEdit  ->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "Bacon Text" ).toString() );
    win.portBeaconIntervalEdit->setText ( portsModel.record( portsTableView->currentIndex().row()).value( "Beacon Interval" ).toString() );


    QByteArray Sym;
    int symindex;
    Sym.clear();
    Sym.append( portsModel.record( portsTableView->currentIndex().row()).value( "PSym" ).toString() );


    if (Index==-1) {
        win.portCallEdit        ->setText ( callEdit->text() );
        win.portLatEdit         ->setText ( latEdit->text() );
        win.portLngEdit         ->setText ( lngEdit->text() );
        win.portUnprotoEdit     ->setText ( unprotoEdit->text() );
        Sym.append( symbolEdit->text() );
        win.portBeaconTextEdit  ->setText ( beacontextEdit->text() );
        win.portBeaconIntervalEdit->setText( "0" );
    }


    symindex = (uchar( Sym.data()[1] )-33) << 1;
    if (Sym.data()[0]=='\\') symindex+=1;
    win.portSymbolEdit->setCurrentIndex( symindex );


    query.exec( "select * from port_param_values "
                "where port_num=" + portsModel.record( portsTableView->currentIndex().row() ).value( "PN" ).toString() );

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
              case 7 :
                        win.proxyCheckBox->setChecked( query.value( 2 ).toString()=="T" );
                        break;
              case 8 :
                        win.proxyAuthCheckBox->setChecked( query.value( 2 ).toString()=="T" );
                        break;
              case 9 :
                        win.proxyNameEdit    ->setText( query.value( 2 ).toString() );
                        break;
              case 10 :
                        win.proxyPortBox     ->setValue( query.value( 2 ).toInt() );
                        break;
              case 11 :
                        win.proxyUserEdit    ->setText( query.value( 2 ).toString() );
                        break;
              case 12 :
                        win.proxyPassEdit    ->setText( query.value( 2 ).toString() );
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

void MainWindowImpl::upPort() {

    APRSCore->updatePort( portsModel.record( portsTableView->currentIndex().row() ).value( "PN" ).toInt() );
    requeryPorts();

}


void MainWindowImpl::downPort() {

    APRSCore->deletePort( portsModel.record( portsTableView->currentIndex().row() ).value( "PN" ).toInt() );
    requeryPorts();

}

void MainWindowImpl::savePortButtonClick( int pnum ) {

    //qDebug()<<"Port save "<<pnum;
    if ( APRSCore->Port[ pnum ] != NULL )
        APRSCore->updatePort( pnum );

}

void MainWindowImpl::UpdatePortsState( int pnum ) {

    if (isrequeringPorts==FALSE) requeryPorts();

}

void MainWindowImpl::changeTab( int index ) {

    this->requeryPorts();

}

void MainWindowImpl::AGWOnOff( ) {

    if ( !connectButton->isChecked() ) {
        if ( AGWcheckBox->isChecked() ) {
            if ( APRSCore->AGWEmulatorActive == FALSE )
                APRSCore->AGWEmulatorPort = AGWPort->text().toInt( );
                APRSCore->AGWEmulatorStart();
        } else {
            if ( APRSCore->AGWEmulatorActive == TRUE )
                APRSCore->AGWEmulatorStop();
        }
    }

}


void MainWindowImpl::showPackets() {

    qDebug() << "Show Packets";

    PacketsWindow.show();

}

void MainWindowImpl::showMessages() {

    qDebug() << "Show Messages";

    MessagesWindow.show();

}

void MainWindowImpl::showStations() {

    qDebug() << "Show Stations";

    StationsWindow.show();

}






