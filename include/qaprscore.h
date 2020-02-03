#ifndef QAPRSCORE_H
#define QAPRSCORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include "include/qaprsports.h"

class QAPRSCore : public QObject
{
    Q_OBJECT
public:
    QAPRSCore(QObject *parent = nullptr);
    QAPRSPort    *Port[256];
    QTextEdit    *log;
    QSqlDatabase *db;
    QAPRSPort    *createPort( QString PortType );
    void          createPorts();
    void          closeAllPorts();
    //agw simulator
    QTcpServer   *tcpServer;
    QTcpSocket   *tcpServerConnection;
    bool          monitoringPackets;
    QFile         fileLog;
    QTextStream   outLog;
    QFile         filePacketLog;
    QTextStream   outPacketLog;

signals:
    void TRXPacket();
public slots:
        void ToLog (QString Text);
private slots:
        void RXPacket (int PortNum, QString To, QString From, QString MsgText);
        void TXPacket (int PortNum, QString To, QString From, QString MsgText);
        //agw simulator
        void tcpServerNewConnection ();
        void tcpServerDisconnect () ;
        void tcpServerRead ();

};

#endif // QAPRSCORE_H
