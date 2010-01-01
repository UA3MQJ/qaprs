#ifndef QAPRSPORTS_H
#define QAPRSPORTS_H

#include <QObject>
#include <QNetworkProxy>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "include/qaprspacket.h"

#ifdef Q_OS_WIN
# include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#endif

class QAPRSPort: public QObject
{
    Q_OBJECT
    public:
       QAPRSPort( QObject *parent = 0 );
       ~QAPRSPort();


    virtual void    openPort();
    virtual void    closePort();
    virtual void    updatePort();
    virtual void    sendPacket( QString To, QString From, QString MsgText );
    virtual void    setParam( QString ParamName, QString ParamValue );
    QString APRSCall;
    bool    isActive() { return active; };
    void    setReadOnly(bool RO);
    bool    isReadOnly();
    void    setCall(QString Text);
    void    setLatitude(QString Text);
    void    setLongitude(QString Text);
    void    setUnprotoAddress(QString Text);
    void    setBeaconComment(QString Text);
    void    setSymbol(QString Text);
    void    setBeaconInterval(int iv);
    QString PortType; //ХЛЪ ОНПРЮ (Internet, KISS, AXIP etc)
    QString PortName; //ОНКЭГНБЮРЕКЭЯЙНЕ ХЛЪ ОНПРЮ (Internet aprs.net, kiss<->mix, etc)
    int     PortNum;
    int     agwPortNum;
    bool    active;
    bool    readOnly;
    QString Call;
    QString Latitude;
    QString Longitude;
    QString UnprotoAddress;
    QString BeaconText;
    QString Symbol;
    int     BeaconInterval;
    QTextEdit *log;
    bool    rxData, txData;
signals:
    void RXPacket(int PortNum, QString To, QString From, QString MsgText);
    void TXPacket(int PortNum, QString To, QString From, QString MsgText);
    void ToLog(QString Text);
    void ChangeState(int pnum);


};

//AXIP
class QAPRSAXIPPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSAXIPPORT( QObject * parent = 0 );
        ~QAPRSAXIPPORT( );

    void    openPort();
    void    closePort();
    void    updatePort();
    void    setParam( QString ParamName, QString ParamValue );
    void    sendPacket( QString To, QString From, QString MsgText );
    bool    isActive();
    void    setReadOnly(bool RO);
    bool    isReadOnly();
    void    setCall(QString Text);
    void    setLatitude(QString Text);
    void    setLongitude(QString Text);
    void    setUnprotoAddress(QString Text);
    void    setBeaconComment(QString Text);
    void    setSymbol(QString Text);
    void    setBeaconInterval(int iv);
    //================================
    QUdpSocket *udpSocket;
    QTimer     *beacontimer;
    int     RXPort;
    int     TXPort;
    QHostAddress *TXAddress;

    bool    RXPortUpdate;

private slots:
    void readPendingDatagrams();
    void onTimer();

};

//Internet
class QAPRSINTERNETPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSINTERNETPORT( QObject * parent = 0 );
        ~QAPRSINTERNETPORT( );

    void    openPort();
    void    closePort();
    void    updatePort();
    void    setParam( QString ParamName, QString ParamValue );
    void    sendPacket( QString To, QString From, QString MsgText );
    bool    isActive();
    void    setReadOnly(bool RO);
    bool    isReadOnly();
    void    setCall(QString Text);
    void    setLatitude(QString Text);
    void    setLongitude(QString Text);
    void    setUnprotoAddress(QString Text);
    void    setBeaconComment(QString Text);
    void    setSymbol(QString Text);
    void    setBeaconInterval(int iv);
    //================================
    QTimer     *beacontimer;
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
    void onTimer();
    void onReconnectTimer();
};

