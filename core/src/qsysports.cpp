#include "../../core/include/qsysports.h"

QSysPorts::QSysPorts() {

    int i;

    for(i=0;i<256;i++){

        port[ i ] = nullptr;

    }

    SysVars = nullptr;

}
