/*

Класс для хранения системных переменных. Ключ - строка. Значение - строка.

 */

#ifndef QSYSVARS_H
#define QSYSVARS_H

#include <QtCore>

class QSysVar;
class QSysVars;

class QSysVar {
public:
    QSysVar() { next = nullptr; prev = nullptr; }
    QString  name;
    QString  value;
    QSysVar *next;
    QSysVar *prev;
    bool     stored; //признак того, что переменная сохранена в хранилище
};

class QSysVars {
public:
    QSysVars( );
    QSysVar  *first;
    QSysVar  *last;
    QString   getVal( QString varName );
    void      delVal( QString varName );
    void      setVal( QString varName, QString varValue );
};

#endif // QSYSVARS_H
