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
#include <QDomDocument>
#include "ui_mainwindow.h"
#include "include/qaprscore.h"
#include "include/customsqlmodels.h"
#include "include/packetswindow.h"
#include "include/messageswindow.h"
#include "include/stationswindow.h"
#include "include/mapswindow.h"


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
        QAPRSCore *APRSCore;
        QString portsQuery;
        QTimer  *autoconnectTimer;

        int    autoTime; //

        void closeEvent(QCloseEvent *event);
        void hideEvent ( QHideEvent * event );


        PacketsWindowImpl  PacketsWindow;
        MessagesWindowImpl MessagesWindow;
        StationsWindowImpl StationsWindow;
        MapsWindowImpl     MapsWindow;

public slots:
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
        void autoconnect();
        void saveAndQuit();


        void UpdatePortsState( int pnum );
        void changeTab( int index );

        void savePortButtonClick( int pnum );
        void AGWOnOff( );
        void upPort();
        void downPort();

        void showPackets();
        void showMessages();
        void showStations();
        void showMaps();

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
        QAction *showMapsAction;
        QAction *quitAction;

        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;


};
#endif




