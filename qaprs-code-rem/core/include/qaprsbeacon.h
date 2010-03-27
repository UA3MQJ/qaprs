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

       //номер маяка в списке ядра (присваивается ядром при создании маяка)
       void         setBeaconNum( int bN ) { iBeaconNum = bN; }
       int          beaconNum() { return iBeaconNum; }

       //установка APRS позывного
       void         setAPRSCall( QString call ) { APRSCall = call; }

       //Установка параметра маяка (устанавливает параметры маяка без изменения текущих, если маяк открыт);
       virtual void setParam( QString ParamName, QString ParamValue );

       //открывает (запускает) маяк (запускает маяк с установленными параметрами);
       virtual void openBeacon();

       //Закрытие маяка (закрывает маяк);
       virtual void closeBeacon();

       //Определение статуса маяка (включен/отключен);
      int           beaconStatus() { return iBeaconStatus; }// 0-включен; 1-отключен
       //Установка статуса порта (включен/отключен/ошибка);
      void          setBeaconStatus( int pStatus ) { iBeaconStatus = pStatus; emit changeState( iBeaconNum, pStatus ); }


private:
       int          iBeaconNum; //номер маяка в списке ядра (присваивается ядром при создании маяка)
       int          iBeaconStatus; //статус маяка 0-включен; 1-отключен

       QString      Call;     //позывной маяка
       QString      Lat;      //широта
       QString      Lng;      //долгота
       int          Interval; //интервал
       QString      Unproto;  //путь маяка
       QString      Text;     //текст маяка
       QString      Sym;      //символ маяка
       int          PortNum;  //порт, на который маяки
       QString      APRSCall; //позывной APRS

       QTimer      *beacontimer; //таймер маяка

signals:
       //Смена статуса порта (0-включен/1-отключен/2-ошибка);
       void         changeState( int bNum,  int state );
       //посылка маяка
       void         sendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText );

private slots:
       void         onTimer();

};


