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

        packetsModel.setQuery( packetsQuery );
        packetTableView->setModel( &packetsModel );

        isrequeringPackets = FALSE;

}

void PacketsWindowImpl::showEvent ( QShowEvent * event ) {

    qDebug() << "Packets show";

    if ( getDBVal("pwin_X").length() > 0 ) {

        this->setGeometry( getDBVal("pwin_X").toInt() + 4, getDBVal("pwin_Y").toInt() + 23 /* !!! DURDOM */,
                           getDBVal("pwin_W").toInt(), getDBVal("pwin_H").toInt() );
    }

    if (APRSCore->coreActive==TRUE) {
        qDebug() << "Core in active mode";
        requeryPackets();
    } else {
        qDebug() << "Core not connected to sys DB";
    }

}

void PacketsWindowImpl::closeEvent(QCloseEvent *event) {

    qDebug()<<"Packets Close event";

    QSqlQuery query;

    query.prepare( "delete from vars where varname like 'pwin_%' " );
    query.exec();

    setDBVal( "pwin_X", QString::number( this->x() ) );
    setDBVal( "pwin_Y", QString::number( this->y() ) );
    setDBVal( "pwin_W", QString::number( this->width() ) );
    setDBVal( "pwin_H", QString::number( this->height() ) );

    hide();
    event->ignore();

}

void PacketsWindowImpl::TRXPacket() {

    QSqlQuery query;

    if (!(fromzePacketList->isChecked()) and this->isVisible() ) requeryPackets();

}

void PacketsWindowImpl::requeryPackets() {

    if (isrequeringPackets == FALSE) {

        isrequeringPackets = TRUE;
        qDebug()<< "PacketsWindow - packets Requery";

        packetsModel.setQuery( packetsModel.query().lastQuery() );

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

QString PacketsWindowImpl::getDBVal( QString varname ) {

    QSqlQuery query;

    query.prepare( "select varval from vars where varname='" + varname + "'" );
    query.exec();
    query.first();
    QString value = query.value( 0 ).toString();

    return value;

}

void PacketsWindowImpl::setDBVal( QString varname, QString varval ) {

    QSqlQuery query;

    query.prepare( "insert into vars(varval, varname) values(:p1, :p2) " );
    query.bindValue(":p1", varval);
    query.bindValue(":p2", varname);
    query.exec();

}
