#include "./include/mainwindow.h"
#include "include/storageprogress.h"
#include "include/portoption.h"
#include "include/beaconoption.h"
#include "../../core/include/qaprssymbols.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //QApplication::setStyle( "plastique" ); //установить пластиковый стиль
    //QApplication::setPalette( QApplication::style()->standardPalette() ); //со стандартной палитрой
    QApplication::setWindowIcon( QIcon( ":images/config.png" ) );

    PacketsWindow.setWindowIcon( QIcon( ":images/packet.png" ) );
    MessagesWindow.setWindowIcon( QIcon( ":images/kontact_mail.png" ) );
    StationsWindow.setWindowIcon( QIcon( ":images/stlist2.png" ) );
    MessengerWindow.setWindowIcon( QIcon( ":images/messenger.png" ) );

    //порты
    ui->saveButton->setIcon( QIcon( ":images/filesave.png" ) );
    ui->closeButton->setIcon( QIcon( ":images/closetab.png" ) );
    ui->cancelButton->setIcon( QIcon( ":images/cancelupd.png" ) );
    ui->qrySaveButton->setIcon( QIcon( ":images/filesave.png" ) );
    ui->qryCloseButton->setIcon( QIcon( ":images/closetab.png" ) );
    ui->qryCancelButton->setIcon( QIcon( ":images/cancelupd.png" ) );

    ui->editPortButton->setIcon( QIcon( ":images/edit.png" ) );
    ui->createPortButton->setIcon( QIcon( ":images/plus.png" ) );
    ui->deletePortButton->setIcon( QIcon( ":images/minus.png" ) );

    ui->openPortButton->setIcon( QIcon( ":images/port_on.png" ) );
    ui->closePortButton->setIcon( QIcon( ":images/port_off.png" ) );
    ui->openPortsButton->setIcon( QIcon( ":images/ports_on.png" ) );
    ui->closePortsButton->setIcon( QIcon( ":images/ports_off.png" ) );
    //маяки
    ui->createBeaconButton->setIcon( QIcon( ":images/plus.png" ) );
    ui->editBeaconButton->setIcon( QIcon( ":images/edit.png" ) );
    ui->deleteBeaconButton->setIcon( QIcon( ":images/minus.png" ) );
    ui->sendBeaconButton->setIcon( QIcon( ":images/position.png" ) );
    ui->sendStatusButton->setIcon( QIcon( ":images/label.png" ) );

    ui->onBeaconButton->setIcon( QIcon( ":images/port_on.png" ) );
    ui->offBeaconButton->setIcon( QIcon( ":images/port_off.png" ) );

    ui->onAllBeaconsButton->setIcon( QIcon( ":images/ports_on.png" ) );
    ui->offPortsButton->setIcon( QIcon( ":images/ports_off.png" ) );
    //квери
    ui->qryAddCallButton->setIcon( QIcon( ":images/plus.png" ) );
    ui->qryDeleteCallButton->setIcon( QIcon( ":images/minus.png" ) );


    createActions();
    createTrayIcon();

    //основная закладка
    connect( ui->saveButton,         SIGNAL( clicked() ),
             this,                   SLOT( saveButtonClick() ) );

    connect( ui->cancelButton,       SIGNAL( clicked() ),
             this,                   SLOT( cancelButtonClick() ) );

    connect( ui->closeButton,        SIGNAL( clicked() ),
             this,                   SLOT( closeButtonClick() ) );

    connect( ui->qrySaveButton,      SIGNAL( clicked() ),
             this,                   SLOT( qrySaveButtonClick() ) );

    connect( ui->qryCancelButton,    SIGNAL( clicked() ),
             this,                   SLOT( qryCancelButtonClick() ) );

    connect( ui->qryCloseButton,     SIGNAL( clicked() ),
             this,                   SLOT( qryCloseButtonClick() ) );

    //закладка с портами
    connect( ui->openPortsButton,    SIGNAL( clicked() ),
             this,                   SLOT( openPortsButtonClick() ) );

    connect( ui->createPortButton,   SIGNAL( clicked() ),
             this,                   SLOT( createPortButtonClick() ) );

    connect( ui->closePortsButton,   SIGNAL( clicked() ),
             this,                   SLOT( closePortsButtonClick() ) );

    connect( ui->openPortButton,     SIGNAL( clicked() ),
             this,                   SLOT( openPortButtonClick() ) );

    connect( ui->closePortButton,    SIGNAL( clicked() ),
             this,                   SLOT( closePortButtonClick() ) );

    connect( ui->deletePortButton,   SIGNAL( clicked() ),
             this,                   SLOT( deletePortButtonClick() ) );

    connect( ui->editPortButton,     SIGNAL( clicked() ),
             this,                   SLOT( editPortButtonClick() ) );

    connect( ui->portsTableView,     SIGNAL( doubleClicked ( QModelIndex ) ),
             this,                   SLOT( onPortDoubleClicked ( QModelIndex ) ) );

    //закладка с маяками
    connect( ui->createBeaconButton, SIGNAL( clicked() ),
             this,                   SLOT( createBeaconButtonClick() ) );

    connect( ui->editBeaconButton,   SIGNAL( clicked() ),
             this,                   SLOT( editBeaconButtonClick() ) );

    connect( ui->deleteBeaconButton, SIGNAL( clicked() ),
             this,                   SLOT( deleteBeaconButtonClick() ) );

    connect( ui->onBeaconButton,     SIGNAL( clicked() ),
             this,                   SLOT( openBeaconButtonClick() ) );

    connect( ui->offBeaconButton,    SIGNAL( clicked() ),
             this,                   SLOT( closeBeaconButtonClick() ) );

    connect( ui->onAllBeaconsButton, SIGNAL( clicked() ),
             this,                   SLOT( openBeaconsButtonClick() ) );

    connect( ui->offPortsButton,     SIGNAL( clicked() ),
             this,                   SLOT( closeBeaconsButtonClick() ) );

    connect( ui->sendBeaconButton,   SIGNAL( clicked() ),
             this,                   SLOT( sendBeaconButtonClick() ) );

    connect( ui->sendStatusButton,   SIGNAL( clicked() ),
             this,                   SLOT( sendStatusButtonClick() ) );



    //закладка с query
    connect( ui->qryAddCallButton,   SIGNAL( clicked() ),
             this,                   SLOT( qryAddCallButtonClick() ) );

    connect( ui->qryDeleteCallButton,SIGNAL( clicked() ),
             this,                   SLOT( qryDeleteCallButton() ) );


    //модель с портами
    pModel = new PortsModel(256, 3, parent);

    pModel->setHeaderData( 0, Qt::Horizontal, QObject::tr("PType") );
    pModel->setHeaderData( 1, Qt::Horizontal, QObject::tr("PNote") );
    pModel->setHeaderData( 2, Qt::Horizontal, QObject::tr("STATUS") );

    ui->portsTableView->setModel( pModel );

    connect( ui->portsTableView->selectionModel(),
             SIGNAL( currentRowChanged ( const QModelIndex &, const QModelIndex & ) ) ,
             this,
             SLOT( portsCurrentChanged ( const QModelIndex &, const QModelIndex & ) ) );


    ui->portsTableView->selectRow( 0 );

    ui->portsTableView->setColumnWidth(  0,  70 );
    ui->portsTableView->setColumnWidth(  1, 250 );
    ui->portsTableView->setColumnWidth(  2,  70 );

    //pModel - это чтобы пакеты  знали что за порты под номерами 1,2,3
    this->PacketsWindow.pModel = this->pModel;
    this->MessagesWindow.pModel= this->pModel;

    //модель с маяками
    bModel = new BeaconsModel(256, 13, parent);

    bModel->setHeaderData( 0, Qt::Horizontal, QObject::tr("Status") );
    bModel->setHeaderData( 1, Qt::Horizontal, QObject::tr("PN") );
    bModel->setHeaderData( 2, Qt::Horizontal, QObject::tr("PNote") );
    bModel->setHeaderData( 3, Qt::Horizontal, QObject::tr("BSym") );
    bModel->setHeaderData( 4, Qt::Horizontal, QObject::tr("Comm") );
    bModel->setHeaderData( 5, Qt::Horizontal, QObject::tr("BCall") );
    bModel->setHeaderData( 6, Qt::Horizontal, QObject::tr("BLat") );
    bModel->setHeaderData( 7, Qt::Horizontal, QObject::tr("BLng") );
    bModel->setHeaderData( 8, Qt::Horizontal, QObject::tr("BUnproto") );
    bModel->setHeaderData( 9, Qt::Horizontal, QObject::tr("BText") );
    bModel->setHeaderData(10, Qt::Horizontal, QObject::tr("BInterval") );
    bModel->setHeaderData(11, Qt::Horizontal, QObject::tr("SText") );
    bModel->setHeaderData(12, Qt::Horizontal, QObject::tr("SInterval") );

    ui->beaconsTableView->setModel( bModel );

    connect( ui->beaconsTableView->selectionModel(),
             SIGNAL( currentRowChanged ( const QModelIndex &, const QModelIndex & ) ) ,
             this,
             SLOT( beaconsCurrentChanged ( const QModelIndex &, const QModelIndex & ) ) );

    ui->beaconsTableView->selectRow( 0 );

    ui->beaconsTableView->setColumnWidth(  0,  50 );
    ui->beaconsTableView->setColumnWidth(  1,  25 );
    ui->beaconsTableView->setColumnWidth(  2,  50 );
    ui->beaconsTableView->setColumnWidth(  3,  40 );
    ui->beaconsTableView->setColumnWidth(  4,  50 );
    ui->beaconsTableView->setColumnWidth(  5,  65 );
    ui->beaconsTableView->setColumnWidth(  6,  65 );
    ui->beaconsTableView->setColumnWidth(  7,  70 );
    ui->beaconsTableView->setColumnWidth(  8, 110 );
    ui->beaconsTableView->setColumnWidth(  9, 110 );
    ui->beaconsTableView->setColumnWidth( 10,  70 );
    ui->beaconsTableView->setColumnWidth( 11, 110 );
    ui->beaconsTableView->setColumnWidth( 12,  70 );


    myCallsModel.setColumnCount( 1 );
    ui->qryCallsTableView->setModel( &myCallsModel );
    myCallsModel.clear();
    QStringList Labels;
    Labels << "Call";
    myCallsModel.setHorizontalHeaderLabels( Labels );
    ui->qryCallsTableView->setColumnWidth( 0, 80 );
    ui->qryCallsTableView->selectRow( 0 );

    int i;

    for(i=0;i<256;i++) lamps[ i ] = 0;



    //если ядро запускается вместе с ГУЙ
    //Core    = new QAPRSCore::QAPRSCore();
    //Core->Start();
    Core = nullptr;


    atomGUI = new QAtomix();
    atomGUI->setAtomName( "atomGUI" );

    timer = new QTimer( this->atomGUI );
    connect( timer, SIGNAL( timeout() ), this, SLOT( onTimer() ) );
    timer->start( 100 );

    //SysSymbols
    bModel->SysSymbols = atomGUI->SysSymbols;
    this->StationsWindow.SysSymbols = atomGUI->SysSymbols;
    this->StationsWindow.SysVars    = atomGUI->SysVars;

    PacketsWindow.atomGUI = atomGUI;
    MessengerWindow.SysStations = atomGUI->SysStations;
    MessengerWindow.SysPackets  = atomGUI->SysPackets;
    MessengerWindow.SysVars     = atomGUI->SysVars;
    MessengerWindow.atomGUI     = atomGUI;


    connect( atomGUI, SIGNAL( SIGweSysvarSet( QString, QString ) ),
             this,    SLOT(   weSysvarSet( QString, QString ) ) );

    connect( atomGUI, SIGNAL( SIGweSysportSetParam( int, QString, QString ) ),
             this,    SLOT(      weSysportSetParam( int, QString, QString ) ) );

    connect( atomGUI, SIGNAL( SIGweSysportChangeState( int, QString ) ),
             this,    SLOT(   weSysportChangeState( int, QString ) ) );

    connect( atomGUI, SIGNAL( SIGweSysbeaconSetParam( int, QString, QString ) ),
             this,    SLOT(      weSysbeaconSetParam( int, QString, QString ) ) );

    connect( atomGUI, SIGNAL( SIGweSysbeaconChangeState( int, QString ) ),
             this,    SLOT(   weSysbeaconChangeState( int, QString ) ) );

    connect( atomGUI, SIGNAL( SIGweportRXPacket( int, bool ) ),
             this,    SLOT(   weportRXPacket( int, bool ) ) );

    connect( atomGUI, SIGNAL( SIGweportTXPacket( int, bool ) ),
             this,    SLOT(   weportTXPacket( int, bool ) ) );

    connect( atomGUI,               SIGNAL( SIGportRXAPRSPacket( int, int, QString, QString, QString, QString, QString ) ),
             &PacketsWindow ,       SLOT(          RXAPRSPacket( int, int, QString, QString, QString, QString, QString ) ) );

    connect( atomGUI,               SIGNAL( SIGportTXAPRSPacket( int, int, QString, QString, QString, QString, QString ) ),
             &PacketsWindow ,       SLOT(          TXAPRSPacket( int, int, QString, QString, QString, QString, QString ) ) );

    connect( atomGUI,               SIGNAL( RCVStationPositionInfo( QString, QString, QString, QString, QString, QString, QString, QString ) ),
             &StationsWindow,       SLOT(   RCVStationPositionInfo( QString, QString, QString, QString, QString, QString, QString, QString ) ) );

    connect( atomGUI,               SIGNAL( RCVStationStatus( QString, QString ) ),
             &StationsWindow,       SLOT(   RCVStationStatus( QString, QString ) ) );

    //void    RCVRXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
    //void    RCVTXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );

    connect( atomGUI,               SIGNAL( RCVRXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ),
             &MessagesWindow,       SLOT(      RXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ) );

    connect( atomGUI,               SIGNAL( RCVTXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ),
             &MessagesWindow,       SLOT(      TXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ) );

    connect( atomGUI,               SIGNAL( RCVRXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ),
             &MessengerWindow,      SLOT(      RXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ) );

    connect( atomGUI,               SIGNAL( RCVTXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ),
             &MessengerWindow,      SLOT(      TXAPRSMessage( int, int, QString, QString, QString, QString, QString ) ) );


    connect( atomGUI,               SIGNAL( SIGallLoaded( ) ),
             this,       SLOT(      allLoaded( ) ) );




    //atomGUI->beClient( "127.0.0.1", 6000 );



    ui->portsTableView->selectRow( 0 );
    this->updPortsButtonState ( );

    scene = new QGraphicsScene();
    map   = new QGraphicsPixmapItem();

    ui->graphicsView->setScene( scene );
    scene->addItem( map );

    map->setPixmap( QPixmap( "etc/misa.jpg" ) );
    map->setTransformationMode ( Qt::SmoothTransformation );

    map->setScale( map->scale() * 0.6 );



    qDebug() << "MainWindow::MainWindow()";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions() {

    showGenOptionAction = new QAction(tr("&Options"), this);
    connect(showGenOptionAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    showGenOptionAction->setIcon( QIcon(":/images/config.png") );

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeApp()));
    quitAction->setIcon( QIcon(":/images/exit.png") );

    showPacketsAction = new QAction(tr("&Packets"), this);
    connect(showPacketsAction, SIGNAL(triggered()), this, SLOT(showPackets()));
    showPacketsAction->setIcon( QIcon(":/images/packet.png") );

    showMessagesAction = new QAction(tr("&Messages"), this);
    connect(showMessagesAction, SIGNAL(triggered()), this, SLOT(showMessages()));
    showMessagesAction->setIcon( QIcon(":/images/kontact_mail.png") );

    showStationsAction = new QAction(tr("&Stations"), this);
    connect(showStationsAction, SIGNAL(triggered()), this, SLOT(showStations()));
    showStationsAction->setIcon( QIcon(":/images/stlist2.png") );

    showMapsAction = new QAction(tr("&Maps"), this);
    connect(showMapsAction, SIGNAL(triggered()), this, SLOT(showMaps()));
    showMapsAction->setIcon( QIcon(":/images/map.png") );

    showMessengerAction = new QAction(tr("&Messenger"), this);
    connect(showMessengerAction, SIGNAL(triggered()), this, SLOT(showMessenger()));
    showMessengerAction->setIcon( QIcon(":/images/messenger.png") );

    //экшены портов
    openPortAction   = new QAction(tr("&Open Port"), this);
    openPortAction  ->setIcon( QIcon(":/images/port_on.png") );
    connect( openPortAction,   SIGNAL( triggered() ), this, SLOT( openPortButtonClick() ) );

    closePortAction  = new QAction(tr("&Close Port"), this);
    closePortAction ->setIcon( QIcon(":/images/port_off.png") );
    connect( closePortAction,  SIGNAL( triggered() ), this, SLOT( closePortButtonClick() ) );

    createPortAction = new QAction(tr("&Create Port"), this);
    createPortAction->setIcon( QIcon(":/images/plus.png") );
    connect( createPortAction, SIGNAL( triggered() ), this, SLOT( createPortButtonClick() ) );

    editPortAction   = new QAction(tr("&Edit Port Parameters"), this);
    editPortAction  ->setIcon( QIcon(":/images/edit.png") );
    connect( editPortAction,   SIGNAL( triggered() ), this, SLOT( editPortButtonClick() ) );

    deletePortAction = new QAction(tr("&Delete Port"), this);
    deletePortAction->setIcon( QIcon(":/images/minus.png") );
    connect( deletePortAction, SIGNAL( triggered() ), this, SLOT( deletePortButtonClick() ) );

    openPortsAction  = new QAction(tr("&Open All Ports"), this);
    openPortsAction ->setIcon( QIcon(":/images/ports_on.png") );
    connect( openPortsAction,  SIGNAL( triggered() ), this, SLOT( openPortsButtonClick() ) );

    closePortsAction = new QAction(tr("&Close All Ports"), this);
    closePortsAction->setIcon( QIcon(":/images/ports_off.png") );
    connect( closePortsAction, SIGNAL( triggered() ), this, SLOT( closePortsButtonClick() ) );

    //экшены маяков
    createBeaconAction  = new QAction(tr("&Create Beacon"), this);
    createBeaconAction  ->setIcon( QIcon(":/images/plus.png") );
    connect( createBeaconAction,  SIGNAL( triggered() ), this, SLOT( createBeaconButtonClick() ) );

    editBeaconAction    = new QAction(tr("&Edit Beacon Parameters"), this);
    editBeaconAction    ->setIcon( QIcon(":/images/edit.png") );
    connect( editBeaconAction,    SIGNAL( triggered() ), this, SLOT( editBeaconButtonClick() ) );

    deleteBeaconAction  = new QAction(tr("&Delete Beacon"), this);
    deleteBeaconAction  ->setIcon( QIcon(":/images/minus.png") );
    connect( deleteBeaconAction,  SIGNAL( triggered() ), this, SLOT( deleteBeaconButtonClick() ) );

    openBeaconAction    = new QAction(tr("&To switch ON the Beacon"), this);
    openBeaconAction    ->setIcon( QIcon(":/images/port_on.png") );
    connect( openBeaconAction,    SIGNAL( triggered() ), this, SLOT( openBeaconButtonClick() ) );

    closeBeaconAction   = new QAction(tr("&To switch OFF the Beacon"), this);
    closeBeaconAction   ->setIcon( QIcon(":/images/port_off.png") );
    connect( closeBeaconAction,   SIGNAL( triggered() ), this, SLOT( closeBeaconButtonClick() ) );

    sendBeaconAction    = new QAction(tr("&Send Beacon"), this);
    sendBeaconAction    ->setIcon( QIcon(":/images/position.png") );
    connect( sendBeaconAction,    SIGNAL( triggered() ), this, SLOT( sendBeaconButtonClick() ) );

    sendStatusAction    = new QAction(tr("&Send Status"), this);
    sendStatusAction    ->setIcon( QIcon(":/images/label.png") );
    connect( sendStatusAction,    SIGNAL( triggered() ), this, SLOT( sendStatusButtonClick() ) );

    openBeaconsAction   = new QAction(tr("&Switch ON all Beacons"), this);
    openBeaconsAction   ->setIcon( QIcon(":/images/ports_on.png") );
    connect( openBeaconsAction,   SIGNAL( triggered() ), this, SLOT( openBeaconsButtonClick() ) );

    closeBeaconsAction  = new QAction(tr("&Switch OFF all Beacons"), this);
    closeBeaconsAction  ->setIcon( QIcon(":/images/ports_off.png") );
    connect( closeBeaconsAction,  SIGNAL( triggered() ), this, SLOT( closeBeaconsButtonClick() ) );



}

