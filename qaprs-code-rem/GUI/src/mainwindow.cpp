#include "include/mainwindow.h"
#include "include/portoption.h"
#include "include/beaconoption.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyle( "plastique" ); //установить пластиковый стиль
    QApplication::setPalette( QApplication::style()->standardPalette() ); //со стандартной палитрой

    //грузим символы в массивы
    QDomDocument doc("symbols");
    QFile fileConfig("./symbols/symbols.xml");
    if (!fileConfig.open(QIODevice::ReadOnly)) {
        //return 1;
        qDebug() << "symbols.xml open error";
    }
    if (!doc.setContent(&fileConfig)) {
        //return 1;
        qDebug() << "symbols.xml set content error";
    }
    fileConfig.close();

    symNames = new QVariantMap;
    symImgs = new QVariantMap;

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {

            symNames->insert( e.attributes().namedItem("code").nodeValue(), e.attributes().namedItem("name").nodeValue() );
            symImgs->insert( e.attributes().namedItem("code").nodeValue(), e.attributes().namedItem("img").nodeValue() );

        }
        n = n.nextSibling();
    }

    //qDebug() << symNames.value("/I").toString() ;

    //обработчик сигнала STOP от ядра
    connect( &Core, SIGNAL( stop() ),
             this,  SLOT( stopCore() ) );

    Core.Start();

    tcpGUIClient = new QTcpSocket();
    timer = new QTimer(this);

    connect( tcpGUIClient, SIGNAL( connected() ),    this, SLOT( tcpGUIClientconnected() ) );
    connect( tcpGUIClient, SIGNAL( disconnected() ), this, SLOT( tcpGUIClientconnected() ) );

    connect( tcpGUIClient, SIGNAL( error( QAbstractSocket::SocketError ) ),
             this,         SLOT( tcpGUIClienterror( QAbstractSocket::SocketError ) ) );

    connect( tcpGUIClient, SIGNAL( readyRead() ), this, SLOT( tcpGUIClientread() ) );

    CoreAddress = "127.0.0.1";
    CorePort    = "5000";

    allmsg.clear();

    tcpGUIClient->connectToHost( CoreAddress, CorePort.toInt() );

    connect( ui->saveButton,         SIGNAL( clicked() ),
             this,                   SLOT( saveButtonClick() ) );

    connect( ui->cancelButton,       SIGNAL( clicked() ),
             this,                   SLOT( cancelButtonClick() ) );

    connect( ui->closeButton,        SIGNAL( clicked() ),
             this,                   SLOT( closeButtonClick() ) );

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

    connect( ui->editBeaconButton,   SIGNAL( clicked() ),
             this,                   SLOT( editBeaconButtonClick() ) );

    connect( ui->onBeaconButton,     SIGNAL( clicked() ),
             this,                   SLOT( onBeaconButtonClick() ) );

    connect( ui->offBeaconButton,    SIGNAL( clicked() ),
             this,                   SLOT( offBeaconButtonClick() ) );

    connect( ui->deleteBeaconButton, SIGNAL( clicked() ),
             this,                   SLOT( deleteBeaconButtonClick() ) );

    connect( ui->onAllBeaconsButton, SIGNAL( clicked() ),
             this,                   SLOT( onAllBeaconsButtonClick() ) );

    connect( ui->offPortsButton,     SIGNAL( clicked() ),
             this,                   SLOT( offPortsButtonClick() ) );

    connect( ui->createBeaconButton, SIGNAL( clicked() ),
             this,                   SLOT( createBeaconButtonClick() ) );

    connect( ui->portsTableView,     SIGNAL( doubleClicked ( QModelIndex ) ),
             this,                   SLOT( onPortDoubleClicked ( QModelIndex ) ) );

    connect( ui->beaconsTableView,   SIGNAL( doubleClicked ( QModelIndex ) ),
             this,                   SLOT( onBeaconDoubleClicked ( QModelIndex ) ) );




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

    //модель с маяками
    bModel = new BeaconsModel(256, 11, parent);

    bModel->symNames = symNames;
    bModel->symImgs  = symImgs;

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



    connect( timer, SIGNAL( timeout() ), this, SLOT( onTimer() ) );

    timer->start( 500 );

}

