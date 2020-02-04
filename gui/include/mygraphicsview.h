#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug>

class myGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    myGraphicsView(QObject *parent = 0);


    virtual void wheelEvent(QWheelEvent *event);

signals:
    void    onMouseZoomInOut( );

};

#endif // MYGRAPHICSVIEW_H
