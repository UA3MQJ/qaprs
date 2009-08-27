#ifndef QAPRSCORE_H
#define QAPRSCORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include "include/qaprsports.h"
#include "include/aprs.h"

class QAPRSCore : public QObject
{
    Q_OBJECT
public:
    QAPRSCore( QObject *parent = 0 );
    QAPRSPort *Port[256];
    QTextEdit *log;
    QSqlDatabase *db;
    QAPRSPort *createPort( QString PortType );
    void      createPorts();
    void      closePorts();
    void      updatePort( int pnum );
    void      deletePort( int pnum );
    void      AGWEmulatorStop();
    void      AGWEmulatorStart();
    int       AGWEmulatorPort;
    void      sendAGWPortInfo();
    bool      coreActive;
    //station parameters
    QString   Lat, Lng;
    //agw simulator
    int agwPN; //port count
    bool AGWEmulatorActive;
    QTcpServer *tcpServer;
    QTcpSocket *tcpServerConnection;
    bool    monitoringPackets;
    QFile       fileLog;
    QTextStream outLog;
    QFile       filePacketLog;
    QTextStream outPacketLog;

    QSqlQuery queryInsert1;

signals:
        void TRXPacket();
        void TRXMessage();
        void UpdateStationList();
        void PortChangeState(int pnum);
public slots:
        void ToLog (QString Text);
private slots:
        void RXPacket (int PortNum, QString To, QString From, QString MsgText);
        void TXPacket (int PortNum, QString To, QString From, QString MsgText);
        void CorePortChangeState(int pnum);
        //agw simulator
        void tcpServerNewConnection ();
        void tcpServerDisconnect () ;
        void tcpServerRead ();

};

#endif // QAPRSCORE_H