//слот, в который помещаем все логи
void MainWindow::ToLog( QString Text )
{
    //ui->logEdit->insertHtml( (QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss") + " " + Text );
    //ui->logEdit->verticalScrollBar()->setSliderPosition( ui->logEdit->verticalScrollBar()->maximum() );
}

//обработчик сигнала STOP от ядра
void MainWindow::stopCore()
{
    this->close();
}

void MainWindow::tcpGUIClientconnected () {

    int i;

    QByteArray tmp;

    qDebug() << "tcpGUIClientconnected";

    //запрашиваем системные переменные
    tcpGUIClient->write( "sysvar set\r\n" );

    //запрашиваем системные порты
    //tcpGUIClient->write( "SYSPORT\r\n" );

    //получить статус портов и маяков
    tmp.clear();
    for(i=0;i<256;i++) {
        tmp.append( "SYSPORT PORT_NUM=" + QString::number( i ) + " GET STATE\r\n" );
        tmp.append( "SYSPORT PORT_NUM=" + QString::number( i ) + " SET PORT_NAME\r\n" );
        tmp.append( "SYSPORT PORT_NUM=" + QString::number( i ) + " SET PORT_TYPE\r\n" );

        tmp.append( "SYSBEACON BCN_NUM=" + QString::number( i ) + " GET STATE\r\n" );
        tmp.append( "SYSBEACON BCN_NUM=" + QString::number( i ) + " SET\r\n" );

    }
    tcpGUIClient->write( tmp );


}

void MainWindow::tcpGUIClientdisconnected () {

    qDebug() << "tcpGUIClientdisconnected";

}

void MainWindow::tcpGUIClienterror (QAbstractSocket::SocketError socketError){

    qDebug() << "tcpGUIClienterror";

}

