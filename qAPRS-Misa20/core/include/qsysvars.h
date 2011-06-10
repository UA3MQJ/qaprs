/*

����� ��� �������� ��������� ����������. ���� - ������. �������� - ������.

 */

#ifndef QSYSVARS_H
#define QSYSVARS_H

#include <QtCore>

class QSysVar;
class QSysVars;

class QSysVar {
public:
    QSysVar() { next = NULL; prev = NULL; };
    QString  name;
    QString  value;
    QSysVar *next;
    QSysVar *prev;
    bool     stored; //������� ����, ��� ���������� ��������� � ���������
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
