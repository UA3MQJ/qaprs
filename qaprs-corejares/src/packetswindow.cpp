#include "packetswindow.h"
//
PacketsWindowImpl::PacketsWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{
	setupUi(this);

        packetsQuery = "select p1.K, substr(p1.DT,9,2)||\".\"||substr(p1.DT,6,2)||\".\"||substr(p1.DT,1,4) as DATA, substr(p1.DT,12,8) as TIME, "
                       "\"(\"||p1.port_num||\") \"||(p3.port_type_not) as Port, p2.port_note as PNote, p1.TRX as TRX, p1.PTo, p1.PFrom, p1.PVia, p1.Message "
                       "from packets p1 left join ports p2 on p1.port_num = p2.port_num "
                       "left join port_types p3 on p3.port_type_id = p2.port_type_id "
                       "order by K desc";

        isrequeringPackets = FALSE;

}

void PacketsWindowImpl::showEvent ( QShowEvent * event ) {

    qDebug() << "Packets show";

    if (APRSCore->coreActive==TRUE) {
        qDebug() << "Core in active mode";
        requeryPackets();
    } else {
        qDebug() << "Core not connected to sys DB";
    }

}

void PacketsWindowImpl::closeEvent(QCloseEvent *event) {

    qDebug()<<"Packets Close event";
    hide();
    event->ignore();

}

void PacketsWindowImpl::TRXPacket() {

    if (!(fromzePacketList->isChecked()) and this->isVisible() ) requeryPackets();

}

void PacketsWindowImpl::requeryPackets() {

    if (isrequeringPackets == FALSE) {

        isrequeringPackets = TRUE;
        qDebug()<< "PacketsWindow - packets Requery";

        packetTableView->setModel( NULL );
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
        isrequeringPackets = FALSE;

    }

}


