/*

 ����� ����. ���� ������ � ���, ��� �� ����� ������������ ����, ���� � ����� � ������� ������������ - �������.
 ��� ��� ������� ����� ����� ����� �������, ��� ���� ���������� ������ ������ �� ��� ������� ����������,
 ����� ��� ����. �� ��������� ����� �� ���� ��������� ������ ���� ���������� ���������� � ������� ��� ���������.
 ��� �������, ������������ � �����, ���������� � � ������. ����� ����� ������ - ���������� ����� ����� � ������.
 ��� ������ �� ������ �������������. �� ������ �� ��, ��� ��� ���������� � ������ �����������.
 ���� ���������� � ����� ����������, GUI � ������, �� ��� �������������� �� ���� ������ ����� ������.

 ������� �������� ������������ �����, ������� ������ ���� ����������� - ��� ����������� ����� ����� ����.
 ������ ����� ������������� \r\n, � ������ UTF16.

 � ������ ����������� ����� ��������� ������ ������. �������� - ������� ����� ������. ���� ���� �������� � �������,
 ���� gui ���������� �������.

 2011-06-11 ������ ����� ������� ����� ��������� ���� ������ ID_DEST>ID_SRC.
            ���� ����� ���������� ����������� �����, �� ������ ������ ������������ �.
            ���� ����� ���������� ����, �� ID_DEST = 0

 2011-06-17 �����������. �������� ����������� ����� ����� ���� ��������� ����������. �� ���� ���������
            �� ������ SysVars ����� �� NULL � ����� ����� ����� �������� � ���� �� ������. ����� ��������
            ���������� ����� ����� �������� ������ ���������� ����� ������ ��������.

 2011-06-21 ������� � ������ ������ ���� �����������, ���� �� ���������� � ������� �������.
            ���������� ������� �� ������� ���� ����������� ���� �� ��������� ��� ������.
            ��� ����� ������������ �������.

 */

#ifndef QATOMIX_H
#define QATOMIX_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include "../../core/include/version.h"
#include "../../core/include/qsysvars.h"
#include "../../core/include/qsysports.h"
#include "../../core/include/qsysbeacons.h"
#include "../../core/include/qsyspackets.h"
#include "../../core/include/qsysstationlist.h"
#include "../../core/include/qaprssymbols.h"


class QAtomix : public QThread
{
    Q_OBJECT
public:
    explicit QAtomix(QObject *parent = 0);

    //���������� ��� ������ storage ������������ ������ �� ��������
    bool started;

    bool sysDBNeedDeleteSysPorts;
    bool sysDBNeedDeleteSysBeacons;

    //�������� ��
    bool loadingMode; //����� ������������ ����� - � �������� ������������� � �������� ������

    bool connected; //������� ����, ��� �� ������� � ������� �������

    int     uid;
    QString atomID;

    void    setAtomName( QString sAtomName );
    void    setStorageType( int stp );
    QString atomName( ) { return vatomName; };
    void    startSaveToStorage();

    //���������� ������������ �����
    void    addAbStorage(); //���������� ����� ����������� ������ � ������ � ��������� ��
    void    addAbSysVars(); //���������� ����� ����������� ����� ������ ����� ����������


    //���������� � ���������� ����� ������
    //���-�� ����� ������. �������� ������ ���� ��������� ����
    //��� �������
    int           im; // 0-server; 1-client
    QTcpServer   *tcpServer;
    QTcpSocket   *tcpServerConnection;
    int           tcpPort;
    QTcpSocket   *tcpClient;
    QByteArray    inData;
    QTextCodec   *sysDecoder; //��������� �������������� UTF8<->UTF16
    // ��� ���������� ������� ����� � ��� �� QString'� ���������� UTF16
    // ��� �������� ����� tcp/ip ������������ ������������� � UTF8
    QTextCodec   *sysDecoder16; //��������� �������������� UTF16<->UTF16

    //����� ID_DEST>ID_SRC thought
    void    doThink( QString ID_DEST, QString ID_SRC, QString thought ); //��������

    int     iStorageType; // -1 - �� ��������� ������� ������, 0 - SQLite


