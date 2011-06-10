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

       //номер маяка в списке ядра (присваивается ядром при создании маяка)
       void         setBeaconNum( int bN ) { iBeaconNum = bN; }
       int          beaconNum() { return iBeaconNum; }

       //установка APRS позывного
       //void         setAPRSCall( QString call ) { APRSCall = call; }

       //установка VERSION позывного
       //void         setAPRSVersion( QString ver ) { APRSVersion = ver; }
       //QSysDBase     *SYSDB;
       //QAtomix *atomCORE;
       QSysVars  *SysVars;

       //Установка параметра маяка (устанавливает параметры маяка без изменения текущих, если маяк открыт);
       virtual void setParam( QString ParamName, QString ParamValue );
       virtual QString getParam( QString ParamName );
       virtual int getParamCount() {return 10;}; //сейчас у маяка 10 параметров
       virtual QString getParamName( int parNum );
       virtual QString getParamValue( int parNum );

       virtual void updateBeacon();

       //открывает (запускает) маяк (запускает маяк с установленными параметрами);
       virtual void openBeacon();

       //Закрытие маяка (закрывает маяк);
       virtual void closeBeacon();

       //Определение статуса маяка (включен/отключен);
      int           beaconStatus() { return iBeaconStatus; }// 0-включен; 1-отключен
       //Установка статуса порта (включен/отключен/ошибка);
      void          setBeaconStatus( int pStatus ) { iBeaconStatus = pStatus; emit changeState( iBeaconNum, pStatus ); }

      QString       call() { return Call; }
      int           portNum() { return PortNum; }

      void         sendBeacon(); //послать маяк принудительно
      void         sendStatus(); //послать статус принудительно

      bool stored; //признак того, что маяк сохранен в хранилище

private:
       int          iBeaconNum; //номер маяка в списке ядра (присваивается ядром при создании маяка)
       int          iBeaconStatus; //статус маяка 0-включен; 1-отключен

       QString      Call;     //позывной маяка
       QString      Lat;      //широта
       QString      Lng;      //долгота
       int          Interval; //интервал
       int          sInterval; //интервал статуса
       QString      Unproto;  //путь маяка
       QString      Text;     //текст маяка
       QString      sText;    //текст статуса
       QString      Sym;      //символ маяка
       int          PortNum;  //порт, на который маяки
       //QString      APRSCall; //позывной APRS
       //QString      APRSVersion; //ВЕРСИЯ


       QTimer      *beacontimer; //таймер маяка
       QTimer      *statustimer; //таймер статус текста

signals:
       //Смена статуса порта (0-включен/1-отключен/2-ошибка);
       void         changeState( int bNum,  int state );
       //посылка маяка
       void         sendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText );

private slots:
       void         onTimer();
       void         onSTimer();

};


#endif
