#ifndef QAPRSCORE_H
#define QAPRSCORE_H

#include <QObject>
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDomDocument>
#include <QStringList>
#include <QDebug>
#include <QTextCodec>
#include "../../core/include/qaprsports.h"
#include "../../core/include/qaprsbeacon.h"

class QAPRSCore : public QObject
{
    Q_OBJECT
public:
    QAPRSCore();
    ~QAPRSCore();
    void Start(); //������ ����
    //�� �������� ����� ���� ����������� �������� ��� ����� ���������
    void setGUIPort( int port ); //��������� ������ TCP/IP �����, �� �������� ���� ���� GUI
    void setDBName( QString DBName ); //��������� ����� ��������� ���� ������
    void recreateSYSDB();//������������ ��������� ���� ������
    //������ � �������
    int  createPort( int pNum, QString pType );//�������� ����� ������������� ����+������ � ��
    int  createPort( int pNum );//�������� ����� (������ ������� �� ��)
    int  deletePort( int pNum );//�������� ����� (���� �� ��� ������)
    int  setPortName( int pNum, QString pName );//������ ���������������� �� �����+������ � ��
    int  setPortParam( int pNum, QString pVar, QString pVal );//������ �������� �����+������ � ��
    int  openPort( int pNum );//�������� ����� (���� �� ��� ������)
    int  closePort( int pNum );//�������� ����� (���� �� ��� ������)
    void createAllPorts( );//�������� ���� ������
    void closeAllPorts( );//�������� ���� ������
    void openAllPorts( );//�������� ���� ������
    //������ � �������
    int  createBeacon( int num, int port_num, QString bSym, QString bCall, QString bLat, QString bLng,
                                QString bUnproto, QString bText, QString bInterval);
    int  setBeaconParam( int bNum, QString pVar, QString pVal );//������ �������� �����+������ � ��
    void createAllBeacons( );//�������� ���� ������
    void openAllBeacons( );//�������� ���� ������
    void closeAllBeacons( );//�������� ���� ������
    int  openBeacon( int bNum );//�������� ����� (���� �� ��� ������)
    int  closeBeacon( int bNum );//�������� ����� (���� �� ��� ������)
    int  deleteBeacon( int bNum );//�������� ����� (���� �� ��� ������)

private:
    //��������� ��
    QSqlDatabase  db;
    QString       SYSDBName;
    //������ � ���������� ��� ������ � ���������� ������
    QTcpServer   *tcpGUIServer;
    QTcpSocket   *tcpGUIServerConnection;
    int           tcpGUIPort;
    //������ ���� ������, ���������� �� ����������, ���� �� ���� ���� ��� ������
    //����������� � ������. ���� ���� ������ �������� ������, �� �������� �����
    //�� ���� � ������������, ��� �������
    QByteArray    inData;

    //��������� ������� �� GUI
    void          doCmd( QByteArray cmdText );

    QString       APRSCall; //�������� APRS

    QString QStringToHEX( QString Text );

    //�����
    QAPRSPort     *port[256];
    //�����
    QAPRSBeacon   *beacon[256];

signals:
    void stop(); //�� ����� ������� ���������� ��� �������, �������������� ���� ������ ���� ������(����������)

private slots:
    //������ � GUI
    void tcpGUIServerNewConnection (); //������������� GUI
    void tcpGUIServerDisconnect () ;   //������������ GUI
    void tcpGUIServerRead ();          //������ ��������� �� GUI
    void portRXPacket( int pNum,  bool isRX ); //���� ����� ������
    void portTXPacket( int pNum,  bool isRX ); //���� �������� ������
    void portChangeState( int pNum,  int state ); //����� ������� ����� (0-�������/1-��������/2-������);

    void beaconChangeState( int bNum,  int state ); //����� ������� ����� (0-�������/1-��������);
    void beaconSendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText ); //������� �����

};

#endif // QAPRSCORE_H