void MainWindow::tcpGUIClientread(){

   // qDebug() << "tcpGUIClientread";

    QTextCodec* decoder = QTextCodec::codecForName("UTF-8");

    QByteArray msg;
    QByteArray cmd;

    int i;

    //log->insertHtml( "(" + QString::number( PortNum ) + ")" + PortType + "::<font color='#008800'>RX Packet</font><br>");

    //while (tcpGUIClient->bytesAvailable()==0);
    msg = tcpGUIClient->readAll();

    allmsg.append( msg );

    if ( allmsg.right( 2 ) == "\r\n" ) {

        while ( allmsg.length() != 0) {

            cmd = allmsg.left( allmsg.indexOf("\r") );

            if ( cmd.length() != 0) {

                //qDebug() << "res=" << cmd ; //обработать результат

                if ( cmd.left(10)=="SYSVAR SET" ) {
                    //если пришла в результате системная переменная, то выведем ее в едиты
                    QString varname = cmd.split(' ').at(2).split('=').at(0);
                    QString varval  = decoder->toUnicode( cmd.mid( cmd.indexOf( "=" ) + 1 ) );

                    if ( varname == "Call" )        ui->callEdit->setText( varval );
                    if ( varname == "Name" )        ui->nameEdit->setText( varval );
                    if ( varname == "QTHN" )        ui->qthnameEdit->setText( varval );
                    if ( varname == "Lat" )         ui->latEdit->setText( varval );
                    if ( varname == "Lng" )         ui->lngEdit->setText( varval );
                    if ( varname == "Unproto" )     ui->unprotoEdit->setText( varval );
                    if ( varname == "Beacon" )      ui->beacontextEdit->setText( varval );
                    if ( varname == "Symbol" )      ui->symbolEdit->setText( varval );
                    if ( varname == "APRSCall" )    ui->APRSCallEdit->setText( varval );

                }

                //обработка set причем только PORT_NAME и PORT_TYPE
                QStringList parsedcmd = ((QString)(cmd)).split(' ') ;

                if ( parsedcmd.at( 0 ) == "SYSPORT" ) {

                    if ( parsedcmd.at( 1 ).toUpper().split('=').at( 0 ) ==  "PORT_NUM" ) {

                        int portNum = parsedcmd.at( 1 ).toUpper().split('=').at( 1 ).toInt();

                        if ( parsedcmd.at( 2 ) == "SET" ) {

                            QString par_name = cmd.mid( cmd.indexOf( "SET" ) + 4, cmd.indexOf("=", cmd.indexOf("=") + 1 ) - cmd.indexOf( "SET" ) - 4 );
                            QString par_value = decoder->toUnicode( cmd.mid( cmd.indexOf("=", cmd.indexOf("=") + 1 ) + 1 ) );

                            //qDebug() << "par_name=" << par_name;
                            //qDebug() << "par_value=" << par_value;

                            if ( par_name == "PORT_TYPE" ) pModel->setData( pModel->index( portNum, 0, QModelIndex()), par_value );
                            if ( par_name == "PORT_NAME" ) {

                                pModel->setData( pModel->index( portNum, 1, QModelIndex()), par_value );

                                for(i=0;i<10;i++) {
                                    if ( ( bModel->data( bModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toString() != "" )
                                         and ( ( bModel->data( bModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toInt() ) == ( portNum + 1 ) ) ) {
                                        //qDebug() << "i=" << i << ( bModel->data( bModel->index( i, 1, QModelIndex()), Qt::DisplayRole ).toInt() ) << portNum + 1;
                                        bModel->setData( bModel->index( i, 2, QModelIndex()), par_value );
                                    }
                                }

                            }

                            emit RCVPortParam( portNum, par_name, par_value );

                        }

                    }

                }

                if ( parsedcmd.at( 0 ) == "SYSBEACON" ) {

                    //qDebug() << "SYSBEACON" << parsedcmd;

                   if ( parsedcmd.at( 1 ).toUpper().split('=').at( 0 ) ==  "BCN_NUM" ) {

                        int bcknNum = parsedcmd.at( 1 ).toUpper().split('=').at( 1 ).toInt();

                        //qDebug() << "SYSBEACON BCN_NUM=" << bcknNum;

                        if ( parsedcmd.at( 2 ) == "SET" ) {

                            QString par_name = cmd.mid( cmd.indexOf( "SET" ) + 4, cmd.indexOf("=", cmd.indexOf("=") + 1 ) - cmd.indexOf( "SET" ) - 4 );
                            QString par_value = decoder->toUnicode( cmd.mid( cmd.indexOf("=", cmd.indexOf("=") + 1 ) + 1 ) );

                            //qDebug() << "par_name=" << par_name;
                            //qDebug() << "par_value=" << par_value;

                            if ( ( par_name == "PORT_NUM" ) and ( par_value != "" ) ) {
                                bModel->setData( bModel->index( bcknNum, 1, QModelIndex()), QString::number( par_value.toInt() + 1 ) );
                                bModel->setData( bModel->index( bcknNum, 2, QModelIndex()), pModel->data( pModel->index( par_value.toInt(), 1, QModelIndex()), Qt::DisplayRole ).toString() );
                            }
                            if ( par_name == "SYM" )       {
                                if ( par_value=="" ) {
                                    bModel->setData( bModel->index( bcknNum, 3, QModelIndex()), "" );
                                    bModel->setData( bModel->index( bcknNum, 4, QModelIndex()), "" );
                                } else {
                                    bModel->setData( bModel->index( bcknNum, 3, QModelIndex()), par_value );
                                    bModel->setData( bModel->index( bcknNum, 4, QModelIndex()), symNames->value( par_value ).toString() );
                                }
                            }
                            if ( par_name == "CALL" )      bModel->setData( bModel->index( bcknNum, 5, QModelIndex()), par_value );
                            if ( par_name == "LAT" )       bModel->setData( bModel->index( bcknNum, 6, QModelIndex()), par_value );
                            if ( par_name == "LNG" )       bModel->setData( bModel->index( bcknNum, 7, QModelIndex()), par_value );
                            if ( par_name == "UNPROTO" )   bModel->setData( bModel->index( bcknNum, 8, QModelIndex()), par_value );
                            if ( par_name == "TEXT" )      bModel->setData( bModel->index( bcknNum, 9, QModelIndex()), par_value );
                            if ( par_name == "INTERVAL" )  bModel->setData( bModel->index( bcknNum,10, QModelIndex()), par_value );

                            emit RCVBeaconParam( bcknNum, par_name, par_value );
                        }

                    }

                }


                //события

                if ( cmd.left(24)=="EVENT RXPACKET PORT_NUM=" ) {
                    //если пришла информация о портах
                    QByteArray tcmd;
                    int portNum;

                    tcmd = cmd.mid( 24 ); //отрезаем "SYSPORT port_num="
                    //qDebug() << tcmd.left( tcmd.indexOf( ";" ) );
                    portNum = tcmd.left( tcmd.indexOf( ";" ) ).toInt();

                    tcmd = tcmd.mid( tcmd.indexOf( ";" ) + 2 ); //отрезаем по следующий параметр
                    //qDebug() << tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) ;

                    if ( tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) == "TRUE" ) {
                        pModel->setData( pModel->index( portNum, 2, QModelIndex()), "RX" );
                        ui->portsTableView->repaint();
                    }// else {
                    //    model->setData( model->index( portNum, 2, QModelIndex()), "0" );
                    //}
                    //qDebug() << "EVENT RXPACKET port_num=" << portNum << tcmd.left( tcmd.indexOf( ";" ) ) << tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 );

                    //timer->start( 125 );

                }

                if ( cmd.left(24)=="EVENT TXPACKET PORT_NUM=" ) {
                    //если пришла информация о портах
                    QByteArray tcmd;
                    int portNum;

                    tcmd = cmd.mid( 24 ); //отрезаем "SYSPORT port_num="
                    //qDebug() << tcmd.left( tcmd.indexOf( ";" ) );
                    portNum = tcmd.left( tcmd.indexOf( ";" ) ).toInt();

                    tcmd = tcmd.mid( tcmd.indexOf( ";" ) + 2 ); //отрезаем по следующий параметр
                    //qDebug() << tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) ;

                    if ( tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) == "TRUE" ) {
                        pModel->setData( pModel->index( portNum, 2, QModelIndex()), "TX" );
                        ui->portsTableView->repaint();
                    }// else {
                    //    model->setData( model->index( portNum, 2, QModelIndex()), "0" );
                    //}
                    //qDebug() << "EVENT TXPACKET port_num=" << portNum << tcmd.left( tcmd.indexOf( ";" ) ) << tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 );

                    //timer->start( 125 );

                }

                //EVENT CHANGESTATE PORT_NUM=0; STATUS=ON;
                if ( cmd.left(27)=="EVENT CHANGESTATE PORT_NUM=" ) {

                    QByteArray tcmd;
                    int portNum;

                    tcmd = cmd.mid( 27 ); //отрезаем "SYSPORT port_num="
                    //qDebug() << tcmd.left( tcmd.indexOf( ";" ) );
                    portNum = tcmd.left( tcmd.indexOf( ";" ) ).toInt();

                    tcmd = tcmd.mid( tcmd.indexOf( ";" ) + 2 ); //отрезаем по следующий параметр
                    //qDebug() << tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) ;

                    //pModel->setData( pModel->index( portNum, 2, QModelIndex()), tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) );
                    if ( tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) == "DELETED" ) {
                        pModel->setData( pModel->index( portNum, 0, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 1, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 2, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 3, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 4, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 5, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 6, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 7, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 8, QModelIndex()), "" );
                        pModel->setData( pModel->index( portNum, 9, QModelIndex()), "" );
                    } else {
                        pModel->setData( pModel->index( portNum, 2, QModelIndex()), tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) );
                    }
                }

                //qDebug() << cmd;
                //EVENT CHANGESTATE BCN_NUM=0; STATUS=ON;
                if ( cmd.left(26)=="EVENT CHANGESTATE BCN_NUM=" ) {

                    //qDebug() << cmd;

                    QByteArray tcmd;
                    int beaconNum;

                    tcmd = cmd.mid( 26 );
                    beaconNum = tcmd.left( tcmd.indexOf( ";" ) ).toInt();

                    tcmd = tcmd.mid( tcmd.indexOf( ";" ) + 2 ); //отрезаем по следующий параметр
                    //qDebug() << tcmd;
                    //qDebug() << tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) ;

                    if ( tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) == "DELETED" ) {
                        bModel->setData( bModel->index( beaconNum, 0, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 1, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 2, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 3, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 4, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 5, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 6, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 7, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 8, QModelIndex()), "" );
                        bModel->setData( bModel->index( beaconNum, 9, QModelIndex()), "" );
                    } else {
                        bModel->setData( bModel->index( beaconNum, 0, QModelIndex()), tcmd.mid( tcmd.indexOf( "=" ) + 1, tcmd.indexOf( ";" ) - tcmd.indexOf( "=" ) - 1 ) );
                    }



                }


            }

            allmsg = allmsg.mid( allmsg.indexOf("\r") + 2 ); //отрезать

        }

    }

    updPortsButtonState();
    updBeaconsButtonState();

}

