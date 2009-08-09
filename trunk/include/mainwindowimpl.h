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
#include <QFile>
#include "ui_mainwindow.h"
#include "include/qaprscore.h"

//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	QSqlDatabase *db;
	QSqlQueryModel plainModel;
        QSqlQueryModel packetsModel;
        QSqlQueryModel messagesModel;
        QString portsQuery;
        QString packetsQuery;
        QString messagesQuery;
        QString DBType, DBName, UName, UPass, Host, Port, Call, Name, QTHN, Beacon, Symbol, Lat, Lng, Unproto ;
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
private:
	void loadOptionsFromFile();
        void saveOptionsToFile();
        void requeryPorts();
        void requeryPackets();
        void requeryMessages();
        QSystemTrayIcon *trayIcon;
};
#endif




