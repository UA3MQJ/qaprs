#include "../../core/include/qsysstationlist.h"

QAPRSStation::QAPRSStation() {

    call     = "";
    sym      = "";
    symPic   = NULL;
    callText = NULL;
    callRect = NULL;

    nextStation = NULL;

}

QAPRSStation::QAPRSStation( QString call ) {

    this->call = call;
    sym      = "";
    symPic   = NULL;
    callText = NULL;
    callRect = NULL;

    nextStation = NULL;

}

QAPRSStation::QAPRSStation( QString call, QString sym ) {

    this->call = call;
    this->sym  = sym;
    symPic   = NULL;
    callText = NULL;
    callRect = NULL;

    nextStation = NULL;

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

    firstStation = NULL;
    stationCount = 0;

    lastCall = "";
    lastStation = NULL; //������ �� ��������� ������������� �������

}

QAPRSstationList::~QAPRSstationList() {


}

void QAPRSstationList::stationSetParam( QString call, QString par_name, QString par_val ) {

    //qDebug() << "QAPRSstationList::stationSetParam call=" << call << " par_name=" << par_name << " par_val=" << par_val;

    //������ �������� �������, � ���� �� ��� � ������ �� ������� ���������

    QAPRSStation *stationPtr; //��������� �� �������
    QAPRSStation *newstationPtr; //��������� �� ����� �������

    if ( call == lastCall ) { //���������� ������ �� ��������� ������� � ��������. ���� ���������, �� �� ����

        stationPtr = lastStation;

    } else { //����� ���� � ������

        stationPtr = firstStation; //� ������ ������

        if ( stationPtr == NULL ) { //���� ����� NULL �� �� ����� ������� ��� � ������ - ���������

            //qDebug() << "add first station";

                        newstationPtr = new QAPRSStation::QAPRSStation( call );
            stationCount  = stationCount + 1;
            firstStation  = newstationPtr;
            stationPtr    = newstationPtr;

        } else { //����� ����, ���� �� ������ ������� � ������ ��������

            while ( stationPtr->call != call ){

                stationPtr = stationPtr->nextStation; //��������� �� ���������
                if ( stationPtr == NULL ) { //�� ���� ��������� �� ��������� = NULL �� �� ����� � ����� ��������
                    //qDebug() << "doshli do konca i dobavili call="<<call;
                    newstationPtr = new QAPRSStation::QAPRSStation( call );
                    stationCount  = stationCount + 1;
                    newstationPtr->nextStation = firstStation;
                    firstStation  = newstationPtr;
                    stationPtr    = newstationPtr;
                }

            }

        }

    }

    stationPtr->setParameter( par_name, par_val ); //������������� ��������
    lastCall      = call; //���������� �������� ��������� ���������� �������
    lastStation   = stationPtr;

    emit SIGstationSetParam( call );

}

void QAPRSstationList::copyFromList( QAPRSstationList *stList ) {

    QAPRSStation *stPtr;

    stPtr = stList->firstStation;
    while ( stPtr != NULL ) {

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

    while ( st != NULL ) {

        if ( st->call == Call) return st;

        st = st->nextStation;

    }

    return NULL;

}

