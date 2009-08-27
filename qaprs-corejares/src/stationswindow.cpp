#include "stationswindow.h"
//
StationsWindowImpl::StationsWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{
	setupUi(this);

        stationsQuery = "select t1.Call, t1.Sym, t2.Comment, t1.Lat, t1.Lng, t1.Grid, t1.Via, t1.StComment, null as Length, null as Deg, t1.LH, NULL as MinAgo "
                        "from stations t1 left join symbols t2 on t1.sym=t2.sym";

        isrequeringStations = FALSE;

        stationsModel.APRSCore = this->APRSCore;

}

void StationsWindowImpl::showEvent ( QShowEvent * event ) {

    qDebug() << "Stations show";

    if (APRSCore->coreActive==TRUE) {
        qDebug() << "Core in active mode";
        requeryStations();
    } else {
        qDebug() << "Core not connected to sys DB";
    }

}

void StationsWindowImpl::closeEvent(QCloseEvent *event) {

    qDebug()<<"Stations Close event";
    hide();
    event->ignore();

}

void StationsWindowImpl::UpdateStationList() {

    if (this->isVisible() ) requeryStations();

}

void StationsWindowImpl::requeryStations() {

    if (isrequeringStations == FALSE) {

        qDebug()<< "StationsWindow - Stations Requery";
        isrequeringStations = TRUE;

        stationsTableView->setModel( NULL );
        stationsModel.setQuery( stationsQuery );
        stationsTableView->setModel( &stationsModel );

        stationsTableView->selectRow( 0 );

        stationsTableView->setColumnWidth(  0,  70 );
        stationsTableView->setColumnWidth(  1,  40 );
        stationsTableView->setColumnWidth(  2,  70 );
        stationsTableView->setColumnWidth(  3,  60 );
        stationsTableView->setColumnWidth(  4,  67 );
        stationsTableView->setColumnWidth(  5,  55 );
        stationsTableView->setColumnWidth(  6,  110 );
        stationsTableView->setColumnWidth(  7,  110 );
        stationsTableView->setColumnWidth(  8,  55 );
        stationsTableView->setColumnWidth(  9,  55 );
        stationsTableView->setColumnWidth( 10,  110 );
        stationsTableView->setColumnWidth( 11,  55 );

        isrequeringStations = FALSE;

    }

}


