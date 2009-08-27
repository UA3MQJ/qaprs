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
#include <QHideEvent>
#include <QFile>
#include <QMenu>
#include <QDebug>
#include "ui_mainwindow.h"
#include "include/qaprscore.h"
#include "include/customsqlmodels.h"
#include "include/packetswindow.h"
#include "include/messageswindow.h"
#include "include/stationswindow.h"


//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT



public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
        ~MainWindowImpl( );
	QSqlDatabase *db;
        //QSqlQueryModel portsModel;
        PortsSqlModel portsModel;

        bool isrequeringPorts;

        QString messagesQuery;
        QString DBType, DBName, UName, UPass, Host, Port, Call, Name, QTHN, Beacon, Symbol, Lat, Lng, Unproto, AGWEN, AGWP ;
        QAPRSCore *APRSCore;
        QString portsQuery;

        void closeEvent(QCloseEvent *event);
        void hideEvent ( QHideEvent * event );


        PacketsWindowImpl  PacketsWindow;
        MessagesWindowImpl MessagesWindow;
        StationsWindowImpl StationsWindow;

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


        void UpdatePortsState( int pnum );
        void changeTab( int index );

        void savePortButtonClick( int pnum );
        void AGWOnOff( );
        void upPort();
        void downPort();

        void showPackets();
        void showMessages();
        void showStations();

private:
	void loadOptionsFromFile();
        void saveOptionsToFile();
        void requeryPorts();
        void createActions();
        void createTrayIcon();

        QAction *showGenOptionAction;
        QAction *showPacketsAction;
        QAction *showMessagesAction;
        QAction *showStationsAction;
        QAction *quitAction;

        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;


};
#endif




