#ifndef PACKETSWINDOW_H
#define PACKETSWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include "include/qaprscore.h"
#include "include/customsqlmodels.h"
#include "ui_packets.h"
//
class PacketsWindowImpl : public QMainWindow, public Ui::PacketsWindow
{
Q_OBJECT
public:
        PacketsWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
//	QSqlQueryModel plainModel;

        QSqlDatabase *db;
        QAPRSCore *APRSCore;
        PacketsSqlModel packetsModel;
        QString packetsQuery;
        bool isrequeringPackets;
        void requeryPackets();
        QString getDBVal( QString varname );
        void setDBVal( QString varname, QString varval );

private slots:
        void TRXPacket();
private:
        void closeEvent(QCloseEvent *event);
        void showEvent ( QShowEvent * event );


};
#endif




