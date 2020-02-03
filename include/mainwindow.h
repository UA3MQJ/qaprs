#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtNetwork>
#include <QTextStream>
#include <QSystemTrayIcon>
#include <QFile>
#include "include/qaprscore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase *db;
    QSqlQueryModel plainModel;
    QSqlQueryModel packetsModel;
    QSqlQueryModel messagesModel;
    QString portsQuery;
    QString packetsQuery;
    QString messagesQuery;
    QString DBType, DBName, UName, UPass, Host, Port, Call, Name, QTHN, Beacon, Symbol, Lat, Lng, Unproto ;
    QAPRSCore *APRSCore;

public slots:
        void newBaseButtonClick();
        void connectButtonClick();
        void disconnectButtonClick();
        void saveButtonClick();
        void cancelButtonClick();
        void deletePortButtonClick();
        void editPortButtonClick();
        void addPortButtonClick();
        void currentIndexChanged( int index);
        void upAllPorts();
        void downAllPorts();
        void TRXPacket();


private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    void loadOptionsFromFile();
    void saveOptionsToFile();
    void requeryPorts();
    void requeryPackets();
    void requeryMessages();


};
#endif




