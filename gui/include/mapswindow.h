#ifndef MAPSWINDOW_H
#define MAPSWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include <QPaintEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QWheelEvent>
#include "ui_maps.h"
#include "../../core/include/aprs.h"
#include "../../core/include/qsysstationlist.h"
#include "../../core/include/qaprssymbols.h"

class MvScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MvScene(QObject *parent = nullptr);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


signals:
    void    onMouseMove( QPointF point );




};

//
class MapsWindowImpl : public QMainWindow, public Ui::MapsWindow
{
Q_OBJECT
public:
        MapsWindowImpl( QWidget * parent = nullptr );
        ~MapsWindowImpl();

        QString InfFile;
        QString ImgFile;

        //QGraphicsScene *scene;
        MvScene *scene;
        QGraphicsPixmapItem *map;

        qreal mmx1;     // min значение в map по X
        qreal mmx2;     // max значение в map по X
        qreal mmy1;     // min значение в map по Y
        qreal mmy2;     // max значение в map по Y
        qreal mWidth;   // 0..map.sizeX
        qreal mHeight;  // 0..map.sizeY
        qreal mdx, mdy; // mdx=mmx2-mmx1; mdy=mmy2-mmy1;

        qreal mrx1, mrx2, mry1, mry2;
        qreal mrWidth, mrHeight;
        qreal sdx, sdy, shx, shy;

        /*
         * схема карты мира
         *    180            90            0            90            180
         * 90  +-------------------------------------------------------+  90
         *     |                          (N)                          |
         *     |                          -1                           |
         *     |                                                       |
         * 45  |                                                       |  45
         *     |                                                       |
         *     |                                                       |
         *     |                                                       |
         *  0  |(W)-1                      0                       1(E)|   0
         *     |                                                       |
         *     |                                                       |
         *     |                                                       |
         * 45  |                                                       |  45
         *     |                                                       |
         *     |                           1                           |
         *     |                          (S)                          |
         * 90  +-------------------------------------------------------+  90
         *
         */


        //QAPRSstationList *stationList;
        //станции
        QAPRSstationList *SysStations;

        //массивы, в которых хранятся соответсвия APRS символов
        //QVariantMap *symNames; //их названиям
        //QVariantMap *symImgs;  //их изображениям
        QAPRSSymbols *SysSymbols;


        int rz; //уровень по z следующего объекта

signals:
        //void getStationList();

private slots:
        void zoomInClick();
        void zoomOutClick();
        void zoomInOut();
        void loadMapClick();
        void selectMap( QString mapInfFile, QString mapImgFile );
        void drawMap();
        void setMap( QString mapInfFile, QString mapImgFile );

        //void RCVStationInfo( QString Call, QString Sym, QString LH, QString Lat, QString Lng, QString Grid, QString Via, QString Stcomment );
        void RCVStationInfo( QString Call );

        void onMouseMove( QPointF point );
        void updateStationOnMap( QString Call );

private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent  *event );

};
#endif




