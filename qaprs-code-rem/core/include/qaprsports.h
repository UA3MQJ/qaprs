#ifndef QAPRSPORTS_H
#define QAPRSPORTS_H
#endif

#include <QObject>
#include <QNetworkProxy>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "../../core/include/qaprspacket.h"

#ifdef Q_OS_WIN
# include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <stdio.h>   /* ����������� ���������� �����/������ */
#include <string.h>  /* ���������� ��������� ������� */
#include <unistd.h>  /* ���������� ����������� ������� UNIX */
#include <fcntl.h>   /* ���������� ���������� ������� */
#include <errno.h>   /* ���������� ����� ������ */
#include <termios.h> /* ���������� ���������� POSIX-���������� */
#endif

class QAPRSPort: public QObject
{
    Q_OBJECT

public:
       QAPRSPort( QObject *parent = 0 );
       ~QAPRSPort();

       //int portNum; //����� ����� � ������ ���� (������������� ����� ��� �������� �����)
       void setPortNum( int pN ) { iportNum = pN; }
       int  portNum() { return iportNum; }

        //�������� ����� (��������� ���� � �������������� �����������);
       virtual void openPort();

        //�������� ����� (��������� ����);
       virtual void closePort();

        //��������� ��������� ����� (������������� ��������� ����� ��� ��������� �������, ���� ���� ������);
       virtual void setParam( QString ParamName, QString ParamValue );

       void         setAPRSCall( QString call ) { sAPRSCall = call; }
       QString      APRSCall( ) { return sAPRSCall; }

       //���������� ����� (� �������� ����� ������������� ����� ��������� ������ �������);
       virtual void updatePort();

        //����������� ������� ����� (�������/��������/������);
       int  portStatus() { return iPortStatus; }// 0-�������; 1-��������; 2-������.
        //��������� ������� ����� (�������/��������/������);
       void  setPortStatus( int pStatus ) { iPortStatus = pStatus; emit changeState( iportNum, pStatus ); }

        //����������� "���� ����� ������";
       bool isRXPacket() { return bRXPacket; } // TRUE - ���� ���� �����
       void setRXPacket( bool RX ) { bRXPacket = RX; emit RXPacket( iportNum, RX ); } // TRUE - ���� ���� �����

        //����������� "���� �������� ������";
       bool isTXPacket() { return bTXPacket; }// TRUE - ���� ���� ��������
       void setTXPacket( bool TX ) { bTXPacket = TX; emit TXPacket( iportNum, TX ); }// TRUE - ���� ���� ��������

        //����������� "����� ������";
       bool isBUSY() { return bBUSY; }// TRUE - ���� ����� ������
       void setBUSY( bool busy ) { bBUSY = busy; emit changeBUSYState( iportNum, busy ); }// TRUE - ���� ����� ������



        //����������� ���� �����
       QString      portType() {return this->sPortType; } // ���������� ��� �����
       void         setPortType( QString PType ) { sPortType = PType; } // ���������� ��� �����

        //����������� ����� �����
       QString      portName() {return this->sPortName; } // ���������� ��� �����
       void         setPortName( QString PName ) { sPortName = PName; } // ������ ��� �����



        //�������� APRS ������ ����� ���� (����, ������, ��������������);
       virtual void sendAPRSPacket( QString To, QString From, QString MsgText );

        //�������� AX ������ ����� ���� (AX �����);
       virtual void sendAXPacket( QByteArray AXPacket );

private:
       int          iportNum; //����� ����� � ������ ���� (������������� ����� ��� �������� �����)
       int          iPortStatus; //������ ����� 0-�������; 1-��������; 2-������.
       bool         bRXPacket;   //TRUE ����������� �����
       bool         bTXPacket;   //TRUE ���������� ����� (�� FLDIGI �������)
       bool         bBUSY;       //TRUE ���� ����� �����

       QString      sPortType; //��� ����� (Internet, KISS, AXIP etc)
       QString      sPortName; //���������������� ��� ����� (Internet aprs.net, kiss<->mix, etc)

       QString      sAPRSCall;

signals:
        //������ ������ AX ����� ����� ���� (����, ������, ����������������);
       void         RXAXPacket( int pNum, QByteArray Packet );

        //������ �������� APRS ������ ����� ���� (����, ������, ��������������);
       void         RXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );

        //������ ������� AX ����� ����� ���� (����, ������, ����������������);
       void         TXAXPacket( int pNum, QByteArray Packet );

        //������ �������� APRS ������ ����� ���� (����, ������, ��������������);
       void         TXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );

        //����� ������� ����� (0-�������/1-��������/2-������);
       void         changeState( int pNum,  int state );

        //������ "���� ����� ������";
       void         RXPacket( int pNum,  bool isRX );//��� ����: ��� ������ � ��� ���������

        //������ "���� �������� ������";
       void         TXPacket( int pNum,  bool isTX );//��� ����: ��� ������ � ��� ���������

        //������ ����� ��������� �����;
       void         changeBUSYState( int pNum,  bool busy );//��� ����� ��������� ����� (���� � ������ ����� �� ���������� ��������� �����)


};