//установить значение переменной БД
void MainWindow::setDBValue( QString varname, QString varval ) {

    QByteArray cmd;

    QTextCodec* decoder = QTextCodec::codecForName("UTF-8");

    cmd.append( "sysvar set " + varname + "=" + decoder->fromUnicode( varval ) + "\r\n");
    tcpGUIClient->write( cmd );

}

void MainWindow::onTimer() {

    int i;
    //Если лампочки горят, то выключаем их
    for(i=0;i<256;i++) {
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

void MainWindow::saveButtonClick(){

    setDBValue( "Call", ui->callEdit->text() );
    setDBValue( "Name", ui->nameEdit->text() );
    setDBValue( "QTHN", ui->qthnameEdit->text() );
    setDBValue( "Lat", ui->latEdit->text() );
    setDBValue( "Lng", ui->lngEdit->text() );
    setDBValue( "Unproto", ui->unprotoEdit->text() );
    setDBValue( "Beacon", ui->beacontextEdit->text() );
    setDBValue( "Symbol", ui->symbolEdit->text() );
    setDBValue( "APRSCall", ui->APRSCallEdit->text() );

}

void MainWindow::cancelButtonClick(){

    //запросить значения переменных заново
    tcpGUIClient->write( "sysvar set\r\n" );

}

void MainWindow::closeButtonClick(){

    this->close();

}

void MainWindow::openPortsButtonClick(){

    //запросить открытие всех портов
    tcpGUIClient->write( "SYSPORT ALL OPEN\r\n" );

}


void MainWindow::closePortsButtonClick(){

    //запросить закрытие всех портов
    tcpGUIClient->write( "SYSPORT ALL CLOSE\r\n" );

}

void MainWindow::openPortButtonClick() {

    QByteArray cmd;

    //запросить открытие порта
    cmd.append( "SYSPORT PORT_NUM=" );
    cmd.append( QString::number( ui->portsTableView->currentIndex().row() ) );
    cmd.append( " OPEN\r\n" );

    tcpGUIClient->write( cmd );

}

void MainWindow::closePortButtonClick() {

    QByteArray cmd;

    //запросить закрытие порта
    cmd.append( "SYSPORT PORT_NUM=" );
    cmd.append( QString::number( ui->portsTableView->currentIndex().row() ) );
    cmd.append( " CLOSE\r\n" );

    tcpGUIClient->write( cmd );

    //qDebug() << "SYSPORT PORT_NUM=" << ui->portsTableView->currentIndex().row() << "CLOSE";

}

void MainWindow::deletePortButtonClick() {

    QByteArray cmd;

    int ret = QMessageBox::warning(this, tr("CORE Message - Remove Port?"),
                                   tr("At port removal all its parametres from a system database leave\n"
                                      "Remove Port?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No );

    if ( ret == QMessageBox::Yes ) {

        //запросить удаление порта
        cmd.append( "SYSPORT PORT_NUM=" );
        cmd.append( QString::number( ui->portsTableView->currentIndex().row() ) );
        cmd.append( " DELETE\r\n" );

        tcpGUIClient->write( cmd );

    }

}

void MainWindow::editPortButtonClick() {

    PortoptionDialog *win;

    win = new PortoptionDialog();

    win->portNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->portNumber = ui->portsTableView->currentIndex().row();

    win->portTypeBox->setCurrentIndex( 0 );
    win->stackedWidget->setCurrentIndex( 0 );

    //отправлять все принятые от ядра параметры портов - диалогу параметров порта
    connect(this, SIGNAL(RCVPortParam( int, QString, QString )),
            win, SLOT(RCVPortParam( int, QString, QString )));

    //отправлять ядру все установленные на диалоге параметры
    connect(win, SIGNAL(setPortParam( int, QString, QString )),
            this, SLOT(setPortParamFFrm( int, QString, QString )));

    QByteArray cmd;

    cmd.append( "SYSPORT PORT_NUM=" );
    cmd.append( QString::number( ui->portsTableView->currentIndex().row() ) );
    cmd.append( " SET\r\n" );

    tcpGUIClient->write( cmd );

    win->show();

}

void MainWindow::createPortButtonClick() {

    PortoptionDialog *win;

    win = new PortoptionDialog();

    win->portNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->portNumber = ui->portsTableView->currentIndex().row();

    win->portTypeBox->setCurrentIndex( 0 );
    win->stackedWidget->setCurrentIndex( 0 );

    win->userEdit->setText( ui->callEdit->text() );
    win->passEdit->setText( "-1" );

    //отправлять все принятые от ядра параметры портов - диалогу параметров порта
    connect(this, SIGNAL(RCVPortParam( int, QString, QString )),
            win, SLOT(RCVPortParam( int, QString, QString )));

    //отправлять ядру все установленные на диалоге параметры
    connect(win, SIGNAL(setPortParam( int, QString, QString )),
            this, SLOT(setPortParamFFrm( int, QString, QString )));

    win->show();

}

//сюда принимаются установленные параметры от окошек редактирования параметров портов
void MainWindow::setPortParamFFrm( int pNum, QString pName, QString pVal  ) {

    //qDebug() << "pNum = " << pNum << " pName = " << pName << " pVal = " << pVal;

    //this->setPortParam( pNum, pName, pVal );

    QTextCodec* coder = QTextCodec::codecForName("UTF-8");

    QByteArray cmd;
    cmd.append( "SYSPORT PORT_NUM=" );
    cmd.append( QString::number( pNum ) );
    cmd.append( " SET " + pName + "=" + coder->fromUnicode( pVal ) + "\r\n" );


    //qDebug() << cmd;
    tcpGUIClient->write( cmd );

}


void MainWindow::onBeaconButtonClick() {

    QByteArray cmd;

    cmd.append( "SYSBEACON BCN_NUM=" );
    cmd.append( QString::number( ui->beaconsTableView->currentIndex().row() ) );
    cmd.append( " OPEN\r\n" );

    tcpGUIClient->write( cmd );

}

void MainWindow::offBeaconButtonClick() {

    QByteArray cmd;

    cmd.append( "SYSBEACON BCN_NUM=" );
    cmd.append( QString::number( ui->beaconsTableView->currentIndex().row() ) );
    cmd.append( " CLOSE\r\n" );

    tcpGUIClient->write( cmd );


}

void MainWindow::deleteBeaconButtonClick() {

    QByteArray cmd;

    int ret = QMessageBox::warning(this, tr("CORE Message - Remove Beacon?"),
                                   tr("At beacon removal all its parametres from a system database leave\n"
                                      "Remove Beacon?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No );

    if ( ret == QMessageBox::Yes ) {

        cmd.append( "SYSBEACON BCN_NUM=" + QString::number( ui->beaconsTableView->currentIndex().row() ) + " DELETE\r\n" );

        tcpGUIClient->write( cmd );

    }

}

void MainWindow::onAllBeaconsButtonClick(){

    //запросить открытие всех маяков
    tcpGUIClient->write( "SYSBEACON ALL OPEN\r\n" );

}

void MainWindow::offPortsButtonClick(){

    //запросить закрытие всех маяков
    tcpGUIClient->write( "SYSBEACON ALL CLOSE\r\n" );

}

//сюда принимаются установленные параметры от окошек редактирования параметров маяков
void MainWindow::setBeaconParamFFrm( int bNum, QString pName, QString pVal  ) {

    //qDebug() << "pNum = " << bNum << " pName = " << pName << " pVal = " << pVal;

    QTextCodec* coder = QTextCodec::codecForName("UTF-8");

    QByteArray cmd;
    cmd.append( "SYSBEACON BCN_NUM=" );
    cmd.append( QString::number( bNum ) );
    cmd.append( " SET " + pName + "=" + coder->fromUnicode( pVal ) + "\r\n" );

    tcpGUIClient->write( cmd );

}

void MainWindow::editBeaconButtonClick() {

    BeaconoptionDialog *win;
    QString portSym;

    win = new BeaconoptionDialog();

    win->bckNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->beaconNumber = ui->beaconsTableView->currentIndex().row();

    int i;
    for(i=0;i<( symNames->count() / 2 );i++) {

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i ).toString(),
                                 symNames->keys().at( i ) );

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i + ( symNames->count() / 2 ) ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i + ( symNames->count() / 2 ) ).toString(),
                                 symNames->keys().at( i + ( symNames->count() / 2 ) ) );

    }

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


    connect(this, SIGNAL(RCVBeaconParam( int, QString, QString )),
            win,    SLOT(RCVBeaconParam( int, QString, QString )));

    connect(win, SIGNAL(setBeaconParam( int, QString, QString )),
            this,  SLOT(setBeaconParamFFrm( int, QString, QString )));

    QByteArray cmd;
    cmd.append( "SYSBEACON BCN_NUM=" + QString::number( ui->beaconsTableView->currentIndex().row() ) + " SET\r\n" );
    tcpGUIClient->write( cmd );

    win->show();

}

void MainWindow::createBeaconButtonClick() {

    BeaconoptionDialog *win;
    QString portSym;

    win = new BeaconoptionDialog();

    win->bckNumEdit->setText( QString::number( ui->portsTableView->currentIndex().row() + 1 ) );
    win->beaconNumber = ui->beaconsTableView->currentIndex().row();

    win->bckLatEdit->setText( ui->latEdit->text() );
    win->bckLngEdit->setText( ui->lngEdit->text() );

    win->bckUnprotoEdit->setText( ui->unprotoEdit->text() );
    win->bckTextEdit->setText( ui->beacontextEdit->text() );

    win->bckCallEdit->setText( ui->callEdit->text() );
    win->bckIntervalEdit->setText( "-1" );

    int i;
    for(i=0;i<( symNames->count() / 2 );i++) {

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i ).toString(),
                                 symNames->keys().at( i ) );

        win->bckSymBox->addItem( QIcon( symImgs->values().at( i + ( symNames->count() / 2 ) ).toString().prepend("./symbols/") ),
                                 symNames->values().at( i + ( symNames->count() / 2 ) ).toString(),
                                 symNames->keys().at( i + ( symNames->count() / 2 ) ) );

    }

    win->bckSymBox->setCurrentIndex(  win->bckSymBox->findData( ui->symbolEdit->text() ) );

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

    connect(this, SIGNAL(RCVBeaconParam( int, QString, QString )),
            win,    SLOT(RCVBeaconParam( int, QString, QString )));

    connect(win, SIGNAL(setBeaconParam( int, QString, QString )),
            this,  SLOT(setBeaconParamFFrm( int, QString, QString )));

    win->show();

}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::portsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous ) {

    this->updPortsButtonState();

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

}

