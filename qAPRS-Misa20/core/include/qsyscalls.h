/*

����� ��� �������� �������� �������. ���� - ������. �������� - ������(�� �����).

 */

#ifndef QSYSCALLS_H
#define QSYSCALLS_H

#include <QtCore>

class QSysCall;
class QSysCalls;

class QSysCall {
public:
    QSysCall() { next = NULL; prev = NULL; };
    QString  name;
    //QString  value;
    QSysCall *next;
    QSysCall *prev;
    bool     stored; //������� ����, ��� ���������� ��������� � ���������
};

class QSysCalls {
public:
    QSysCalls( );
    QSysCall  *first;
    QSysCall  *last;
    QString   getCall( QString callName );
    void      delCall( QString callName );
    void      addCall( QString callName );
};

#endif // QSYSCALLS_H
