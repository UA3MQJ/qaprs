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
#include <stdio.h>   /* уФБОДБТФОЩЕ ПВЯСЧМЕОЙС ЧЧПДБ/ЧЩЧПДБ */
#include <string.h>  /* пВЯСЧМЕОЙС УФТПЛПЧЩИ ЖХОЛГЙК */
#include <unistd.h>  /* пВЯСЧМЕОЙС УФБОДБТФОЩИ ЖХОЛГЙК UNIX */
#include <fcntl.h>   /* пВЯСЧМЕОЙС ХРТБЧМЕОЙС ЖБКМБНЙ */
#include <errno.h>   /* пВЯСЧМЕОЙС ЛПДПЧ ПЫЙВПЛ */
#include <termios.h> /* пВЯСЧМЕОЙС ХРТБЧМЕОЙС POSIX-ФЕТНЙОБМПН */
#endif

class QAPRSPort: public QObject
{
    Q_OBJECT

public:
       QAPRSPort( QObject *parent = 0 );
       ~QAPRSPort();

       //int portNum; //номер порта в списке ядра (присваивается ядром при создании порта)
       void setPortNum( int pN ) { iportNum = pN; }
       int  portNum() { return iportNum; }

        //Открытие порта (открывает порт с установленными параметрами);
       virtual void openPort();

        //Закрытие порта (закрывает порт);
       virtual void closePort();

        //Установка параметра порта (устанавливает параметры порта без изменения текущих, если порт открыт);
       virtual void setParam( QString ParamName, QString ParamValue );

       void         setAPRSCall( QString call ) { sAPRSCall = call; }
       QString      APRSCall( ) { return sAPRSCall; }

       //Обновление порта (в открытом порте устанавливает новые параметры взамен текущих);
       virtual void updatePort();

        //Определение статуса порта (включен/отключен/ошибка);
       int  portStatus() { return iPortStatus; }// 0-включен; 1-отключен; 2-ошибка.
        //Установка статуса порта (включен/отключен/ошибка);
       void  setPortStatus( int pStatus ) { iPortStatus = pStatus; emit changeState( iportNum, pStatus ); }

        //Определение "идет прием пакета";
       bool isRXPacket() { return bRXPacket; } // TRUE - если идет прием
       void setRXPacket( bool RX ) { bRXPacket = RX; emit RXPacket( iportNum, RX ); } // TRUE - если идет прием

        //Определение "идет передача пакета";
       bool isTXPacket() { return bTXPacket; }// TRUE - если идет передача
       void setTXPacket( bool TX ) { bTXPacket = TX; emit TXPacket( iportNum, TX ); }// TRUE - если идет передача

        //Определение "линия занята";
       bool isBUSY() { return bBUSY; }// TRUE - если линия занята
       void setBUSY( bool busy ) { bBUSY = busy; emit changeBUSYState( iportNum, busy ); }// TRUE - если линия занята



        //Определение типа порта
       QString      portType() {return this->sPortType; } // возвращает тип порта
       void         setPortType( QString PType ) { sPortType = PType; } // возвращает тип порта

        //Определение имени порта
       QString      portName() {return this->sPortName; } // возвращает имя порта
       void         setPortName( QString PName ) { sPortName = PName; } // задает имя порта



        //Передача APRS пакета через порт (Кому, ОтКого, ТекстСообщения);
       virtual void sendAPRSPacket( QString To, QString From, QString MsgText );

        //Передача AX пакета через порт (AX пакет);
       virtual void sendAXPacket( QByteArray AXPacket );

private:
       int          iportNum; //номер порта в списке ядра (присваивается ядром при создании порта)
       int          iPortStatus; //статус порта 0-включен; 1-отключен; 2-ошибка.
       bool         bRXPacket;   //TRUE принимается пакет
       bool         bTXPacket;   //TRUE передается пакет (во FLDIGI реально)
       bool         bBUSY;       //TRUE если канал занят

       QString      sPortType; //имя порта (Internet, KISS, AXIP etc)
       QString      sPortName; //пользовательское имя порта (Internet aprs.net, kiss<->mix, etc)

       QString      sAPRSCall;

signals:
        //Сигнал Принят AX пакет через порт (Кому, ОтКого, СодержимоеПакета);
       void         RXAXPacket( int pNum, QByteArray Packet );

        //Сигнал Принятия APRS пакета через порт (Кому, ОтКого, ТекстСообщения);
       void         RXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );

        //Сигнал передан AX пакет через порт (Кому, ОтКого, СодержимоеПакета);
       void         TXAXPacket( int pNum, QByteArray Packet );

        //Сигнал передачи APRS пакета через порт (Кому, ОтКого, ТекстСообщения);
       void         TXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );

        //Смена статуса порта (0-включен/1-отключен/2-ошибка);
       void         changeState( int pNum,  int state );

        //Сигнал "идет прием пакета";
       void         RXPacket( int pNum,  bool isRX );//два раза: при начале и при окончании

        //Сигнал "идет передача пакета";
       void         TXPacket( int pNum,  bool isTX );//два раза: при начале и при окончании

        //Сигнал смены занятости линии;
       void         changeBUSYState( int pNum,  bool busy );//при смене занятости линии (пока в портах нигде не определить занятость линии)


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