void MainWindow::beaconsCurrentChanged( const QModelIndex & current, const QModelIndex & previous ) {

    this->updBeaconsButtonState();

}

void MainWindow::updBeaconsButtonState ( ) {

    if ( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "" ) {

        ui->onBeaconButton->setEnabled( false );
        ui->offBeaconButton->setEnabled( false );
        ui->deleteBeaconButton->setEnabled( false );
        ui->editBeaconButton->setEnabled( false );
        ui->sendBeaconButton->setEnabled( false );
        ui->createBeaconButton->setEnabled( true );

    } else {

        ui->onBeaconButton->setEnabled( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "OFF" );
        ui->offBeaconButton->setEnabled( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() != "OFF" );
        ui->sendBeaconButton->setEnabled( bModel->data( bModel->index( ui->beaconsTableView->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole ).toString() != "OFF" );
        ui->deleteBeaconButton->setEnabled( true );
        ui->editBeaconButton->setEnabled( true );
        ui->createBeaconButton->setEnabled( false );

    }

}

void MainWindow::onPortDoubleClicked ( const QModelIndex & index ) {

    if ( ui->editPortButton->isEnabled() == true ) editPortButtonClick();

}

void MainWindow::onBeaconDoubleClicked ( const QModelIndex & index ) {

    if ( ui->editBeaconButton ->isEnabled() == true ) editBeaconButtonClick();

}