void MainWindow::createTrayIcon() {

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showPacketsAction);
    trayIconMenu->addAction(showMessagesAction);
    trayIconMenu->addAction(showStationsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(showMapsAction);
    trayIconMenu->addAction(showMessengerAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(showGenOptionAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon( QIcon(":/images/logo.png") );
    trayIcon->setToolTip( versionName );
    trayIcon->show();

    connect( trayIcon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT( systrayclick() ) );

}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::showEvent( QShowEvent *event ) {

    int i;

    //восстановление размеров и положения окна
    QSettings settings( versionName, "MainWindow" );
    restoreGeometry( settings.value("geometry").toByteArray() );
    restoreState( settings.value("windowState").toByteArray() );

    //восстановление ширины столбцов
    for(i=0;i<=2;i++)
        if ( !settings.value("MainWindowPortsColumn" + QString::number( i ) + "Width").isNull() )
            ui->portsTableView->setColumnWidth( i, settings.value("MainWindowPortsColumn" + QString::number( i ) + "Width").toInt() );

    for(i=0;i<=12;i++)
        if ( !settings.value("MainWindowBeaconsColumn" + QString::number( i ) + "Width").isNull() )
            ui->beaconsTableView->setColumnWidth( i, settings.value("MainWindowBeaconsColumn" + QString::number( i ) + "Width").toInt() );


}


void MainWindow::closeEvent(QCloseEvent *event) {

    int i;

    //сохранение размеров и положения окна
    QSettings settings( versionName, "MainWindow");
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );

    //сохранение ширины столбцов
    for(i=0;i<=2;i++)
        settings.setValue( "MainWindowPortsColumn" + QString::number( i ) + "Width", ui->portsTableView->columnWidth( i ) );

    for(i=0;i<=12;i++)
        settings.setValue( "MainWindowBeaconsColumn" + QString::number( i ) + "Width", ui->beaconsTableView->columnWidth( i ) );



    hide();
    event->ignore();

}

void MainWindow::hideEvent ( QHideEvent * event ) {

}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {

    if ( ui->tabWidget->currentIndex() == 1 ) {

        QMenu menu("my", 0);

        menu.addAction( createPortAction );
        menu.addAction( editPortAction );
        menu.addAction( deletePortAction );
        menu.addSeparator();
        menu.addAction( openPortAction );
        menu.addAction( closePortAction );
        menu.addSeparator();
        menu.addAction( openPortsAction );
        menu.addAction( closePortsAction );

        createPortAction ->setEnabled( ui->createPortButton->isEnabled() );
        editPortAction   ->setEnabled( ui->editPortButton->isEnabled() );
        deletePortAction ->setEnabled( ui->deletePortButton->isEnabled() );

        openPortAction   ->setEnabled( ui->openPortButton->isEnabled() );
        closePortAction  ->setEnabled( ui->closePortButton->isEnabled() );

        openPortsAction  ->setEnabled( ui->openPortsButton->isEnabled() );
        closePortsAction ->setEnabled( ui->closePortsButton->isEnabled() );


        menu.exec( event->globalPos() );

    } else if ( ui->tabWidget->currentIndex() == 2 ) {

    QMenu menu("my", 0);

    menu.addAction( createBeaconAction );
    menu.addAction( editBeaconAction );
    menu.addAction( deleteBeaconAction );
    menu.addSeparator();
    menu.addAction( openBeaconAction );
    menu.addAction( closeBeaconAction );
    menu.addSeparator();
    menu.addAction( sendBeaconAction );
    menu.addAction( sendStatusAction );
    menu.addSeparator();
    menu.addAction( openBeaconsAction );
    menu.addAction( closeBeaconsAction );

    createBeaconAction->setEnabled( ui->createBeaconButton->isEnabled() );
    editBeaconAction  ->setEnabled( ui->editBeaconButton->isEnabled() );
    deleteBeaconAction->setEnabled( ui->deleteBeaconButton->isEnabled() );

    openBeaconAction  ->setEnabled( ui->onBeaconButton->isEnabled() );
    closeBeaconAction ->setEnabled( ui->offBeaconButton->isEnabled() );

    sendBeaconAction  ->setEnabled( ui->sendBeaconButton->isEnabled() );
    sendStatusAction  ->setEnabled( ui->sendStatusButton->isEnabled() );


    menu.exec( event->globalPos() );

}

}

void MainWindow::saveButtonClick() {
//сохранение параметров
    int i;

    //setDBValue( "Lat",      ui->latEdit->text() );
    //setDBValue( "Lng",      ui->lngEdit->text() );

    QList<QLineEdit *> widgets = this->findChildren<QLineEdit *>();
    //if ( widgets.count() == 1 ) widgets.at( 0 )->setText( varval );
    for(i=0;i<widgets.count();i++) {
        if ( widgets.at( i )->objectName().left( 7 ) == "sysvar_" ) {
            QString sysvarName = widgets.at( i )->objectName().mid( 7 );
            QString sysvarValue = widgets.at( i )->text();
            //qDebug() << "sysvarName = " << sysvarName << " sysvarValue=" << sysvarValue;

            //setDBValue( sysvarName, sysvarValue );
            //atomGUI->doThink( "SYSVAR SET " + sysvarName + "=" + sysvarValue );
            this->atomGUI->weSysvarSet( sysvarName, sysvarValue );
        }

    }

}

void MainWindow::cancelButtonClick() {
//отменить изменения параметров

   //this->atomGUI->loadingMode = true;
   this->atomGUI->weSysvarReq();
   //this->atomGUI->doThink( "ALL DATA LOADED?" );

}

void MainWindow::closeButtonClick() {
//скрытие клиентской части

    this->close();

}

void MainWindow::showPackets() {
//показываем окно пакетов
    PacketsWindow.enableSort = true;
    PacketsWindow.show();
}

void MainWindow::showMessages() {
//показываем окно сообщений
    MessagesWindow.show();
}

void MainWindow::showStations() {
//показываем окно станций
    StationsWindow.show();
}

void MainWindow::showMaps() {
//создаем и показываем окно карт

    MapsWindowImpl *win;

    win = new MapsWindowImpl();

    win->setWindowIcon( QIcon( ":images/map.png" ) );


    /*
    //qDebug() << win;

    connect( this,       SIGNAL( RCVStationInfo( QString, QString, QString, QString, QString, QString, QString, QString, QString ) ),
             win,        SLOT( RCVStationInfo( QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );


    //tcpGUIClient->write( QByteArray( "STATIONS GET\r\n" ) );
    win->stationList = new QAPRSstationList();

    win->stationList->copyFromList( stationList );

    win->symNames    = symNames;
    win->symImgs     = symImgs; */

    //connect( this,       SIGNAL( RCVStationInfo( QString, QString, QString, QString, QString, QString, QString, QString, QString ) ),
    //         win,        SLOT( RCVStationInfo( QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );

    win->SysStations = new QAPRSstationList();
    win->SysStations->copyFromList( this->atomGUI->SysStations );

    //это чтобы параметры применялись на все копии списка станций
    connect( this->atomGUI,     SIGNAL( SIGstationSetParam( QString, QString, QString ) ),
             win->SysStations,  SLOT( stationSetParam( QString, QString, QString ) ) );

    //а это чтоб на карте обновлялось при изменении параметров
    connect( win->SysStations,     SIGNAL( SIGstationSetParam( QString ) ),
             win,                  SLOT( RCVStationInfo( QString ) ) );



    win->SysSymbols = this->atomGUI->SysSymbols;


    win->setVisible( true );

}

void MainWindow::showMessenger() {
//показываем окно почтовика

    /*

    MessengerWindow.toComboBox->

    for(i=0;i<256;i++) {

        if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "" ) {
            portSym = ":images/port.png";
            win->portNumBox->addItem( QIcon( portSym ), QString::number( i + 1 ), i );
        } else {
            portSym = ":images/port.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "AXIP" )   portSym = ":images/portaxip.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "KISS" )   portSym = ":images/portkiss.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "AGW" )    portSym = ":images/portagw.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "Inet" )   portSym = ":images/portinet.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "FLDIGI" ) portSym = ":images/portfldigi.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "VASKA" )  portSym = ":images/portvaska.png";
            win->portNumBox->addItem( QIcon( portSym ),
                                      QString::number( i + 1 ) + "-" +
                                      pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() + "-" +
                                      pModel->data( pModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toString(), i );

        }

    }
*/

    MessengerWindow.updateStationList();
    MessengerWindow.updateMessageList();
    MessengerWindow.show();


}

void MainWindow::closeApp() {
//закрытие клиентской части
/*
    this->Core->atomCORE->weSysbeaconCloseAll();
    this->Core->atomCORE->weSysportCloseAll();

    //если ядро запускается вместе с ГУЙ
    if ( this->Core->atomCORE->started ) {
        StorageProgressDialog *win;

        win = new StorageProgressDialog();

        connect( this->Core->atomCORE, SIGNAL( storeProgress( QString ) ),
                 win,                  SLOT( storeProgress( QString) ) );

        win->exec();

    }
*/
    if ( this->Core != nullptr ) this->Core->atomCORE->wait();

    trayIcon->hide();

    this->close();
    PacketsWindow.close(); //окно с пакетами
    MessagesWindow.close(); //окно с сообщениями
    StationsWindow.close(); //окно со станциями
    MessengerWindow.close(); //окно почтовика

    qApp->quit();

}

void MainWindow::systrayclick ( ) {
//клик по иконке в трее

}

//на закладке ports
void MainWindow::openPortsButtonClick() {
//открыть все порты

    atomGUI->weSysportOpenAll();

}

void MainWindow::closePortsButtonClick() {
//закрыть все порты

    atomGUI->weSysportCloseAll();

}

void MainWindow::openPortButtonClick() {
//открыть определенный порт

    atomGUI->weSysportOpen( ui->portsTableView->currentIndex().row() );

}

void MainWindow::closePortButtonClick() {
//закрыть определенный порт

    atomGUI->weSysportClose( ui->portsTableView->currentIndex().row() );

}

void MainWindow::deletePortButtonClick() {
//удалить определенный порт
    QByteArray cmd;

    int ret = QMessageBox::warning(this, tr("CORE Message - Remove Port?"),
                                   tr("At port removal all its parametres from a system database leave\n"
                                      "Remove Port?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No );

    if ( ret == QMessageBox::Yes ) {

        atomGUI->weSysportDelete( ui->portsTableView->currentIndex().row() );

    }

}

void MainWindow::editPortButtonClick() {
//редактировать параметры определенного порта

    PortoptionDialog *win;

    win = new PortoptionDialog();

    win->portNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->portNumber = ui->portsTableView->currentIndex().row();

    win->portTypeBox->setCurrentIndex( 0 );
    win->stackedWidget->setCurrentIndex( 0 );

    //отправлять все принятые от ядра параметры портов - диалогу параметров порта


    connect( this->atomGUI , SIGNAL(SIGweSysportSetParam( int, QString, QString )),
             win,            SLOT(RCVPortParam( int, QString, QString )));

    //отправлять ядру все установленные на диалоге параметры
    connect(win, SIGNAL(setPortParam( int, QString, QString )),
            this, SLOT(setPortParamFFrm( int, QString, QString )));

    connect(win, SIGNAL(updatePort( int )),
            this, SLOT(updatePortFFrm( int )));


    atomGUI->loadingMode = true;
    atomGUI->weSysportReqParams( ui->portsTableView->currentIndex().row() );
    //atomGUI->doThink( "ALL DATA LOADED?" );

    win->show();

}

void MainWindow::createPortButtonClick() {
//добавить порт
//то же самое, только параметры не запрашиваем
    PortoptionDialog *win;

    win = new PortoptionDialog();

    win->setWindowTitle( tr("Create NEW port; ") + win->windowTitle() );

    win->portNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->portNumber = ui->portsTableView->currentIndex().row();

    win->portTypeBox->setCurrentIndex( 0 );
    win->stackedWidget->setCurrentIndex( 0 );

    //отправлять все принятые от ядра параметры портов - диалогу параметров порта


    connect( this->atomGUI , SIGNAL(SIGweSysportSetParam( int, QString, QString )),
             win,            SLOT(RCVPortParam( int, QString, QString )));

    //отправлять ядру все установленные на диалоге параметры
    connect(win, SIGNAL(setPortParam( int, QString, QString )),
            this, SLOT(setPortParamFFrm( int, QString, QString )));

    connect(win, SIGNAL(updatePort( int )),
            this, SLOT(updatePortFFrm( int )));

    win->show();

}

void MainWindow::onPortDoubleClicked( const QModelIndex & index ) {
//при двойном щелчке по порту - открываем его параметры

    if ( ui->editPortButton->isEnabled() == true ) {
        this->editPortButtonClick();
    }

}

void MainWindow::portsCurrentChanged( const QModelIndex & current, const QModelIndex & previous ) {

    this->updPortsButtonState ( );

}

void MainWindow::updatePortFFrm( int pNum ) {
//сюда принимается сигнал о необходимости обновить параметры определенного порта

    this->atomGUI->weSysportUpdate( pNum );
    //this->atomGUI->say( "update port"+QString::number( pNum ) );

}

void MainWindow::updPortsButtonState ( ) {

    if ( pModel->data( pModel->index( ui->portsTableView->currentIndex().row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "" ) {

        ui->openPortButton->setEnabled( false );
        ui->closePortButton->setEnabled( false );
        ui->deletePortButton->setEnabled( false );
        ui->editPortButton->setEnabled( false );
        ui->createPortButton->setEnabled( true );

    } else {

        ui->openPortButton->setEnabled( pModel->data( pModel->index( ui->portsTableView->currentIndex().row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "OFF" );
        ui->closePortButton->setEnabled( pModel->data( pModel->index( ui->portsTableView->currentIndex().row(), 2, QModelIndex()), Qt::DisplayRole ).toString() != "OFF" );
        ui->deletePortButton->setEnabled( true );
        ui->editPortButton->setEnabled( true );
        ui->createPortButton->setEnabled( false );

    }

    ui->portLabel->setText( "<" + QString::number( ui->portsTableView->currentIndex().row() + 1 ) + "/256>" );

}

void MainWindow::onTimer() {
//гасим зажженные лампочки
    int i;
    //Если лампочки горят, то выключаем их

    for(i=0;i<256;i++) {
        if ( lamps[ i ] > 0 ) {

            lamps[ i ] = lamps[ i ] - 1;
            if ( lamps[ i ] == 0 ) {
                if (pModel->data( pModel->index( i, 2, QModelIndex()), Qt::DisplayRole ).toString()=="RX") {
                    pModel->setData( pModel->index( i, 2, QModelIndex()), "ON" );
                    ui->portsTableView->repaint();
                }
                if (pModel->data( pModel->index( i, 2, QModelIndex()), Qt::DisplayRole ).toString()=="TX") {
                    pModel->setData( pModel->index( i, 2, QModelIndex()), "ON" );
                    ui->portsTableView->repaint();
                }

            }

        }

    }

        /*
        if (pModel->data( pModel->index( i, 2, QModelIndex()), Qt::DisplayRole ).toString()=="RX") {
            pModel->setData( pModel->index( i, 2, QModelIndex()), "ON" );
            ui->portsTableView->repaint();
        }
        if (pModel->data( pModel->index( i, 2, QModelIndex()), Qt::DisplayRole ).toString()=="TX") {
            pModel->setData( pModel->index( i, 2, QModelIndex()), "ON" );
            ui->portsTableView->repaint();
        }*/

}

//закладк beacons
void MainWindow::createBeaconButtonClick() {
//создать маяк
    BeaconoptionDialog *win;
    QString portSym;

    win = new BeaconoptionDialog();

    win->bckNumEdit->setText( QString::number( ui->beaconsTableView->currentIndex().row() + 1 ) );
    win->beaconNumber = ui->beaconsTableView->currentIndex().row();

    QAPRSSymbol *tmp;

    tmp = this->atomGUI->SysSymbols->first;

    while ( tmp != nullptr ) {

        //qDebug() << "code=" << tmp->code << " icon=" << tmp->img << " name=" << tmp->name;

        win->bckSymBox->addItem( QIcon( tmp->img().prepend("./symbols/") ),
                                 tmp->name() ,
                                 tmp->code() );

        tmp = tmp->next;
    }

    win->bckNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->beaconNumber = ui->beaconsTableView->currentIndex().row();

    win->bckLatEdit->setText( ui->sysvar_Lat->text() );
    win->bckLngEdit->setText( ui->sysvar_Lng->text() );

    win->bckUnprotoEdit->setText( ui->sysvar_DefaultUnproto->text() );
    win->bckTextEdit->setText( ui->sysvar_Beacon->text() );

    win->bckCallEdit->setText( ui->sysvar_Call->text() );
    win->bckIntervalEdit->setText( "-1" );

    win->statusTextEdit->setText( "$VERSION" );
    win->statusIntervalEdit->setText( "-1" );

    win->bckSymBox->setCurrentIndex(  win->bckSymBox->findData( ui->sysvar_Symbol->text() ) );

/*
    int i;
    for(i=0;i<( symNames->count() / 2 );i++) {

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i ).toString(),
                                 symNames->keys().at( i ) );

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i + ( symNames->count() / 2 ) ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i + ( symNames->count() / 2 ) ).toString(),
                                 symNames->keys().at( i + ( symNames->count() / 2 ) ) );

    }
*/

    int i;
    for(i=0;i<256;i++) {

        if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "" ) {
            portSym = ":images/port.png";
            win->portNumBox->addItem( QIcon( portSym ), QString::number( i + 1 ), i );
        } else {
            portSym = ":images/port.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "AXIP" )   portSym = ":images/portaxip.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "KISS" )   portSym = ":images/portkiss.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "AGW" )    portSym = ":images/portagw.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "Inet" )   portSym = ":images/portinet.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "FLDIGI" ) portSym = ":images/portfldigi.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "VASKA" )  portSym = ":images/portvaska.png";
            win->portNumBox->addItem( QIcon( portSym ),
                                      QString::number( i + 1 ) + "-" +
                                      pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() + "-" +
                                      pModel->data( pModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toString(), i );

        }

    }


    connect(this->atomGUI, SIGNAL(SIGweSysbeaconSetParam( int, QString, QString )),
            win,    SLOT(RCVBeaconParam( int, QString, QString )));

    connect(win, SIGNAL(setBeaconParam( int, QString, QString )),
            this,  SLOT(setBeaconParamFFrm( int, QString, QString )));

    connect(win, SIGNAL(updateBeacon( int )),
            this,  SLOT(updateBeaconFFrm( int )));

    //QByteArray cmd;
    //cmd.append( "SYSBEACON BCN_NUM=" + QString::number( ui->beaconsTableView->currentIndex().row() ) + " SET\r\n" );
    //tcpGUIClient->write( cmd );

    win->show();

}

void MainWindow::editBeaconButtonClick() {
//редактировать параметры определенного маяка

    BeaconoptionDialog *win;
    QString portSym;

    win = new BeaconoptionDialog();

    win->bckNumEdit->setText( QString::number( ui->beaconsTableView->currentIndex().row() + 1 ) );
    win->beaconNumber = ui->beaconsTableView->currentIndex().row();

    QAPRSSymbol *tmp;

    tmp = this->atomGUI->SysSymbols->first;

    while ( tmp != nullptr ) {

        //qDebug() << "code=" << tmp->code << " icon=" << tmp->img << " name=" << tmp->name;

        win->bckSymBox->addItem( QIcon( tmp->img().prepend("./symbols/") ),
                                 tmp->name(),
                                 tmp->code() );

        tmp = tmp->next;
    }

/*
    int i;
    for(i=0;i<( symNames->count() / 2 );i++) {

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i ).toString(),
                                 symNames->keys().at( i ) );

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i + ( symNames->count() / 2 ) ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i + ( symNames->count() / 2 ) ).toString(),
                                 symNames->keys().at( i + ( symNames->count() / 2 ) ) );

    }
*/

    int i;
    for(i=0;i<256;i++) {

        if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "" ) {
            portSym = ":images/port.png";
            win->portNumBox->addItem( QIcon( portSym ), QString::number( i + 1 ), i );
        } else {
            portSym = ":images/port.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "AXIP" )   portSym = ":images/portaxip.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "KISS" )   portSym = ":images/portkiss.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "AGW" )    portSym = ":images/portagw.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "Inet" )   portSym = ":images/portinet.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "FLDIGI" ) portSym = ":images/portfldigi.png";
            if ( pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() == "VASKA" )  portSym = ":images/portvaska.png";
            win->portNumBox->addItem( QIcon( portSym ),
                                      QString::number( i + 1 ) + "-" +
                                      pModel->data( pModel->index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() + "-" +
                                      pModel->data( pModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toString(), i );

        }

    }


    connect(this->atomGUI, SIGNAL(SIGweSysbeaconSetParam( int, QString, QString )),
            win,    SLOT(RCVBeaconParam( int, QString, QString )));

    connect(win, SIGNAL(setBeaconParam( int, QString, QString )),
            this,  SLOT(setBeaconParamFFrm( int, QString, QString )));

    connect(win, SIGNAL(updateBeacon( int )),
            this,  SLOT(updateBeaconFFrm( int )));

    //QByteArray cmd;
    //cmd.append( "SYSBEACON BCN_NUM=" + QString::number( ui->beaconsTableView->currentIndex().row() ) + " SET\r\n" );
    //tcpGUIClient->write( cmd );


    atomGUI->loadingMode = true;
    atomGUI->weSysbeaconReqParams( ui->beaconsTableView->currentIndex().row() );
    //atomGUI->doThink( "ALL DATA LOADED?" );

    win->show();

}

void MainWindow::deleteBeaconButtonClick() {
//удалить определенный маяк

        int ret = QMessageBox::warning(this, tr("CORE Message - Remove Beacon?"),
                                       tr("At beacon removal all its parametres from a system database leave\n"
                                          "Remove Beacon?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No );

        if ( ret == QMessageBox::Yes ) {

            atomGUI->weSysbeaconDelete( ui->beaconsTableView->currentIndex().row() );

        }

}

void MainWindow::openBeaconButtonClick() {
//открыть определенный маяк

    atomGUI->weSysbeaconOpen( ui->beaconsTableView->currentIndex().row() );

}

void MainWindow::closeBeaconButtonClick() {
//закрыть определенный маяк

    atomGUI->weSysbeaconClose( ui->beaconsTableView->currentIndex().row() );

}

void MainWindow::sendBeaconButtonClick() {
//послать маяк

    atomGUI->weSysbeaconSend( ui->beaconsTableView->currentIndex().row() );

}

void MainWindow::sendStatusButtonClick() {
//послать статус

    atomGUI->weSysbeaconStatusSend( ui->beaconsTableView->currentIndex().row() );

}

void MainWindow::openBeaconsButtonClick() {
//открыть все маяки

    atomGUI->weSysbeaconOpenAll();

}

void MainWindow::closeBeaconsButtonClick() {
//закрыть все маяки

    atomGUI->weSysbeaconCloseAll();

}

void MainWindow::beaconsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous ) {

    this->updBeaconsButtonState();

}

void MainWindow::updBeaconsButtonState() {

    if ( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "" ) {

        ui->onBeaconButton->setEnabled( false );
        ui->offBeaconButton->setEnabled( false );
        ui->deleteBeaconButton->setEnabled( false );
        ui->editBeaconButton->setEnabled( false );
        ui->sendBeaconButton->setEnabled( false );
        ui->sendStatusButton->setEnabled( false );
        ui->createBeaconButton->setEnabled( true );

    } else {

        ui->onBeaconButton->setEnabled( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "OFF" );
        ui->offBeaconButton->setEnabled( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() != "OFF" );
        //ui->sendBeaconButton->setEnabled( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() != "OFF" );
        ui->sendBeaconButton->setEnabled( true );
        ui->sendStatusButton->setEnabled( true );
        ui->deleteBeaconButton->setEnabled( true );
        ui->editBeaconButton->setEnabled( true );
        ui->createBeaconButton->setEnabled( false );

    }

    ui->beaconLabel->setText( "<" + QString::number( ui->beaconsTableView->currentIndex().row() + 1 ) + "/256>" );

}

void MainWindow::storeProgress( QString msg) {

    /*
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    splash->showMessage( msg, topRight, Qt::white );
    splash->repaint();
    qApp->processEvents();
*/
    //Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    //splash->showMessage( msg, topRight, Qt::white );

}

void MainWindow::allLoaded( ) {

    qDebug() << "allLoaded";

    //splash->finish( this );
    //delete splash;

    qDebug() << "packetCount=" << this->atomGUI->SysPackets->packetCount;
    this->PacketsWindow.packetsModel.setRowCount( this->atomGUI->SysPackets->packetCount );
    //this->PacketsWindow.pModel->setColumnCount( 50 );

    QAPRSPacket *tmp;
    tmp = this->atomGUI->SysPackets->last;
    int i = 0;

    while ( tmp != nullptr ) {

        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 0 ), tmp->K );

        QDateTime PacketDateTime;

        PacketDateTime = QDateTime::fromString( tmp->DT , "dd.MM.yyyy hh:mm:ss.zzz" );

        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 0 ), tmp->K );
        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 1 ), PacketDateTime.toString( "dd.MM.yyyy" ) );
        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 2 ), PacketDateTime.toString( "hh:mm:ss" ) );
        if ( pModel != nullptr ) PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 3 ), "(" + QString::number( tmp->Port_Num ) + ") " + pModel->data( pModel->index( tmp->Port_Num, 0, QModelIndex()), Qt::DisplayRole ).toString() );

        if ( pModel != nullptr ) PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 4 ), pModel->data( pModel->index( tmp->Port_Num, 1, QModelIndex()), Qt::DisplayRole ).toString() );

        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 5 ), tmp->TRX );
        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 6 ), tmp->PTo );
        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 7 ), tmp->PFrom );
        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 8 ), tmp->PVia );
        PacketsWindow.packetsModel.setData( PacketsWindow.packetsModel.index( i, 9 ), tmp->Message );

        i++;
        tmp = tmp->prev;
    }





}

