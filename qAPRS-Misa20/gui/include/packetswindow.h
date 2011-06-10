#ifndef PACKETSWINDOW_H
#define PACKETSWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include <QCloseEvent>
#include <QSettings>
#include <QStandardItemModel>
#include <QList>
#include "./include/custommodels.h"
#include "../../core/include/qsysports.h"
#include "../../core/include/qatomix.h"
#include "ui_packets.h"
//
class PacketsWindowImpl : public QMainWindow, public Ui::PacketsWindow
{
Q_OBJECT
public:
        PacketsWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

        //QStandardItemModel packetsModel;
        PacketsModel packetsModel;
        PortsModel   *pModel;

        QAtomix   *atomGUI;

        bool enableSort;

        void closeEvent( QCloseEvent *event );
        void showEvent( QShowEvent *event );

private slots:
        void RXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );
        void TXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );
        void addPacket( QString TRX, int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );
private:



};
#endif




