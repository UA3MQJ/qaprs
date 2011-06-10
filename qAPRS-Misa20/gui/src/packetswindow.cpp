#include "include/packetswindow.h"
#include "../../core/include/version.h"
//
PacketsWindowImpl::PacketsWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{

    setupUi(this);

    pModel = NULL;

    atomGUI = NULL;

    packetsModel.setColumnCount( 10 );

    QStringList Labels;

    //K, DATA, TIME, Port, PNote, TRX, PTo, PFrom, PVia, Message
    Labels << tr("K") << tr("DATA") << tr("TIME") << tr("Port") << tr("PNote") << tr("TRX") << tr("PTo") << tr("PFrom") << tr("PVia") << tr("Message");

    packetsModel.setHorizontalHeaderLabels( Labels );

    this->packetTableView->setModel( &packetsModel );

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

    enableSort = false;

}

void PacketsWindowImpl::showEvent ( QShowEvent * event ) {

    int i;

    qDebug() << "Packets show";

    //восстановление размеров и положения окна
    QSettings settings( versionName, "PacketsWindow" );
    restoreGeometry( settings.value("geometry").toByteArray() );
    restoreState( settings.value("windowState").toByteArray() );

    //восстановление ширины столбцов
    for(i=0;i<=9;i++)
        if ( !settings.value("PacketsWindowColumn" + QString::number( i ) + "Width").isNull() )
            packetTableView->setColumnWidth( i, settings.value("PacketsWindowColumn" + QString::number( i ) + "Width").toInt() );


}

void PacketsWindowImpl::closeEvent(QCloseEvent *event) {

    int i;

    qDebug()<<"Packets Close event";

    //сохранение размеров и положения окна
    QSettings settings( versionName, "PacketsWindow");
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );

    //сохранение ширины столбцов
    for(i=0;i<=9;i++)
        settings.setValue( "PacketsWindowColumn" + QString::number( i ) + "Width", packetTableView->columnWidth( i ) );

    this->hide();
    event->ignore();

}


void PacketsWindowImpl::RXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText ) {

    if ( atomGUI != NULL ) {
        if ( atomGUI->loadingMode == false ) {
            addPacket( "RX", pK, pNum, DT, To, From, Via, MsgText );
        }
    }

}

void PacketsWindowImpl::TXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText ) {

    if ( atomGUI != NULL ) {
        if ( atomGUI->loadingMode == false ) {
            addPacket( "TX", pK, pNum, DT, To, From, Via, MsgText );
        }
    }

}

void PacketsWindowImpl::addPacket( QString TRX, int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText ) {

    QDateTime PacketDateTime;

    PacketDateTime = QDateTime::fromString( DT, "dd.MM.yyyy hh:mm:ss.zzz" );

    //qDebug() << "add row pk =" << pK;

    packetsModel.insertRow( 0 );
    packetsModel.setData( packetsModel.index( 0, 0 ), pK );
    packetsModel.setData( packetsModel.index( 0, 1 ), PacketDateTime.toString( "dd.MM.yyyy" ) );
    packetsModel.setData( packetsModel.index( 0, 2 ), PacketDateTime.toString( "hh:mm:ss" ) );
    if ( pModel != NULL ) packetsModel.setData( packetsModel.index( 0, 3 ), "(" + QString::number( pNum ) + ") " + pModel->data( pModel->index( pNum, 0, QModelIndex()), Qt::DisplayRole ).toString() );

    if ( pModel != NULL ) packetsModel.setData( packetsModel.index( 0, 4 ), pModel->data( pModel->index( pNum, 1, QModelIndex()), Qt::DisplayRole ).toString() );

    packetsModel.setData( packetsModel.index( 0, 5 ), TRX );
    packetsModel.setData( packetsModel.index( 0, 6 ), To );
    packetsModel.setData( packetsModel.index( 0, 7 ), From );
    packetsModel.setData( packetsModel.index( 0, 8 ), Via );
    packetsModel.setData( packetsModel.index( 0, 9 ), MsgText );

    if ( enableSort == true ) packetsModel.sort( 0, Qt::DescendingOrder );

}
