#ifndef QSYSPACKETS_H
#define QSYSPACKETS_H

#include <QObject>
#include <QDateTime>

class QAPRSPacket
{
public:
    QAPRSPacket();
    QAPRSPacket( int K, QString DT, int Port_Num, QString TRX, QString PTo, QString PFrom, QString PVia, QString Message );
    ~QAPRSPacket();

    int       K;
    QString   DT;
    int       Port_Num;
    QString   TRX;
    QString   PTo;
    QString   PFrom;
    QString   PVia;
    QString   Message;

    QAPRSPacket *next;
    QAPRSPacket *prev;

    bool     stored; //������� ����, ��� ����� �������� � ���������
};

class QAPRSPacketList : public QObject
{
    Q_OBJECT
public:
    QAPRSPacketList();
    ~QAPRSPacketList();

    QAPRSPacket *first; //������ ����� � ������ (� �� ������ ��������)
    QAPRSPacket *last; //������ ����� � ������ (� �� ������ ��������)
    int packetCount;
    //����� ������ �� �������
    int packet_Key;

public slots:
    void addPacket( int K, QString DT, int Port_Num, QString TRX, QString PTo, QString PFrom, QString PVia, QString Message );


};


#endif // QSYSPACKETS_H
