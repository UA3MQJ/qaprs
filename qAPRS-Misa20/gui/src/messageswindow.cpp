#include "include/messageswindow.h"
#include "../../core/include/version.h"
//
MessagesWindowImpl::MessagesWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{

    setupUi(this);

    messagesModel.setColumnCount( 10 );

    QStringList Labels;

    //K, DATA, TIME, Port, PNote, TRX, PFrom, MTo, Message, MSGID
    Labels << tr("K") << tr("DATA") << tr("TIME") << tr("Port") << tr("PNote") << tr("TRX") << tr("MTo") << tr("PFrom") << tr("Message") << tr("MSGID");
    messagesModel.setHorizontalHeaderLabels( Labels );

    messagesTableView->setModel( &messagesModel );

    messagesTableView->setColumnWidth(  0,  50 );
    messagesTableView->setColumnWidth(  1,  70 );
    messagesTableView->setColumnWidth(  2,  55 );
    messagesTableView->setColumnWidth(  3,  50 );
    messagesTableView->setColumnWidth(  4,  60 );
    messagesTableView->setColumnWidth(  5,  30 );
    messagesTableView->setColumnWidth(  6,  70 );
    messagesTableView->setColumnWidth(  7,  70 );
    messagesTableView->setColumnWidth(  8, 230 );
    messagesTableView->setColumnWidth(  9,  50 );

}

void MessagesWindowImpl::showEvent ( QShowEvent * event ) {

    int i;

    qDebug() << "Messages show";

    //восстановление размеров и положения окна
    QSettings settings( versionName, "MessagesWindow" );
    restoreGeometry( settings.value("geometry").toByteArray() );
    restoreState( settings.value("windowState").toByteArray() );

    //восстановление ширины столбцов
    for(i=0;i<=9;i++)
        if ( !settings.value("MessagesWindowColumn" + QString::number( i ) + "Width").isNull() )
            messagesTableView->setColumnWidth( i, settings.value("MessagesWindowColumn" + QString::number( i ) + "Width").toInt() );


}

void MessagesWindowImpl::closeEvent(QCloseEvent *event) {

    int i;

    qDebug()<<"Messages Close event";

    //сохранение размеров и положения окна
    QSettings settings( versionName, "MessagesWindow" );
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );

    //сохранение ширины столбцов
    for(i=0;i<=9;i++)
        settings.setValue( "MessagesWindowColumn" + QString::number( i ) + "Width", messagesTableView->columnWidth( i ) );

    this->hide();
    event->ignore();

}

void MessagesWindowImpl::RXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID ) {
    addMessage( "RX", pNum, K, DateTime, To, From, MsgText, MID );
}


void MessagesWindowImpl::TXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID ) {
    addMessage( "TX", pNum, K, DateTime, To, From, MsgText, MID );
}

void MessagesWindowImpl::addMessage( QString TRX, int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID ) {

    QDateTime PacketDateTime;

    PacketDateTime = QDateTime::fromString( DateTime, "dd.MM.yyyy hh:mm:ss.zzz" );

    messagesModel.insertRow( 0 );
    messagesModel.setData( messagesModel.index( 0, 0 ), K );
    messagesModel.setData( messagesModel.index( 0, 1 ), PacketDateTime.toString( "dd.MM.yyyy" ) );
    messagesModel.setData( messagesModel.index( 0, 2 ), PacketDateTime.toString( "hh:mm:ss" ) );
    if ( pModel != NULL ) messagesModel.setData( messagesModel.index( 0, 3 ), "(" + QString::number( pNum ) + ") " + pModel->data( pModel->index( pNum, 0, QModelIndex()), Qt::DisplayRole ).toString() );
    if ( pModel != NULL ) messagesModel.setData( messagesModel.index( 0, 4 ), pModel->data( pModel->index( pNum, 1, QModelIndex()), Qt::DisplayRole ).toString() );
    messagesModel.setData( messagesModel.index( 0, 5 ), TRX );
    messagesModel.setData( messagesModel.index( 0, 6 ), To );
    messagesModel.setData( messagesModel.index( 0, 7 ), From );
    messagesModel.setData( messagesModel.index( 0, 8 ), MsgText );
    messagesModel.setData( messagesModel.index( 0, 9 ), MID );

    messagesModel.sort( 0, Qt::DescendingOrder );
}



