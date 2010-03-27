#ifndef QAPRSBEACON_H
#define QAPRSBEACON_H
#endif
#include <QObject>
#include <QTimer>

class QAPRSBeacon: public QObject
{
    Q_OBJECT

public:
       QAPRSBeacon( QObject *parent = 0 );
       ~QAPRSBeacon();

       //����� ����� � ������ ���� (������������� ����� ��� �������� �����)
       void         setBeaconNum( int bN ) { iBeaconNum = bN; }
       int          beaconNum() { return iBeaconNum; }

       //��������� APRS ���������
       void         setAPRSCall( QString call ) { APRSCall = call; }

       //��������� ��������� ����� (������������� ��������� ����� ��� ��������� �������, ���� ���� ������);
       virtual void setParam( QString ParamName, QString ParamValue );

       //��������� (���������) ���� (��������� ���� � �������������� �����������);
       virtual void openBeacon();

       //�������� ����� (��������� ����);
       virtual void closeBeacon();

       //����������� ������� ����� (�������/��������);
      int           beaconStatus() { return iBeaconStatus; }// 0-�������; 1-��������
       //��������� ������� ����� (�������/��������/������);
      void          setBeaconStatus( int pStatus ) { iBeaconStatus = pStatus; emit changeState( iBeaconNum, pStatus ); }


private:
       int          iBeaconNum; //����� ����� � ������ ���� (������������� ����� ��� �������� �����)
       int          iBeaconStatus; //������ ����� 0-�������; 1-��������

       QString      Call;     //�������� �����
       QString      Lat;      //������
       QString      Lng;      //�������
       int          Interval; //��������
       QString      Unproto;  //���� �����
       QString      Text;     //����� �����
       QString      Sym;      //������ �����
       int          PortNum;  //����, �� ������� �����
       QString      APRSCall; //�������� APRS

       QTimer      *beacontimer; //������ �����

signals:
       //����� ������� ����� (0-�������/1-��������/2-������);
       void         changeState( int bNum,  int state );
       //������� �����
       void         sendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText );

private slots:
       void         onTimer();

};


