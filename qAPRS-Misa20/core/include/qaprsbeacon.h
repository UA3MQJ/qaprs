#ifndef QAPRSBEACON_H
#define QAPRSBEACON_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

#include "../../core/include/qsysvars.h"

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
       //void         setAPRSCall( QString call ) { APRSCall = call; }

       //��������� VERSION ���������
       //void         setAPRSVersion( QString ver ) { APRSVersion = ver; }
       //QSysDBase     *SYSDB;
       //QAtomix *atomCORE;
       QSysVars  *SysVars;

       //��������� ��������� ����� (������������� ��������� ����� ��� ��������� �������, ���� ���� ������);
       virtual void setParam( QString ParamName, QString ParamValue );
       virtual QString getParam( QString ParamName );
       virtual int getParamCount() {return 10;}; //������ � ����� 10 ����������
       virtual QString getParamName( int parNum );
       virtual QString getParamValue( int parNum );

       virtual void updateBeacon();

       //��������� (���������) ���� (��������� ���� � �������������� �����������);
       virtual void openBeacon();

       //�������� ����� (��������� ����);
       virtual void closeBeacon();

       //����������� ������� ����� (�������/��������);
      int           beaconStatus() { return iBeaconStatus; }// 0-�������; 1-��������
       //��������� ������� ����� (�������/��������/������);
      void          setBeaconStatus( int pStatus ) { iBeaconStatus = pStatus; emit changeState( iBeaconNum, pStatus ); }

      QString       call() { return Call; }
      int           portNum() { return PortNum; }

      void         sendBeacon(); //������� ���� �������������
      void         sendStatus(); //������� ������ �������������

      bool stored; //������� ����, ��� ���� �������� � ���������

private:
       int          iBeaconNum; //����� ����� � ������ ���� (������������� ����� ��� �������� �����)
       int          iBeaconStatus; //������ ����� 0-�������; 1-��������

       QString      Call;     //�������� �����
       QString      Lat;      //������
       QString      Lng;      //�������
       int          Interval; //��������
       int          sInterval; //�������� �������
       QString      Unproto;  //���� �����
       QString      Text;     //����� �����
       QString      sText;    //����� �������
       QString      Sym;      //������ �����
       int          PortNum;  //����, �� ������� �����
       //QString      APRSCall; //�������� APRS
       //QString      APRSVersion; //������


       QTimer      *beacontimer; //������ �����
       QTimer      *statustimer; //������ ������ ������

signals:
       //����� ������� ����� (0-�������/1-��������/2-������);
       void         changeState( int bNum,  int state );
       //������� �����
       void         sendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText );

private slots:
       void         onTimer();
       void         onSTimer();

};


#endif
