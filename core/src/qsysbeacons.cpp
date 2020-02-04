#include "../../core/include/qsysbeacons.h"

QSysBeacons::QSysBeacons() {

    int i;

    for(i=0;i<256;i++){

        beacon[ i ] = nullptr;

    }

}