void MainWindow::updateBeaconFFrm( int bNum ) {
//сюда принимается сигнал о необходимости обновить параметры определенного маяка
    this->atomGUI->weSysbeaconUpdate( bNum );
}

void MainWindow::setBeaconParamFFrm( int bNum, QString pName, QString pVal  ) {
//сюда принимаются параметры от форм редактирования параметров маяков
    this->atomGUI->weSysbeaconSetParam( bNum, pName, pVal );
}

//закладка Query
void MainWindow::qrySaveButtonClick() {
//сохранение параметров
    int i;

    QList<QCheckBox *> widgets = this->findChildren<QCheckBox *>();
    for(i=0;i<widgets.count();i++) {
        if ( widgets.at( i )->objectName().left( 7 ) == "sysvar_" ) {
            QString sysvarName = widgets.at( i )->objectName().mid( 7 );
            QString sysvarValue;
            if (widgets.at( i )->isChecked() ) {
                sysvarValue = "0";
            } else {
                sysvarValue = "1";
            }
            //atomGUI->doThink( "SYSVAR SET " + sysvarName + "=" + sysvarValue );
            this->atomGUI->weSysvarSet( sysvarName, sysvarValue );
        }

    }

}

void MainWindow::qryCancelButtonClick() {
//отменить изменения параметров

    //atomGUI->doThink( "", a, "SYSVAR SET" );
    this->atomGUI->weSysvarReq();

}

