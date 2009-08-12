#ifndef __APRS_H__
#define __APRS_H__

#include <QtCore>
#include <math.h>
#include <stdlib.h>

QString real2lat(qreal lat);
QString real2long(qreal lng);
qreal lat2real(QString lat);
qreal long2real(QString lng);
qreal calc_distance_law_of_cosines(qreal lat1, qreal lon1, qreal lat2, qreal lon2);
qreal calc_angle(qreal lat1, qreal lon1, qreal lat2, qreal lon2);
QString real2qthloc(qreal lat, qreal lng) ;

#endif // __APRS_H__