//AGW
class QAPRSAGWPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSAGWPORT( QObject * parent = 0 );
        ~QAPRSAGWPORT( );

    void    openPort();
    void    closePort();
    void    updatePort();
    void    setParam( QString ParamName, QString ParamValue );
    void    sendPacket( QString To, QString From, QString MsgText );
    bool    isActive();
    void    setReadOnly(bool RO);
    bool    isReadOnly();
    void    setCall(QString Text);
    void    setLatitude(QString Text);
    void    setLongitude(QString Text);
    void    setUnprotoAddress(QString Text);
    void    setBeaconComment(QString Text);
    void    setSymbol(QString Text);
    void    setBeaconInterval(int iv);
    //================================
    QTimer     *beacontimer;
    QTimer     *reconnecttimer;
    QTcpSocket *tcpClient;
    int     agwStatus; //0-ЯНЕДХМЕМХЕ Я agw; //1-МНПЛЮКЭМЮЪ ПЮАНРЮ
    QString agwHost;   //
    int     agwPort;   //

private slots:
    void tcpClientconnected () ;
    void tcpClientdisconnected ();
    void tcpClienterror (QAbstractSocket::SocketError socketError);
    void tcpClientread();
    void onTimer();
    void onReconnectTimer();
};


//kiss
 class ComReadThread : public QThread
 {
     Q_OBJECT

 public:
     ComReadThread(QObject *parent = 0);
     ~ComReadThread();
#ifdef Q_OS_WIN
     HANDLE m_Handle;
#endif
signals:
     void RXPacket( QString To, QString From, QString MsgText );

 protected:
     void run();

 };

class QAPRSKISSPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSKISSPORT( QObject * parent = 0 );
        ~QAPRSKISSPORT( );
    void    openPort();
    void    closePort();
    void    updatePort();
    void    setParam( QString ParamName, QString ParamValue );
    void    sendPacket( QString To, QString From, QString MsgText );
    bool    isActive();
    void    setReadOnly(bool RO);
    bool    isReadOnly();
    void    setCall(QString Text);
    void    setLatitude(QString Text);
    void    setLongitude(QString Text);
    void    setUnprotoAddress(QString Text);
    void    setBeaconComment(QString Text);
    void    setSymbol(QString Text);
    void    setBeaconInterval(int iv);
    //================================
    QTimer     *beacontimer;
    QByteArray  Command1;
    QByteArray  Command2;
    QByteArray  Command3;
    QByteArray  Command4;
    int         TXDelay;
    int         TXTail;
    int         SlotTime;
    int         Persist;
    int         FullDuplex;
    QString     ExitFromKISS;
    //ComReadThread *CRThread;
    QTimer     *ComReadtimer;
    
    QByteArray ComReadData;
    QByteArray ComName;
    
#ifdef Q_OS_WIN
    HANDLE m_Handle;
    int TIMEOUT;
    int     ComSpeed;
    //int     ComNumber;
#endif

#ifdef Q_OS_UNIX
 	int fd; /* Файловый дескриптор для порта */
#endif

#endif // QAPRSPORTS_H

private slots:
    void onTimer();
    void onComReadTimer();
    void onComRead(QString, QString, QString);

};

//FL Digi
class QAPRSFLDIGIPORT: public QAPRSPort  {
    Q_OBJECT
    public:
        QAPRSFLDIGIPORT( QObject * parent = 0 );
        ~QAPRSFLDIGIPORT( );

    void    openPort();
    void    closePort();
    void    updatePort();
    void    setParam( QString ParamName, QString ParamValue );
    void    sendPacket( QString To, QString From, QString MsgText );
    bool    isActive();
    void    setReadOnly(bool RO);
    bool    isReadOnly();
    void    setCall(QString Text);
    void    setLatitude(QString Text);
    void    setLongitude(QString Text);
    void    setUnprotoAddress(QString Text);
    void    setBeaconComment(QString Text);
    void    setSymbol(QString Text);
    void    setBeaconInterval(int iv);
    //================================
    QTimer     *beacontimer;
    QTimer     *reconnecttimer;
    int        srvnum;
    QTcpSocket *tcpClient;
    QString    FLDIGIHost;
    QString    FLDIGIPort;
    QByteArray datain;


    int nm;

private slots:
    void tcpClientconnected () ;
    void tcpClientdisconnected ();
    void tcpClienterror (QAbstractSocket::SocketError socketError);
    void tcpClientread();
    void onTimer();
    void onReconnectTimer();
};
