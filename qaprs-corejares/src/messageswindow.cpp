#include "messageswindow.h"
//
MessagesWindowImpl::MessagesWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{
	setupUi(this);

        messagesQuery = "select p1.packet_K as K, substr(p2.DT,9,2)||\".\"||substr(p2.DT,6,2)||\".\"||substr(p2.DT,1,4) as DATA, substr(p2.DT,12,8) as TIME, "
                        "\"(\"||p2.port_num||\") \"||(p4.port_type_not) as Port, p3.port_note as PNote, p2.TRX as TRX, p2.PFrom, p1.MTo, p1.Message, p1.Mess_ID as MSGID "
                        "from messages p1 left join packets p2 on p1.packet_K=p2.K "
                        "left join ports p3 on p3.port_num=p2.port_num "
                        "left join port_types p4 on p4.port_type_id = p3.port_type_id "
                        "order by p1.packet_K desc ";

        isrequeringMessages = FALSE;

}

void MessagesWindowImpl::showEvent ( QShowEvent * event ) {

    qDebug() << "Messages show";

    if (APRSCore->coreActive==TRUE) {
        qDebug() << "Core in active mode";
        requeryMessages();
    } else {
        qDebug() << "Core not connected to sys DB";
    }

}

void MessagesWindowImpl::closeEvent(QCloseEvent *event) {

    qDebug()<<"Messages Close event";
    hide();
    event->ignore();

}

void MessagesWindowImpl::UpdateMessageList() {

    if (this->isVisible() ) requeryMessages();

}

void MessagesWindowImpl::requeryMessages() {

    if (isrequeringMessages == FALSE) {

        isrequeringMessages = TRUE;
        qDebug()<< "MessagesWindow - messages Requery";

        messagesTableView->setModel( NULL );
        messagesModel.setQuery( messagesQuery );
        messagesTableView->setModel( &messagesModel );

        messagesTableView->selectRow( 0 );

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
        isrequeringMessages = FALSE;

    }

}


