#include "../../core/include/qsyspackets.h"

QAPRSPacket::QAPRSPacket() {

    next = NULL;
    prev = NULL;

    stored          = false;

}

QAPRSPacket::QAPRSPacket( int K, QString DT, int Port_Num, QString TRX, QString PTo, QString PFrom, QString PVia, QString Message ) {

    next            = NULL;
    prev            = NULL;
    this->K         = K;
    this->DT        = DT;
    this->Port_Num  = Port_Num;
    this->TRX       = TRX;
    this->PTo       = PTo;
    this->PFrom     = PFrom;
    this->PVia      = PVia;
    this->Message   = Message;

    stored          = false;
}


QAPRSPacket::~QAPRSPacket() {


}

QAPRSPacketList::QAPRSPacketList() {

    packetCount = 0;
    packet_Key  = 0;
    first       = NULL;
    last        = NULL;

}

void QAPRSPacketList::addPacket( int K, QString DT, int Port_Num, QString TRX, QString PTo, QString PFrom, QString PVia, QString Message ) {

    if ( first == NULL ) {

        first = new QAPRSPacket( K, DT, Port_Num, TRX, PTo, PFrom, PVia, Message );
        last  = first;

    } else {

        QAPRSPacket *newPacket = new QAPRSPacket( K, DT, Port_Num, TRX, PTo, PFrom, PVia, Message );

        newPacket->prev = last;
        last->next      = newPacket;
        last            = newPacket;

    }

    packetCount = packetCount + 1;


}


QAPRSPacketList::~QAPRSPacketList() {


}