    //������ ������
    //��������� ����������
    QSysVars     *SysVars;
    //��������� �����
    QSysPorts    *SysPorts;
    //��������� �����
    QSysBeacons  *SysBeacons;
    //�������
    QAPRSSymbols *SysSymbols;
    //��������/���������� ������
    QAPRSPacketList *SysPackets;
    //�������
    QAPRSstationList *SysStations;

private:

    QString vatomName;

    void    weThink( QString ID_DEST, QString ID_SRC, QString thought ); //��� �� ������ (!!!������ �� ��������)

signals:

    //�������, ����������� � ��������� ������ �� ����, ��� ��� ��� ������� �������
    void    SIGweThink( QString thought );
    //������ � ���������� �����������
    void    SIGweSysvarSet( QString VarName, QString VarVal );
    //������ � �������
    void    SIGweSysportSetParam( int PortNum, QString ParName, QString ParVal );
    void    SIGweSysportChangeState( int PortNum, QString PortState );
    //������ � �������
    void    SIGweSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );
    void    SIGweSysbeaconChangeState( int BcknNum, QString BcknState );

    //��������� �����
    void    SIGweportRXPacket( int pNum, bool isRX );
    //�������� �����
    void    SIGweportTXPacket( int pNum, bool isTX );

    void    SIGportRXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );
    void    SIGportTXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );

    void    SIGstationSetParam( QString call, QString par_name , QString par_val );

    //���� ������ ����� �����������, ����� �������� ��� ������ ������ ���������
    //��������, �������� ������������ ���� �� ���� ����� ��������� ���������
    void    storeProgress( QString msg );

    void    RCVStationPositionInfo( QString Call, QString Sym, QString LH, QString Lat, QString Lng, QString Grid, QString Via, QString Stcomment );
    void    RCVStationStatus( QString Call, QString Ststatus );

    void    RCVRXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
    void    RCVTXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );

    void    SIGallLoaded( ); //������ � ����, ��� ��� ���������
    void    SIGSocketError( ); //������ ����������

protected:
    void    run();

