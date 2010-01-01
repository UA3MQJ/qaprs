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
#include "include/qaprscore.h"
#include "ui_maps.h"

class aprsStation : public QGraphicsPixmapItem {
public:
    QString call;
        QString lat, lng;
    qreal rlat, rlng;
    QGraphicsTextItem *callText;
    QGraphicsRectItem *callRect;

    aprsStation ( QGraphicsItem * parent = 0, QGraphicsScene * scene = 0  );
    void setParams( QString ncall, QString nlat, QString nlng, QString npix, QGraphicsScene * nscene );
    ~aprsStation();
};

class aprsMap : public QGraphicsPixmapItem {
public:
    QString mapName;

    aprsMap ( QGraphicsItem * parent = 0, QGraphicsScene * scene = 0  );
    ~aprsMap();
};

class MvScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MvScene(QObject *parent = 0);

    QTextEdit *qthEdit;
    QTextEdit *msgEdit;
    QTextEdit *posEdit;

    qreal mmx1;     // min значение в map по X
    qreal mmx2;     // max значение в map по X
    qreal mmy1;     // min значение в map по Y
    qreal mmy2;     // max значение в map по Y
    qreal mWidth;   // 0..map.sizeX
    qreal mHeight;  // 0..map.sizeY
    qreal mdx, mdy; // mdx=mmx2-mmx1; mdy=mmy2-mmy1;
    //qreal mx, my;
    //qreal px, py;
    //real load map values
    qreal mrx1, mrx2, mry1, mry2;
    qreal mrWidth, mrHeight;
    qreal sdx, sdy, shx, shy;

    aprsMap  * map;
    aprsStation  * obj;
    QList<aprsStation *> stationList;
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
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        int loadMap(QString mapName);
        void drawMap();

};


//
class MapsWindowImpl : public QMainWindow, public Ui::MapsWindow
{
Q_OBJECT
public:
        MapsWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

        QSqlDatabase *db;
        QAPRSCore *APRSCore;
        MvScene * scene;

        int loadMap(QString mapName);
        void unloadMap();
        QString getDBVal( QString varname );
        void setDBVal( QString varname, QString varval );

private slots:
        void zoomInClick();
        void zoomOutClick();
        void loadMapClick();

private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent *event );


};
#endif




