#ifndef QSYSSTATIONLIST_H
#define QSYSSTATIONLIST_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "../../core/include/aprs.h"

class QAPRSStation;
class QAPRSstationList;


class QAPRSStation
{
public:
    QAPRSStation();
    QAPRSStation( QString call );
    QAPRSStation( QString call, QString sym );
    ~QAPRSStation();

    void setParameter( QString par_name, QString par_val );

    QString   call;   //Позывной
    QString   sym;    //Символ
    QString   LH;     //Слышали в последний раз
    QString   lat;    //широта и долгота
    QString   lng;
    QString   grid;
    QString   stcomment; //комментарий к маяку станции
    QString   ststatus;  //статус текст
    QString   via;       //была слышна через

    qreal rlat, rlng;
    QGraphicsPixmapItem *symPic;
    QGraphicsTextItem   *callText;
    QGraphicsRectItem   *callRect;

    QAPRSStation *nextStation;
};

class QAPRSstationList : public QObject
{
    Q_OBJECT
public:
    QAPRSstationList();
    ~QAPRSstationList();

    //станции
    //QAPRSStation  *station[1024];
    //int            stationCount;

    //void stationSetParam( QString call, QString par_name, QString par_val );

    //void stationSetParam( int stIndex, QString par_name, QString par_val );
    //int  getIndexOfStation( QString Call );
    int  getStationCount( );
    //QAPRSStation  *getStation( int stIndex );
    QAPRSStation  *getStation( QString Call );
    void copyFromList( QAPRSstationList *stList );

    QAPRSStation *firstStation; //первая станция в списке
    int stationCount;

    QString lastCall;
    QAPRSStation *lastStation;

signals:

    //сигналы, возникающие у связанных атомов от того, что они эти функции вызвали
    void    SIGstationSetParam( QString call );

public slots:
    void stationSetParam( QString call, QString par_name, QString par_val );

};

#endif // QSYSSTATIONLIST_H
