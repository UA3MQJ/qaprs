#ifndef STATIONSWINDOW_H
#define STATIONSWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include <QCloseEvent>
#include <QTimer>
#include <QStandardItemModel>
#include "./include/custommodels.h"
#include "../../core/include/aprs.h"
#include "../../core/include/qaprssymbols.h"
#include "../../core/include/qsysvars.h"
#include "ui_stations.h"
//
class StationsWindowImpl : public QMainWindow, public Ui::StationsWindow
{
Q_OBJECT
public:
        StationsWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

        //QStandardItemModel stationsModel;
        StationsModel stationsModel;

        //�������, � ������� �������� ����������� APRS ��������
        //QVariantMap *symNames; //�� ���������
        //QVariantMap *symImgs;  //�� ������������
        QAPRSSymbols *SysSymbols;
        QSysVars     *SysVars;

        QString      genLat; //�������� ���������� �������
        QString      genLng; //�� ��� ��������� ���� � ���������� �� ������ �������

        QTimer       refresh;

private slots:
        void RCVStationPositionInfo( QString Call, QString Sym, QString LH, QString Lat, QString Lng, QString Grid, QString Via, QString Stcomment );
        void RCVStationStatus( QString Call, QString Ststatus );
        void onTimer();

private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent *event );


};
#endif




