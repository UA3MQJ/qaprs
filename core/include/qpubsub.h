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

#ifndef QPUBSUB_H
#define QPUBSUB_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QDateTime>
#include <QRandomGenerator>


class QPubsub : public QThread
{
    Q_OBJECT
public:
    explicit QPubsub(QObject *parent = nullptr);

    bool connected; //признак того, что мы связаны с другими атомами

    int     uid;
    QString atomID;
    bool started; //запущен run

    void    setName(QString sName);
    QString getName();



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

private:

    QString name;
    QMap<int,QTcpSocket *> SClients;

signals:
    void    SIGSocketError( ); //Ошибка соединения

protected:
    void    run();

public slots:

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

    void    debug(QString msg);

private slots:

};

#endif // QPUBSUB_H
