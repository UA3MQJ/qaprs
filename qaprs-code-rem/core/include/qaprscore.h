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
    void Start(); //запуск ядра
    //но возможно перед этим потребуется поменять кое какие параметры
    void setGUIPort( int port ); //установка номера TCP/IP порта, по которому ядро ждет GUI
    void setDBName( QString DBName ); //установка имени системной базы данных
    void recreateSYSDB();//пересоздание системной базы данных
    //работа с портами
    int  createPort( int pNum, QString pType );//создание порта определенного типа+запись в БД
    int  createPort( int pNum );//создание порта (данные берутся из БД)
    int  deletePort( int pNum );//удаление порта (если он уже создан)
    int  setPortName( int pNum, QString pName );//задать пользовательское им порта+запись в БД
    int  setPortParam( int pNum, QString pVar, QString pVal );//задать параметр порта+запись в БД
    int  openPort( int pNum );//открытие порта (если он уже создан)
    int  closePort( int pNum );//закрытие порта (если он уже создан)
    void createAllPorts( );//создание всех портов
    void closeAllPorts( );//закрытие всех портов
    void openAllPorts( );//открытие всех портов
    //работа с маяками
    int  createBeacon( int num, int port_num, QString bSym, QString bCall, QString bLat, QString bLng,
                                QString bUnproto, QString bText, QString bInterval);
    int  setBeaconParam( int bNum, QString pVar, QString pVal );//задать параметр маяка+запись в БД
    void createAllBeacons( );//создание всех маяков
    void openAllBeacons( );//открытие всех маяков
    void closeAllBeacons( );//закрытие всех маяков
    int  openBeacon( int bNum );//открытие маяка (если он уже создан)
    int  closeBeacon( int bNum );//закрытие маяка (если он уже создан)
    int  deleteBeacon( int bNum );//удаление маяка (если он уже создан)

private:
    //системная БД
    QSqlDatabase  db;
    QString       SYSDBName;
    //сервер и соединение для работы с клиентской частью
    QTcpServer   *tcpGUIServer;
    QTcpSocket   *tcpGUIServerConnection;
    int           tcpGUIPort;
    //каждый блок данных, полученный от соединения, буть то один байт или группа
    //добавляются в массив. если есть символ перевода строки, то вырезаем кусок
    //до него и обрабатываем, как команду
    QByteArray    inData;

    //обработка команды от GUI
    void          doCmd( QByteArray cmdText );

    QString       APRSCall; //позывной APRS

    QString QStringToHEX( QString Text );

    //порты
    QAPRSPort     *port[256];
    //маяки
    QAPRSBeacon   *beacon[256];

signals:
    void stop(); //по этому сигналу приложение или процесс, поддерживающий ядро должен быть закрыт(остановлен)

private slots:
    //работа с GUI
    void tcpGUIServerNewConnection (); //подсоединение GUI
    void tcpGUIServerDisconnect () ;   //отсоединение GUI
    void tcpGUIServerRead ();          //чтение сообщения от GUI
    void portRXPacket( int pNum,  bool isRX ); //идет прием пакета
    void portTXPacket( int pNum,  bool isRX ); //идет передача пакета
    void portChangeState( int pNum,  int state ); //Смена статуса порта (0-включен/1-отключен/2-ошибка);

    void beaconChangeState( int bNum,  int state ); //Смена статуса маяка (0-включен/1-отключен);
    void beaconSendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText ); //Посылка маяка

};

#endif // QAPRSCORE_H
