/*

Класс для хранения системных маяков.

 */

#ifndef QSYSBEACONS_H
#define QSYSBEACONS_H

#include "../../core/include/qaprsbeacon.h"
#include "../../core/include/qsysvars.h"

class QSysBeacons;

class QSysBeacons {
public:
    QSysBeacons( );
    QAPRSBeacon *beacon[256];
    QSysVars  *SysVars;
};

#endif // QSYSBEACONS_H
