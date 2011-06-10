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
    void Start(); //������ ����

    QTextCodec* sysDecoder; //��������� �������������� UTF8<->UTF16
    // ��� ���������� ������� ����� � ��� �� QString'� ���������� UTF16
    // ��� �������� ����� tcp/ip ������������ ������������� � UTF8

    QTextCodec* packetDecoder; //�������������� ��� ���������������
    //8-�� ������ �������� ������ � ��������� ������� � UNICODE
    //��������, ���� �������������� UI_View, �� ������ � IBM 866

    QAtomix *atomCORE;

private:
    //��������� ��
    QSqlDatabase  db;
    QString       SYSDBName;
    int           workPort;

signals:
    void stop(); //�� ����� ������� ���������� ��� �������, �������������� ���� ������ ���� ������(����������)


};

#endif // QAPRSCORE_H
