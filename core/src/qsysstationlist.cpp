#include "../../core/include/qsysstationlist.h"

QAPRSStation::QAPRSStation() {

    call     = "";
    sym      = "";
    symPic   = nullptr;
    callText = nullptr;
    callRect = nullptr;

    nextStation = nullptr;

}

QAPRSStation::QAPRSStation( QString call ) {

    this->call = call;
    sym      = "";
    symPic   = nullptr;
    callText = nullptr;
    callRect = nullptr;

    nextStation = nullptr;

}

QAPRSStation::QAPRSStation( QString call, QString sym ) {

    this->call = call;
    this->sym  = sym;
    symPic   = nullptr;
    callText = nullptr;
    callRect = nullptr;

    nextStation = nullptr;

}

void QAPRSStation::setParameter( QString par_name, QString par_val ){

    if ( par_name == "call" )      call      = par_val;
    if ( par_name == "sym" )       sym       = par_val;
    if ( par_name == "LH" )        LH        = par_val;
    if ( par_name == "lat" ) {
        lat  = par_val;
        rlat = lat2real( par_val );
    }
    if ( par_name == "lng" ) {
        lng  = par_val;
        rlng = long2real( par_val );
    }
    if ( par_name == "grid" )      grid      = par_val;
    if ( par_name == "" ) stcomment = par_val;
    if ( par_name == "ststatus" )   ststatus = par_val;


    if ( par_name == "via" )       via       = par_val;

}


QAPRSStation::~QAPRSStation() {

}

QAPRSstationList::QAPRSstationList() {

    firstStation = nullptr;
    stationCount = 0;

    lastCall = "";
    lastStation = nullptr; //ссылка на последнюю обработаннную станцию

}

QAPRSstationList::~QAPRSstationList() {


}

void QAPRSstationList::stationSetParam( QString call, QString par_name, QString par_val ) {

    //qDebug() << "QAPRSstationList::stationSetParam call=" << call << " par_name=" << par_name << " par_val=" << par_val;

    //задает параметр станции, а если ее нет в списке то сначала добавляет

    QAPRSStation *stationPtr; //указатель на станцию
    QAPRSStation *newstationPtr; //указатель на новую станцию

    if ( call == lastCall ) { //запоминаем ссылку на последнюю станцию и позывной. если совпадает, то не ищем

        stationPtr = lastStation;

    } else { //иначе ищем в списке

        stationPtr = firstStation; //с самого начала

        if ( stationPtr == nullptr ) { //если сразу nullptr то ни одной станции нет в списке - добавляем

            //qDebug() << "add first station";

                        newstationPtr = new QAPRSStation( call );
            stationCount  = stationCount + 1;
            firstStation  = newstationPtr;
            stationPtr    = newstationPtr;

        } else { //иначе ищем, пока не найдем станцию с нужным позывным

            while ( stationPtr->call != call ){

                stationPtr = stationPtr->nextStation; //переходим на следующую
                if ( stationPtr == nullptr ) { //но если указатель на следующую = nullptr то не нашли и нужно добавить
                    //qDebug() << "doshli do konca i dobavili call="<<call;
                    newstationPtr = new QAPRSStation( call );
                    stationCount  = stationCount + 1;
                    newstationPtr->nextStation = firstStation;
                    firstStation  = newstationPtr;
                    stationPtr    = newstationPtr;
                }

            }

        }

    }

    stationPtr->setParameter( par_name, par_val ); //устанавливаем параметр
    lastCall      = call; //запоминаем позывной последней измененной станции
    lastStation   = stationPtr;

    emit SIGstationSetParam( call );

}

void QAPRSstationList::copyFromList( QAPRSstationList *stList ) {

    QAPRSStation *stPtr;

    stPtr = stList->firstStation;
    while ( stPtr != nullptr ) {

        this->stationSetParam( stPtr->call, "sym", stPtr->sym );
        this->stationSetParam( stPtr->call, "lat", stPtr->lat );
        this->stationSetParam( stPtr->call, "lng", stPtr->lng );
        this->stationSetParam( stPtr->call, "grid", stPtr->grid );
        this->stationSetParam( stPtr->call, "LH", stPtr->LH );
        this->stationSetParam( stPtr->call, "stcomment", stPtr->stcomment );
        this->stationSetParam( stPtr->call, "ststatus",  stPtr->ststatus );
        this->stationSetParam( stPtr->call, "via", stPtr->via );

        stPtr = stPtr->nextStation;
    }

}


int  QAPRSstationList::getStationCount( ) {

    return stationCount;

}


QAPRSStation *QAPRSstationList::getStation( QString Call ) {


    QAPRSStation *st;

    st = this->firstStation;

//    if ( getIndexOfStation( Call ) != -1 ) {
//        return this->station[ getIndexOfStation( Call ) ];
//    }

    while ( st != nullptr ) {

        if ( st->call == Call) return st;

        st = st->nextStation;

    }

    return nullptr;

}

