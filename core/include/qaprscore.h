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
    void Start(); //запуск ядра

    QTextCodec* sysDecoder; //системный перекодировщик UTF8<->UTF16
    // Все внутренние русские буквы и так же QString'и используют UTF16
    // для передачи через tcp/ip используется перекодировка в UTF8

    QTextCodec* packetDecoder; //перекодировщик для преобразованиия
    //8-ми битных пакетных данных в неведомом формате в UNICODE
    //например, если русифицировать UI_View, то данные в IBM 866

    QAtomix *atomCORE;

private:
    //системная БД
    QSqlDatabase  db;
    QString       SYSDBName;
    int           workPort;

signals:
    void stop(); //по этому сигналу приложение или процесс, поддерживающий ядро должен быть закрыт(остановлен)


};

#endif // QAPRSCORE_H
