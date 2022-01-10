/*

 Класс атом. Суть класса в том, что он может существовать один, либо в связи с другими экземплярами - атомами.
 Все они связаны меджу собой таким образом, что весь внутренний диалог одного из них видится остальными,
 почти как свой. На основании этого во всех связанных атомах есть одинаковая информация о текущем его состоянии.
 Все сигналы, произошедшие в одном, происходят и в другом. Вызов слота одного - производит вызов слота в других.
 Это делает их полную синхронизацию. Не смотря на то, что они существуют в разных реальностях.
 Ядро существует в одной реальности, GUI в другой, но они взаимодествуют за счет прямой связи атомов.

 Поэтому основной способностью атома, которая должна быть реализована - это поддержание связи между ними.
 Каждая мысль заканчивается \r\n, а формат UTF16.

 В разных реальностях атомы выполняют разные задачи. Основная - хранить общие данные. Атом ядра работает с портами,
 атом gui отображает события.

 2011-06-11 каждая мысль вначале имеет текстовое поле адреса ID_DEST>ID_SRC.
            если мысль адресована конкретному атому, то другие должны игнорировать её.
            если мысль адресована всем, то ID_DEST = 0

 2011-06-17 Способности. Например способность атома иметь свои системные переменные. То есть указатель
            на объект SysVars будет не nullptr и можно будет сразу работать с ними на чтение. Иначе значение
            переменной можно будет получить только асинхронно через запрос значения.

 2011-06-21 запросы к другим атомам надо задерживать, пока не соединился с другими атомами.
            выполнение ответов на запросы надо задерживать пока не загружены все данные.
            для этого использовать очереди.

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
    explicit QAtomix(QObject *parent = nullptr);

    //переменные для работы storage сохраняющего данные на носитель
    bool started;

    bool sysDBNeedDeleteSysPorts;
    bool sysDBNeedDeleteSysBeacons;

    //заменить на
    bool loadingMode; //режим подчиненного атома - в процессе синхронизации с основным атомом

    bool connected; //признак того, что мы связаны с другими атомами

    int     uid;
    QString atomID;

    void    setAtomName( QString sAtomName );
    void    setStorageType( int stp );
    QString atomName( ) { return vatomName; }
    void    startSaveToStorage();

    //добавление способностей атома
    void    addAbStorage(); //добавление атому способности писать и читать в системную БД
    void    addAbSysVars(); //добавление атому способности иметь список своих переменных
    void    addAbPorts(); //добавление атому способности иметь список портов


    //переменные и реализация связи атомов
    //кто-то будет сервер. наверное раньше всех рожденное ядро
    //это главной
    int           im; // 0-server; 1-client
    QTcpServer   *tcpServer;
    QTcpSocket   *tcpServerConnection;
    int           tcpPort;
    QTcpSocket   *tcpClient;
    QByteArray    inData;
    QTextCodec   *sysDecoder; //системный перекодировщик UTF8<->UTF16
    // Все внутренние русские буквы и так же QString'и используют UTF16
    // для передачи через tcp/ip используется перекодировка в UTF8
    QTextCodec   *sysDecoder16; //системный перекодировщик UTF16<->UTF16

    //связь ID_DEST>ID_SRC thought
    void    doThink( QString ID_DEST, QString ID_SRC, QString thought ); //подумать

    int     iStorageType; // -1 - не сохранять никаких данных, 0 - SQLite


    //память атомов
    //системные переменные
    QSysVars     *SysVars;
    //системные порты
    QSysPorts    *SysPorts;
    //системные маяки
    QSysBeacons  *SysBeacons;
    //символы
    QAPRSSymbols *SysSymbols;
    //принятые/переданные пакеты
    QAPRSPacketList *SysPackets;
    //станции
    QAPRSstationList *SysStations;

    QString       SYSDBName;

private:

    QString vatomName;

    void    weThink( QString ID_DEST, QString ID_SRC, QString thought ); //это мы думаем (!!!самому не вызывать)

signals:

    //сигналы, возникающие у связанных атомов от того, что они эти функции вызвали
    void    SIGweThink( QString thought );
    //работа с системными переменными
    void    SIGweSysvarSet( QString VarName, QString VarVal );
    //работа с портами
    void    SIGweSysportSetParam( int PortNum, QString ParName, QString ParVal );
    void    SIGweSysportChangeState( int PortNum, QString PortState );
    //работа с маяками
    void    SIGweSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );
    void    SIGweSysbeaconChangeState( int BcknNum, QString BcknState );

    //принимаем пакет
    void    SIGweportRXPacket( int pNum, bool isRX );
    //передаем пакет
    void    SIGweportTXPacket( int pNum, bool isTX );

    void    SIGportRXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );
    void    SIGportTXAPRSPacket( int pK, int pNum, QString DT, QString To, QString From, QString Via, QString MsgText );

    void    SIGstationSetParam( QString call, QString par_name , QString par_val );

    //этот сигнал можно отлавливать, чтобы показать что сейчас делает хранилище
    //например, показать пользователю чего мы ждем перед закрытием программы
    void    storeProgress( QString msg );

    void    RCVStationPositionInfo( QString Call, QString Sym, QString LH, QString Lat, QString Lng, QString Grid, QString Via, QString Stcomment );
    void    RCVStationStatus( QString Call, QString Ststatus );

    void    RCVRXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
    void    RCVTXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );

    void    SIGallLoaded( ); //сигнал к тому, что все загрузили
    void    SIGSocketError( ); //Ошибка соединения

protected:
    void    run();

public slots:
    void    say( QString msg );

    //реализация настройки связи
    void    beServer( quint16 port );
    void    beClient( QString address, int port );
    void    SocketError(QAbstractSocket::SocketError socketError);
    void   tcpServerNewConnection (); //подсоединение
    void   tcpServerDisconnect () ;   //отсоединение
    void   tcpServerRead ();          //чтение сообщения
    void   tcpClientread();
    void   tcpClientconnected () ;
    //void tcpClientdisconnected ();
    //void tcpClienterror (QAbstractSocket::SocketError socketError);
    //void tcpClientread();

    void portRXPacket( int pNum,  bool isRX ); //идет прием пакета
    void portTXPacket( int pNum,  bool isRX ); //идет передача пакета
    void portRXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );
    void portTXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );
    void portChangeState( int pNum,  int state ); //Смена статуса порта (0-включен/1-отключен/2-ошибка);

    //обработчик пакетов - сообщений
    void parseAPRSPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString From, QString Via, QString MsgText );
    void parseAPRSPositionPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString Via, QString From, QString MsgText );
    void parseAPRSStatusPacket( int K, int pNum, QString TRX, QString DateTime, QString To, QString Via, QString From, QString MsgText );

    void beaconChangeState( int bNum,  int state ); //Смена статуса маяка (0-включен/1-отключен);
    void beaconSendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText ); //Посылка маяка


    //это методы доступные связанным атомам. работать нужно именно через них!
    //на самом деле они ничего не делают а заставляют все связанные ядра узнать
    //о необходимости этих действий. то есть все(we) атомы должны это сделать.
    //а уже выполнение этих команд делают методы с припиской (DO)
    //
    //
    //работа с системными переменными
    //
    //установить системную переменную
    void weSysvarSet( QString VarName, QString VarVal );
    //запросить все системные переменные
    void weSysvarReq();


    //работа с портами
    //
    //открыть все порты
    void weSysportOpenAll( );
    //закрыть все порты
    void weSysportCloseAll( );
    //запросить все порты
    //передаваться будет список созданных портов: номер/тип/значение а так же запрашиваться статус
    void weSysportReqPorts( );
    //открытие указанного порта
    void weSysportOpen( int PortNum );
    //закрытие указанного порта
    void weSysportClose( int PortNum );
    //удаление указанного порта
    void weSysportDelete( int PortNum );
    //обновление указанного порта
    void weSysportUpdate( int PortNum );
    //установка параметра порта
    //при установке типа порта - порт создается, если его еще нет
    //если тип не совпадает, то порт удаляется и создается новый
    //поэтому сначала задается тип, имя, а потом все остальные параметры порта
    void weSysportSetParam( int PortNum, QString ParName, QString ParVal );
    //запросить все параметры порта
    void weSysportReqParams( int PortNum );
    //запросить статус порта
    void weSysportReqStatus( int PortNum );
    //принимаем пакет
    void weportRXPacket( int pNum, bool isRX );
    //передаем пакет
    void weportTXPacket( int pNum, bool isRX );
    void weportRXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );
    void weportTXAPRSPacket( int pNum,  QString To, QString From, QString MsgText );


    //работа с маяками
    //
    //установка параметра маяка
    //маяк создается при задании любого из параметров
    void weSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );
    //открытие указанного маяка
    void weSysbeaconOpen( int BcknNum );
    //закрытие указанного маяка
    void weSysbeaconClose( int BcknNum );
    //удаление указанного маяка
    void weSysbeaconDelete( int BcknNum );
    //передача указанного маяка
    void weSysbeaconSend( int BcknNum );
    //передача статуса указанного маяка
    void weSysbeaconStatusSend( int BcknNum );
    //обновление указанного маяка
    void weSysbeaconUpdate( int BcknNum );
    //передаваться будет список созданных маяков
    void weSysbeaconReqBeacons( );
    //запросить все параметры маяка
    void weSysbeaconReqParams( int BcknNum );
    //запросить статус маяка
    void weSysbeaconReqStatus( int BcknNum );
    //открыть все маяки
    void weSysbeaconOpenAll( );
    //закрыть все маяки
    void weSysbeaconCloseAll( );

    //запросить все пакеты
    void weSyspacketsReq( );

    //работа с сообщениями
    //
    void weSendMessage( int portNum, QString To, QString From, QString Via, QString MsgText, bool ACK );

private slots:
    //то же, что и выше, но выше были запросы на действия, а тут уже выполнение
    //но выполняет тот, кто это может. к примеру если sysvars есть у атомов клиента и ядра,
    //то порты есть только в атоме ядра и с ними работает только ядро.
    //так же только ядро сохраняет в ядро.

    //работа с системными переменными
    //
    void DOweSysvarSet( QString VarName, QString VarVal );
    void DOweSysvarReq( QString ID_SRC );

    //работа с портами
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

    //работа с маяками
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