//AXIP
class QAPRSAXIPPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSAXIPPORT( QObject * parent = 0 );
        ~QAPRSAXIPPORT( );

       void          openPort();
       void          closePort();
       void          setParam( QString ParamName, QString ParamValue );
       void          updatePort();
       void          sendAPRSPacket( QString To, QString From, QString MsgText );
       void          sendAXPacket( QByteArray AXPacket );
private:
       //================================
       QUdpSocket   *udpSocket;
       QTimer       *beacontimer;
       int           RXPort;
       int           TXPort;
       QHostAddress *TXAddress;
   private slots:
       void readPendingDatagrams();

};

//Internet
class QAPRSINTERNETPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSINTERNETPORT( QObject * parent = 0 );
        ~QAPRSINTERNETPORT( );

        void          openPort();
        void          closePort();
        void          setParam( QString ParamName, QString ParamValue );
        void          updatePort();
        void          sendAPRSPacket( QString To, QString From, QString MsgText );
        void          sendAXPacket( QByteArray AXPacket );

private:
        //================================
        QTimer     *reconnecttimer;
        int        srvnum;
        QTcpSocket *tcpClient;
        QString    UserName;
        QString    UserPass;
        QString    APRSFilter;
        QString    APRSServer1;
        QString    APRSServer2;
        QString    APRSServer3;
        bool       UseProxy;
        bool       UseProxyAuth;
        QString    ProxyName;
        int        ProxyPort;
        QString    ProxyUser;
        QString    ProxyPass;

        int nm;
    private slots:
        void tcpClientconnected () ;
        void tcpClientdisconnected ();
        void tcpClienterror (QAbstractSocket::SocketError socketError);
        void tcpClientread();
        void onReconnectTimer();
};


//AGW
class QAPRSAGWPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSAGWPORT( QObject * parent = 0 );
        ~QAPRSAGWPORT( );

        void          openPort();
        void          closePort();
        void          setParam( QString ParamName, QString ParamValue );
        void          updatePort();
        void          sendAPRSPacket( QString To, QString From, QString MsgText );
        void          sendAXPacket( QByteArray AXPacket );

private:
        //================================
        QTimer     *reconnecttimer;
        QTcpSocket *tcpClient;
        int     agwStatus; //0-connect to agw; //1-normal work
        QString agwHost;   //
        int     agwPort;   //

    private slots:
        void tcpClientconnected () ;
        void tcpClientdisconnected ();
        void tcpClienterror (QAbstractSocket::SocketError socketError);
        void tcpClientread();
        void onReconnectTimer();
};


//kiss
class QAPRSKISSPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSKISSPORT( QObject * parent = 0 );
        ~QAPRSKISSPORT( );

        void          openPort();
        void          closePort();
        void          setParam( QString ParamName, QString ParamValue );
        void          updatePort();
        void          sendAPRSPacket( QString To, QString From, QString MsgText );
        void          sendAXPacket( QByteArray AXPacket );

        //================================
        QTimer       *beacontimer;
        QByteArray    Command1;
        QByteArray    Command2;
        QByteArray    Command3;
        QByteArray    Command4;
        int           TXDelay;
        int           TXTail;
        int           SlotTime;
        int           Persist;
        int           FullDuplex;
        QString       ExitFromKISS;
        //ComReadThread *CRThread;
        QTimer       *ComReadtimer;

        QByteArray    ComReadData;
        QByteArray    ComName;

    #ifdef Q_OS_WIN
        HANDLE        m_Handle;
        int           TIMEOUT;
        int           ComSpeed;
        //int     ComNumber;
    #endif

    #ifdef Q_OS_UNIX
            int       fd; /* ???????? ?????????? ??? ????? */
    #endif


    private slots:
        void onComReadTimer();
        void onComRead(QString, QString, QString);
};

//FL Digi
class QAPRSFLDIGIPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSFLDIGIPORT( QObject * parent = 0 );
        ~QAPRSFLDIGIPORT( );

    void          openPort();
    void          closePort();
    void          setParam( QString ParamName, QString ParamValue );
    void          updatePort();
    void          sendAPRSPacket( QString To, QString From, QString MsgText );
    void          sendAXPacket( QByteArray AXPacket );

    //================================
    QTimer     *reconnecttimer;
    QTcpSocket *tcpClient;
    QString    FLDIGIHost;
    QString    FLDIGIPort;
    QByteArray datain;


private slots:
    void tcpClientconnected () ;
    void tcpClientdisconnected ();
    void tcpClienterror (QAbstractSocket::SocketError socketError);
    void tcpClientread();
    void onReconnectTimer();
};

//VASKA
class QAPRSVASKAPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSVASKAPORT( QObject * parent = 0 );
        ~QAPRSVASKAPORT( );

    void          openPort();
    void          closePort();
    void          setParam( QString ParamName, QString ParamValue );
    void          updatePort();
    void          sendAPRSPacket( QString To, QString From, QString MsgText );
    void          sendAXPacket( QByteArray AXPacket );

    //================================
    QTimer     *reconnecttimer;
    QTcpSocket *tcpClient;
    QString    VASKAHost;
    QString    VASKAPort;
    QString    VASKAMode;
    QByteArray datain;

private slots:
    void tcpClientconnected () ;
    void tcpClientdisconnected ();
    void tcpClienterror (QAbstractSocket::SocketError socketError);
    void tcpClientread();
    void onReconnectTimer();
};
