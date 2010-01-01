#ifndef STATIONSWINDOW_H
#define STATIONSWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include "include/qaprscore.h"
#include "include/customsqlmodels.h"
#include "ui_stations.h"
//
class StationsWindowImpl : public QMainWindow, public Ui::StationsWindow
{
Q_OBJECT
public:
        StationsWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
//	QSqlQueryModel plainModel;

        QSqlDatabase *db;
        QAPRSCore *APRSCore;
        StationsSqlModel stationsModel;
        //QSqlQueryModel stationsModel;
        QString stationsQuery;

        bool isrequeringStations;
        void requeryStations();
        QString getDBVal( QString varname );
        void setDBVal( QString varname, QString varval );

private slots:
        void UpdateStationList();
private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent *event );


};
#endif




