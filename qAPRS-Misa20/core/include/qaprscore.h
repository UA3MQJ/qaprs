#ifndef QAPRSCORE_H
#define QAPRSCORE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include "../../core/include/qatomix.h"

class QAPRSCore : public QObject
{
    Q_OBJECT
public:
    QAPRSCore();
    ~QAPRSCore();

    void setWorkPort( int port );
    void Start(); //запуск €дра

    QTextCodec* sysDecoder; //системный перекодировщик UTF8<->UTF16
    // ¬се внутренние русские буквы и так же QString'и используют UTF16
    // дл€ передачи через tcp/ip используетс€ перекодировка в UTF8

    QTextCodec* packetDecoder; //перекодировщик дл€ преобразовании€
    //8-ми битных пакетных данных в неведомом формате в UNICODE
    //например, если русифицировать UI_View, то данные в IBM 866

    QAtomix *atomCORE;

private:
    //системна€ Ѕƒ
    QSqlDatabase  db;
    QString       SYSDBName;
    int           workPort;

signals:
    void stop(); //по этому сигналу приложение или процесс, поддерживающий €дро должен быть закрыт(остановлен)


};

#endif // QAPRSCORE_H