public slots:
    void    say( QString msg );

    //���������� ��������� �����
    void    beServer( int port );
    void    beClient( QString address, int port );
    void    SocketError(QAbstractSocket::SocketError socketError);
    void   tcpServerNewConnection (); //�������������
    void   tcpServerDisconnect () ;   //������������
    void   tcpServerRead ();          //������ ���������
    void   tcpClientread();
    void   tcpClientconnected () ;
    //void tcpClientdisconnected ();
    //void tcpClienterror (QAbstractSocket::SocketError socketError);
    //void tcpClientread();

    void portRXPacket( int pNum,  bool isRX ); //���� ����� ������
    void portTXPacket( int pNum,  bool isRX ); //���� �������� ������
    void portRXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );
    void portTXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );
    void portChangeState( int pNum,  int state ); //����� ������� ����� (0-�������/1-��������/2-������);

    //���������� ������� - ���������
    void parseAPRSPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString From, QString Via, QString MsgText );
    void parseAPRSPositionPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString Via, QString From, QString MsgText );
    void parseAPRSStatusPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString Via, QString From, QString MsgText );

    void beaconChangeState( int bNum,  int state ); //����� ������� ����� (0-�������/1-��������);
    void beaconSendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText ); //������� �����


    //��� ������ ��������� ��������� ������. �������� ����� ������ ����� ���!
    //�� ����� ���� ��� ������ �� ������ � ���������� ��� ��������� ���� ������
    //� ������������� ���� ��������. �� ���� ���(we) ����� ������ ��� �������.
    //� ��� ���������� ���� ������ ������ ������ � ��������� (DO)
    //
    //
    //������ � ���������� �����������
    //
    //���������� ��������� ����������
    void weSysvarSet( QString VarName, QString VarVal );
    //��������� ��� ��������� ����������
    void weSysvarReq();

    //������ � �������
    //
    //������� ��� �����
    void weSysportOpenAll( );
    //������� ��� �����
    void weSysportCloseAll( );
    //��������� ��� �����
    //������������ ����� ������ ��������� ������: �����/���/�������� � ��� �� ������������� ������
    void weSysportReqPorts( );
    //�������� ���������� �����
    void weSysportOpen( int PortNum );
    //�������� ���������� �����
    void weSysportClose( int PortNum );
    //�������� ���������� �����
    void weSysportDelete( int PortNum );
    //���������� ���������� �����
    void weSysportUpdate( int PortNum );
    //��������� ��������� �����
    //��� ��������� ���� ����� - ���� ���������, ���� ��� ��� ���
    //���� ��� �� ���������, �� ���� ��������� � ��������� �����
    //������� ������� �������� ���, ���, � ����� ��� ��������� ��������� �����
    void weSysportSetParam( int PortNum, QString ParName, QString ParVal );
    //��������� ��� ��������� �����
    void weSysportReqParams( int PortNum );
    //��������� ������ �����
    void weSysportReqStatus( int PortNum );
    //��������� �����
    void weportRXPacket( int pNum, bool isRX );
    //�������� �����
    void weportTXPacket( int pNum, bool isRX );
    void weportRXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );
    void weportTXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );


    //������ � �������
    //
    //��������� ��������� �����
    //���� ��������� ��� ������� ������ �� ����������
    void weSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );
    //�������� ���������� �����
    void weSysbeaconOpen( int BcknNum );
    //�������� ���������� �����
    void weSysbeaconClose( int BcknNum );
    //�������� ���������� �����
    void weSysbeaconDelete( int BcknNum );
    //�������� ���������� �����
    void weSysbeaconSend( int BcknNum );
    //�������� ������� ���������� �����
    void weSysbeaconStatusSend( int BcknNum );
    //���������� ���������� �����
    void weSysbeaconUpdate( int BcknNum );
    //������������ ����� ������ ��������� ������
    void weSysbeaconReqBeacons( );
    //��������� ��� ��������� �����
    void weSysbeaconReqParams( int BcknNum );
    //��������� ������ �����
    void weSysbeaconReqStatus( int BcknNum );
    //������� ��� �����
    void weSysbeaconOpenAll( );
    //������� ��� �����
    void weSysbeaconCloseAll( );

    //��������� ��� ������
    void weSyspacketsReq( );

    //������ � �����������
    //
    void weSendMessage( int portNum, QString To, QString From, QString Via, QString MsgText, bool ACK );

private slots:
    //�� ��, ��� � ����, �� ���� ���� ������� �� ��������, � ��� ��� ����������
    //�� ��������� ���, ��� ��� �����. � ������� ���� sysvars ���� � ������ ������� � ����,
    //�� ����� ���� ������ � ����� ���� � � ���� �������� ������ ����.
    //��� �� ������ ���� ��������� � ����.

    //������ � ���������� �����������
    //
    void DOweSysvarSet( QString VarName, QString VarVal );
    void DOweSysvarReq( QString ID_SRC );

    //������ � �������
    //
    void DOweSysportOpenAll( );
    void DOweSysportCloseAll( );
    void DOweSysportOpen( int PortNum );
    void DOweSysportClose( int PortNum );
    void DOweSysportDelete( int PortNum );
    void DOweSysportUpdate( int PortNum );
    void DOweSysportReqPorts();
    void DOweSysportReqParams( int PortNum );
    void DOweSysportReqStatus( int PortNum );
    void DOweSysportSetParam( int PortNum, QString ParName, QString ParVal );

    //������ � �������
    //
    void DOweSysbeaconOpenAll( );
    void DOweSysbeaconCloseAll( );
    void DOweSysbeaconOpen( int BcknNum );
    void DOweSysbeaconClose( int BcknNum );
    void DOweSysbeaconDelete( int BcknNum );
    void DOweSysbeaconUpdate( int BcknNum );
    void DOweSysbeaconSend( int BcknNum );
    void DOweSysbeaconStatusSend( int BcknNum );
    void DOweSysbeaconReqBeacons( );
    void DOweSysbeaconReqParams( int BcknNum );
    void DOweSysbeaconReqStatus( int BcknNum );
    void DOweSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );

    void DOweSyspacketsReq( );

    void DOweSendMessage( int portNum, QString To, QString From, QString Via, QString MsgText, bool ACK );

};

#endif // QATOMIX_H
