#include "include/mygraphicsview.h"
#include <qmath.h>

myGraphicsView::myGraphicsView(QObject *parent) : QGraphicsView()
{

}

void myGraphicsView::wheelEvent(QWheelEvent *event)
{

//====================================
    double numDegrees = -event->delta() / 8.0;
    double numSteps   = numDegrees / 15.0;
    double factor     = qPow( 1.125, numSteps );
    scale( factor, factor );

    emit onMouseZoomInOut();

}

