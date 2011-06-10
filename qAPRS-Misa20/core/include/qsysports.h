/*

Класс для хранения системных портов.

 */

#ifndef QSYSPORTS_H
#define QSYSPORTS_H

#include "../../core/include/qaprsports.h"
#include "../../core/include/qsysvars.h"

class QSysPorts;

class QSysPorts {
public:
    QSysPorts( );
    QAPRSPort *port[256];
    QSysVars  *SysVars;
};

#endif // QSYSPORTS_H
