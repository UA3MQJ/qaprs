#include "include/messengerwindow.h"
//
MessengerWindowImpl::MessengerWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{

    setupUi(this);

    SysStations = NULL;
    SysPackets  = NULL;
    SysVars     = NULL;
    atomGUI     = NULL;


    //список "от кого"
    stationsModel.setColumnCount( 1 );
    QStringList Labels;
    Labels << tr("Call");
    stationsModel.setHorizontalHeaderLabels( Labels );
    callsTableView->setModel( &stationsModel );
    callsTableView->setColumnWidth(  0,  90 );

    messagesTableView->setModel( &messagesModel );


    //список портов
    int i;
    for(i=0;i<256;i++) {

        portComboBox->addItem( QIcon( ":images/port.png" ), QString::number( i + 1 ) , i );

    }

    updateMessageList();
    toFromChanged();
    msgTextChanged();


    connect( this->toComboBox, SIGNAL( currentIndexChanged( int ) ),
             this,    SLOT( updateMessageList() ) );

    connect( callsTableView->selectionModel(),
             SIGNAL( currentRowChanged ( const QModelIndex &, const QModelIndex & ) ) ,
             this,
             SLOT( callCurrentChanged ( const QModelIndex &, const QModelIndex & ) ) );


    connect( this->msgToEdit,   SIGNAL( textChanged( QString ) ),
             this,              SLOT( toFromChanged() ) );
    connect( this->msgFromEdit, SIGNAL( textChanged( QString ) ),
             this,              SLOT( toFromChanged() ) );
    connect( this->textEdit,    SIGNAL( textChanged( QString ) ),
             this,              SLOT( msgTextChanged() ) );

    connect( this->sendButton,  SIGNAL( clicked() ),
             this,              SLOT( msgSend() ) );



}

void MessengerWindowImpl::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MessengerWindowImpl::showEvent( QShowEvent *event ) {

    //восстановление размеров и положения окна
    QSettings settings( versionName, "MessengerWindow" );
    restoreGeometry( settings.value("geometry").toByteArray() );
    restoreState( settings.value("windowState").toByteArray() );



}

void MessengerWindowImpl::closeEvent(QCloseEvent *event) {

    int i;

    //сохранение размеров и положения окна
    QSettings settings( versionName, "MessengerWindow");
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );

    //сохранение ширины столбцов
    for(i=0;i<=5;i++)
        settings.setValue( "MessengerMsgColumn" + QString::number( i ) + "Width", messagesTableView->columnWidth( i ) );


    hide();
    event->ignore();

}

void MessengerWindowImpl::hideEvent ( QHideEvent * event ) {

}

void MessengerWindowImpl::contextMenuEvent(QContextMenuEvent *event) {

}

void MessengerWindowImpl::RXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID ) {

    updateMessageList();

}

void MessengerWindowImpl::TXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID ) {

    updateMessageList();

}

void MessengerWindowImpl::callCurrentChanged( const QModelIndex & current, const QModelIndex & previous ) {

    updateMessageList();

}

void MessengerWindowImpl::toFromChanged() {

    sendButton->setEnabled( ((msgToEdit->text()!="") && (msgFromEdit->text()!="")) );

}

void MessengerWindowImpl::msgTextChanged() {

    int msgLen = textEdit->text().length();

    label_5->setText( tr( "Text (%1)" ).arg( msgLen ) );

    if ( msgLen > 67 ) {
        label_5->setText( label_5->text() + " " + QString::number( (int)( msgLen / 67 ) + 1) +tr(" msgs") );
    }

}

void MessengerWindowImpl::msgSend() {

    if ( this->atomGUI != NULL ) {

        int portNum;
        QString To;
        QString From;
        QString Via;
        QString MsgText;

        portNum = portComboBox->currentIndex();
        To      = msgToEdit->text().toUpper();
        From    = msgFromEdit->text().toUpper();
        MsgText = textEdit->text();
        Via     = patchEdit->text();

        this->atomGUI->weSendMessage( portNum, To, From, Via, MsgText, ACKcheckBox->isChecked() );

    }

}

void MessengerWindowImpl::updateStationList() {

    if ( SysStations != NULL ) {

        //SysStations->stationCount
        stationsModel.setRowCount( SysStations->stationCount );

        int i;
        QAPRSStation *tmp;

        i = 0;
        tmp = SysStations->firstStation;
        while ( tmp != NULL ) {
            stationsModel.setData( stationsModel.index( i, 0 ), tmp->call );


            i = i + 1;
            tmp = tmp->nextStation;
        }

        stationsModel.sort( 0 );

        stationsModel.insertRow( 0 );
        stationsModel.setData( stationsModel.index( 0, 0 ), "ALL" );

        callsTableView->selectRow( 0 );

    }

}

