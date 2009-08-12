#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtNetwork>
#include <QTextStream>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include "ui_mainwindow.h"
#include "include/qaprscore.h"
#include "include/customsqlmodels.h"

//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	QSqlDatabase *db;
        //QSqlQueryModel portsModel;
        PortsSqlModel portsModel;

        PacketsSqlModel packetsModel;
        //QSqlQueryModel messagesModel;
        MessagesSqlModel messagesModel;
        StationsSqlModel stationsModel;
        QString portsQuery;
        QString packetsQuery;
        QString messagesQuery;
        QString stationsQuery;
        QString DBType, DBName, UName, UPass, Host, Port, Call, Name, QTHN, Beacon, Symbol, Lat, Lng, Unproto, AGWEN, AGWP ;
        QAPRSCore *APRSCore;
private slots:
	void newBaseButtonClick();
	void connectButtonClick();
	void disconnectButtonClick();
        void saveButtonClick();
        void cancelButtonClick();
	void deletePortButtonClick();
	void editPortButtonClick();
	void addPortButtonClick();
	void currentIndexChanged ( int index); 
        void upAllPorts();
        void downAllPorts();
        void TRXPacket();
        void UpdateStationList();
        void UpdateMessageList();
        void UpdatePortsState( int pnum );
        void changeTab( int index );

        void savePortButtonClick( int pnum );
        void AGWOnOff( );
        void upPort();
        void downPort();

private:
	void loadOptionsFromFile();
        void saveOptionsToFile();
        void requeryPorts();
        void requeryPackets();
        void requeryMessages();
        void requeryStations();
        QSystemTrayIcon *trayIcon;
};
#endif