void MainWindow::qryCloseButtonClick() {
//скрытие клиентской части

    this->close();

}

void MainWindow::qryAddCallButtonClick() {
//добавить позывной в список

    bool ok;

    QString text = QInputDialog::getText(this, tr("Add callsign to list"),
                                         tr("Enter Callsign:"), QLineEdit::Normal,
                                         ui->sysvar_Call->text() , &ok);
    if (ok && !text.isEmpty()) {

        QString MYCALLS;
        MYCALLS = this->atomGUI->SysVars->getVal( "MYCALLS" );

        qDebug() << "MYCALLS=" << MYCALLS;
        qDebug() << "text=" << text;
        qDebug() << "indexOf=" << MYCALLS.indexOf( text );

        if ( MYCALLS == "" ) {
            this->atomGUI->weSysvarSet( "MYCALLS", text + ";" );
        } else {
            if ( MYCALLS.indexOf( text + ";" ) == -1 ) {
                this->atomGUI->weSysvarSet( "MYCALLS", text + ";" + MYCALLS );
            }
        }


    }

}

void MainWindow::qryDeleteCallButton() {
//удалить позывной из списка

    int ret = QMessageBox::warning(this, tr("CORE Message - Remove Callsign?"),
                                   tr("Remove Callsign?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No );

    if ( ret == QMessageBox::Yes ) {

        if ( ui->qryCallsTableView->currentIndex().row() == -1 ) {

            QMessageBox msgBox;
            msgBox.setText( tr("Choose callsign.") );
            msgBox.exec();

        } else {

            //ui->qryCallsTableView->currentIndex().data( Qt::DisplayRole ).toString()
            QString MYCALLS="";

            int i;
            //перебираем все записи таблицы
            for(i=0;i<(myCallsModel.rowCount());i++) {
                //если позывной в строке не равен удаляемому, то добавляем в список через точку с запятой
                //таким образом будет список позывных через точку с запятой, кроме удаляемого
                if ( myCallsModel.data( myCallsModel.index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() !=
                     ui->qryCallsTableView->currentIndex().data( Qt::DisplayRole ).toString() ) {

                    MYCALLS = MYCALLS + myCallsModel.data( myCallsModel.index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() + ";";
                }
                 //qDebug() << myCallsModel.data( myCallsModel.index( i, 0, QModelIndex()), Qt::DisplayRole ).toString() ;
                qDebug() << "MYCALLS=" << MYCALLS;
            }

            this->atomGUI->weSysvarSet( "MYCALLS", MYCALLS );

        }

    }

}

void MainWindow::weSysvarSet( QString VarName, QString VarVal ) {

    //qDebug() << "MainWindow::weSysvarSet VarName=" << VarName << " VarVal=" << VarVal;
    //LineEdit's
    QList<QLineEdit *> widgets = this->findChildren<QLineEdit *>( "sysvar_" + VarName );
    if ( widgets.count() == 1 ) widgets.at( 0 )->setText( VarVal );

    //CheckBoxes
    QList<QCheckBox *> widgets2 = this->findChildren<QCheckBox *>( "sysvar_" + VarName );
    if ( widgets2.count() == 1 ) widgets2.at( 0 )->setChecked( VarVal=="0" );

    //MYCALLS
    if ( VarName == "MYCALLS" ) {

            QString calls;
            calls = VarVal;

            this->MessengerWindow.toComboBox->clear();
            this->MessengerWindow.toComboBox->addItem( "ALL" );
            this->MessengerWindow.toComboBox->addItem( "ME" );
            this->MessengerWindow.toComboBox->setCurrentIndex( 1 );

            myCallsModel.clear();
            QStringList Labels;
            Labels << tr("Call");
            myCallsModel.setHorizontalHeaderLabels( Labels );
            ui->qryCallsTableView->setColumnWidth( 0, 80 );
            ui->qryCallsTableView->selectRow( 0 );

            qDebug() << "calls=" << calls;

            if ( calls != "" ) {

                while ( calls.indexOf( ";" ) != -1 ) {
                    myCallsModel.insertRow( 0 );
                    myCallsModel.setData( myCallsModel.index( 0, 0 ), calls.left( calls.indexOf( ";" ) ) );

                    this->MessengerWindow.toComboBox->addItem( calls.left( calls.indexOf( ";" ) ) );

                    calls = calls.mid( calls.indexOf( ";" ) + 1 );
                }

            }
            ui->qryCallsTableView->selectRow( 0 );

    }

    //обновление в почтовике
    //
    //путь по умолчанию
    if ( VarName == "DefaultUnproto" ) {

        this->MessengerWindow.patchEdit->setText( VarVal );

    }



}

void MainWindow::weSysportSetParam( int PortNum, QString ParName, QString ParVal ) {

    //qDebug() << "MainWindow::weSysportSetParam ParName=" << ParName << " ParVal=" << ParVal;

    if ( ParName == "PORT_TYPE" ) {

        QString portType;
        QString portImg;

        portType = "n\\a";
        portImg  = ":images/port.png";

        switch ( ParVal.toInt() ) {
            case 0 : portType = "Inet";
                     portImg  = ":images/portinet.png";
                     break;
            case 1 : portType = "KISS";
                     portImg  = ":images/portkiss.png";
                     break;
            case 2 : portType = "AGW";
                     portImg  = ":images/portagw.png";
                     break;
            case 5 : portType = "AXIP";
                     portImg  = ":images/portaxip.png";
                     break;
            case 6 : portType = "FLDIGI";
                     portImg  = ":images/portfldigi.png";
                     break;
            case 7 : portType = "VASKA";
                     portImg  = ":images/portvaska.png";
                     break;
        }

        pModel->setData( pModel->index( PortNum, 0, QModelIndex()), portType );
        //обновляем порты в мессенджере
        this->MessengerWindow.portComboBox->setItemIcon ( PortNum, QIcon( portImg ) );



    }

    if ( ParName == "PORT_NAME" ) {

        pModel->setData( pModel->index( PortNum, 1, QModelIndex()), ParVal );

        //обновляем порты в мессенджере
        QString ptxt;
        if ( ParVal != "" ) ptxt = QString::number( PortNum + 1) + " - " + ParVal;
        this->MessengerWindow.portComboBox->setItemData ( PortNum, ptxt, Qt::DisplayRole );

        //f а это замена имен в модели маяков
        /*
        int i;
        for(i=0;i<10;i++) {
            if ( ( bModel->data( bModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toString() != "" )
                 && ( ( bModel->data( bModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toInt() ) == ( PortNum + 1 ) ) ) {
                //qDebug() << "i=" << i << ( bModel->data( bModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toInt() ) << portNum + 1;
                bModel->setData( bModel->index( i, 2, QModelIndex()), ParVal );
            }
        }*/

    }

}

void MainWindow::weSysportChangeState( int PortNum, QString PortState ) {

    qDebug() << "MainWindow::weSysportChangeState PortNum=" << PortNum << " PortState=" << PortState;


    if ( PortState == "DELETED" ) {
        pModel->setData( pModel->index( PortNum, 0, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 1, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 2, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 3, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 4, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 5, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 6, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 7, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 8, QModelIndex()), "" );
        pModel->setData( pModel->index( PortNum, 9, QModelIndex()), "" );
        this->updPortsButtonState();
    } else {
        pModel->setData( pModel->index( PortNum, 2, QModelIndex()), PortState );
        this->updPortsButtonState ( );
    }


}

void MainWindow::weSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal ) {

    //qDebug() << "MainWindow::weSysbeaconSetParam BcknNum=" << BcknNum << " ParName=" << ParName << " ParVal=" << ParVal;

    if ( ( ParName == "PORT_NUM" ) && ( ParVal != "" ) ) {
        bModel->setData( bModel->index( BcknNum, 1, QModelIndex()), QString::number( ParVal.toInt() + 1 ) );
        bModel->setData( bModel->index( BcknNum, 2, QModelIndex()), pModel->data( pModel->index( ParVal.toInt(), 1, QModelIndex()), Qt::DisplayRole ).toString() );
    }
    if ( ParName == "SYM" )       {
        if ( ParVal=="" ) {
            bModel->setData( bModel->index( BcknNum, 3, QModelIndex()), "" );
            bModel->setData( bModel->index( BcknNum, 4, QModelIndex()), "" );
        } else {
            bModel->setData( bModel->index( BcknNum, 3, QModelIndex()), ParVal );

            bModel->setData( bModel->index( BcknNum, 4, QModelIndex()), this->atomGUI->SysSymbols->getSymName( ParVal ) );
            //bModel->setData( bModel->index( BcknNum, 4, QModelIndex()), symNames->value( ParVal ).toString() );
        }
    }
    if ( ParName == "CALL" )            bModel->setData( bModel->index( BcknNum, 5, QModelIndex()), ParVal );
    if ( ParName == "LAT" )             bModel->setData( bModel->index( BcknNum, 6, QModelIndex()), ParVal );
    if ( ParName == "LNG" )             bModel->setData( bModel->index( BcknNum, 7, QModelIndex()), ParVal );
    if ( ParName == "UNPROTO" )         bModel->setData( bModel->index( BcknNum, 8, QModelIndex()), ParVal );
    if ( ParName == "TEXT" )            bModel->setData( bModel->index( BcknNum, 9, QModelIndex()), ParVal );
    if ( ParName == "INTERVAL" )        bModel->setData( bModel->index( BcknNum,10, QModelIndex()), ParVal );
    if ( ParName == "STATTEXT" )        bModel->setData( bModel->index( BcknNum,11, QModelIndex()), ParVal );
    if ( ParName == "STATINTERVAL" )    bModel->setData( bModel->index( BcknNum,12, QModelIndex()), ParVal );


}

void MainWindow::weSysbeaconChangeState( int BcknNum, QString BcknState ) {

    if ( BcknState == "DELETED" ) {
        bModel->setData( bModel->index( BcknNum, 0, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 1, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 2, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 3, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 4, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 5, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 6, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 7, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 8, QModelIndex()), "" );
        bModel->setData( bModel->index( BcknNum, 9, QModelIndex()), "" );
        this->updBeaconsButtonState();
    } else {
        bModel->setData( bModel->index( BcknNum, 0, QModelIndex()), BcknState );
        this->updBeaconsButtonState();
    }

}

void MainWindow::weportRXPacket( int pNum, bool isRX ) {

    //qDebug() << "MainWindow::weportRXPacket";
    if ( isRX == true ) {
        pModel->setData( pModel->index( pNum, 2, QModelIndex()), "RX" );
        ui->portsTableView->repaint();

        lamps[ pNum ] = 3;
    }

}

void MainWindow::weportTXPacket( int pNum, bool isTX ) {

    //qDebug() << "MainWindow::weportTXPacket";
    if ( isTX == true ) {
        pModel->setData( pModel->index( pNum, 2, QModelIndex()), "TX" );
        ui->portsTableView->repaint();

        lamps[ pNum ] = 3;
    }

}

void MainWindow::setPortParamFFrm( int pNum, QString pName, QString pVal  ) {
//сюда принимаются параметры от форм редактирования параметров портов

    this->atomGUI->weSysportSetParam( pNum, pName, pVal );
    //qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    //qDebug() << "pNum=" << pNum << " pName="<< pName << " pVal=" << pVal;

}