void MessengerWindowImpl::updateMessageList() {

    int i;

    qDebug() << "MessengerWindowImpl::updateMessageList()";

    messagesModel.clear();

    messagesModel.setColumnCount( 5 );
    QStringList Labels2;
    Labels2 << tr("Data") << tr("Port") << tr("To") << tr("From") << tr("Message") << tr("ID");
    messagesModel.setHorizontalHeaderLabels( Labels2 );
    messagesTableView->setColumnWidth( 0,  80 );
    messagesTableView->setColumnWidth( 1,  30 );
    messagesTableView->setColumnWidth( 2,  65 );
    messagesTableView->setColumnWidth( 3,  65 );
    messagesTableView->setColumnWidth( 4,  190 );
    messagesTableView->setColumnWidth( 5,  30 );

    //восстановление ширины столбцов
    QSettings settings( versionName, "MessengerWindow");

    for(i=0;i<=5;i++)
        if ( !settings.value("MessengerMsgColumn" + QString::number( i ) + "Width").isNull() )
            messagesTableView->setColumnWidth( i, settings.value("MessengerMsgColumn" + QString::number( i ) + "Width").toInt() );

    QString filter_to;
    QString filter_from;

    filter_to = toComboBox->currentText(); //ALL, ME или конкретные позывные
    filter_from = stationsModel.data( stationsModel.index( callsTableView->currentIndex().row() , 0, QModelIndex() ) , Qt::DisplayRole ).toString();


    if (( filter_to != "ME") && ( filter_to != "ALL")) {
        msgFromEdit->setText( filter_to );
    } else {
        msgFromEdit->setText( "" );
    }

    if ( filter_from != "ALL") {
        msgToEdit->setText( filter_from );
    } else {
        msgToEdit->setText( "" );
    }


    if ( ( SysPackets != NULL ) && ( SysVars != NULL ) ) {

        QAPRSPacket *tmp;

        tmp = this->SysPackets->last;

        while ( tmp != NULL ) {

            if ( tmp->Message.left( 1 ) == ":" ) {

                QString MTo;
                QString MMsg;
                QString MID;

                MTo = tmp->Message.mid(1, 9).trimmed();
                if ( tmp->Message.indexOf("{")==-1 ) {
                    //пакет не имеет ID
                    MMsg = tmp->Message.mid(11);
                    MID = "";
                } else {
                    MMsg = tmp->Message.mid(11, tmp->Message.length() - 11 - ( tmp->Message.length() - tmp->Message.indexOf("{") ));
                    MID = tmp->Message.mid( tmp->Message.indexOf("{")+1 );
                }


                QString tstr;

                bool f1;


                f1 = false;

                //qDebug() << SysVars->getVal( "MYCALLS" );

                if ( ( filter_to == "ALL" ) && ( filter_from == "ALL" ) ) {
                    f1 = true;
                }

                if ( ( filter_to == "ALL" ) && ( filter_from != "ALL" ) ) {
                    if ( ( filter_from == MTo ) || ( filter_to == tmp->PFrom ) ) f1 = true;
                }

                if ( !( ( filter_to == "ME" ) || ( filter_to == "ALL" ) ) && ( filter_from == "ALL" ) ) {
                    //это значит определенный калл со всеми
                   //если или то или фром равено указанному, то письмо подходит

                    if ( ( filter_to == MTo ) || ( filter_to == tmp->PFrom ) ) f1 = true;

                }
                
                if ( !( ( filter_to == "ME" ) || ( filter_to == "ALL" ) ) && ( filter_from != "ALL" ) ) {
                    //это значит определенный калл с кем то конкретным

                    if ( ( filter_to == MTo ) && ( filter_from == tmp->PFrom ) ) f1 = true;
                    if ( ( filter_to == tmp->PFrom ) && ( filter_from == MTo ) ) f1 = true;

                }

                if ( ( filter_to == "ME" ) && ( filter_from == "ALL" ) ) {

                    if ( SysVars->getVal( "MYCALLS" ).indexOf( MTo + ";" ) >= 0 ) f1 = true;
                    if ( SysVars->getVal( "MYCALLS" ).indexOf( tmp->PFrom + ";" ) >= 0 ) f1 = true;

                }

                if ( ( filter_to == "ME" ) && ( filter_from != "ALL" ) ) {

                    if ( ( SysVars->getVal( "MYCALLS" ).indexOf( MTo + ";" ) >= 0 ) && ( tmp->PFrom == filter_from ) ) f1 = true;
                    if ( ( SysVars->getVal( "MYCALLS" ).indexOf( tmp->PFrom + ";" ) >= 0 ) && ( MTo == filter_from ) ) f1 = true;

                }


                if ( ( f1 == true ) ) {

                    messagesModel.insertRow( 0 );
                    messagesModel.setData( messagesModel.index( 0, 0 ), tmp->DT );
                    messagesModel.setData( messagesModel.index( 0, 1 ), tmp->Port_Num );
                    messagesModel.setData( messagesModel.index( 0, 2 ), MTo );
                    messagesModel.setData( messagesModel.index( 0, 3 ), tmp->PFrom );
                    messagesModel.setData( messagesModel.index( 0, 4 ), MMsg );
                    messagesModel.setData( messagesModel.index( 0, 5 ), MID );

                }

            }

            tmp = tmp->prev;
        }


        messagesTableView->selectRow( messagesModel.rowCount() - 1 );

    }

}

