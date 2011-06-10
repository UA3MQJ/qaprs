#include "../../core/include/qaprspacket.h"


///////����������� AX �����///////////////////////////////////////////
QAPRSAXUIPacket::QAPRSAXUIPacket( QObject *parent) {

}

QAPRSAXUIPacket::QAPRSAXUIPacket( QString To, QString From, QString MsgText ) {
    QByteArray ta;
    this->clear();

    QByteArray TCall;
    QByteArray FCall;
    QByteArray ToCall;
    QByteArray FromCall;
    QByteArray Packet;
    QByteArray Text;

    QTextCodec* decoder1 = QTextCodec::codecForName("IBM 866");

    int TSID=0;
    int TH=0; //��� ������������
    TCall.clear();
    TCall.append( To );
    FCall.clear();
    FCall.append( From );
    Text.clear();

    Text.append( decoder1->fromUnicode( MsgText ) );

    //qDebug() << "QAPRSAXUIPacket text=" << MsgText;

    //������� ��� ������� � ������ ������� �����
    FCall = FCall.toUpper();
    //while (FCall.indexOf(" ")!=1)
        FCall.replace(QByteArray(" "), QByteArray(""));

    //to call+sid
    ToCall.clear();
    ToCall.append(TCall.toUpper() );
    if (TCall.indexOf("-")>=0) {
        TSID = TCall.right(TCall.length()-TCall.indexOf("-")-1).toInt();
        TSID = TSID & 15;
        ToCall = ToCall.left(ToCall.indexOf("-"));
    };
    ToCall = ToCall.leftJustified(6, ' ');

    TSID = (TSID) + 112; //112 = 01110000
                         //      0CRRSSID
    ToCall.append(TSID);

    //from call(s)+ssid(s)
    FCall = FCall.toUpper();
    if (FCall.indexOf( '>' )!=-1) FCall[ FCall.indexOf( '>' ) ]=',';
    FromCall.clear();

    do {
        int subindex, subindex2, nextindex;
        //����� ���� �������� ��� ���������
        subindex  = (FCall.indexOf(",")==-1)?(FCall.length()-1):(FCall.indexOf(",")-1);
        nextindex = subindex;
        //���� �� ssid? � ����� ������ �� 0 �� subindex2 �������� ������ ��������
        //���� ssid �� ������, �� ���������
        // UA3MQJ-13,WIDE
        // 0    ^  ^^-nextindex
        //      |  +--subindex
        //      +-----subindex2

        subindex2 = (FCall.left(subindex+1).indexOf("-")==-1)?subindex:(FCall.left(subindex+1).indexOf("-")-1);

        TSID = 0;
        TH   = 0;
        QByteArray ttt = FCall.mid(subindex2+2, subindex-subindex2-1);
        if (FCall.left(subindex+1).indexOf("*")>=0) { //retranslate bit
            TH    = 64;
            if (subindex==subindex2) {
                subindex--;
                subindex2--;
            } else subindex--;
        }
        if (FCall.left(subindex+1).indexOf("-")>=0) { //SSID

            TSID  = (FCall.mid(subindex2+2, subindex-subindex2-1).toInt()) ;
        }
        TSID  = 48 + TSID + TH;


        //FromCall.append(FCall.left(subindex2+1).leftJustified(6, ' '));
        //!!!��� ������� ������� �������� � ���������
        FromCall.append(FCall.left(subindex2+1).leftJustified(6, ' ').left( 6 ));

        FromCall.append(TSID);

        FCall = FCall.mid( nextindex+2 ); //delete first CALL

    } while (FCall.length()>0);

    this->clear();
    this->append(ToCall);
    this->append(FromCall);

    for(int i=0;i<(this->length());i++) this->data()[i]=(this->data()[i]<<1);
    //� ��������� ����� ������ ��� 0 ������������� � 1
    this->data()[this->length()-1] = this->data()[this->length()-1] + 1;
    //��������� control byte
    this->append(3); // 0011 - U-Frame
    //��������� PID byte
    this->append(240); // 11110000 - �������� 3-�� ������ �� ������������
    //��������� ��������� ���������
    this->append( Text );
    //��������� ������� ������
    this->append( 13 );

    unsigned short  crc;
    char   *pcBlock;
    unsigned short  len;


    pcBlock = this->data();
    len     = this->length();

    crc = qChecksum(pcBlock, len);
    //��������� CRC
    this->append( crc & 0xFF );
    this->append( crc>>8 );

}


QAPRSAXUIPacket::~QAPRSAXUIPacket( ) {

}

///////����� AXIP///////////////////////////////////////////
QAPRSAXIPPacket::QAPRSAXIPPacket( QObject *parent) {

}

QAPRSAXIPPacket::QAPRSAXIPPacket( QString To, QString From, QString MsgText ) {
//��������� ��������� � AX �������
    QAPRSAXUIPacket AXPacket( To, From, MsgText );

    this->clear();
    this->append( AXPacket );

}


QAPRSAXIPPacket::~QAPRSAXIPPacket( ) {

}
//////����� AGW///////////////////////////////////////////////////////////////
QAPRSAGWPacket::QAPRSAGWPacket( QObject *parent) {

}

QAPRSAGWPacket::QAPRSAGWPacket( QString To, QString From, QString MsgText ) {

    QAPRSAXUIPacket AXPacket( To, From, MsgText );

    QByteArray AGWHeader;
    //020000004b00000000000000000000000000000000000000000000002d0000000000000000+����� ��� CRC, eof 0x0d
    //                                                         ^-����� ������ (pack len - 36)
    //         ^- 'K' - ������� raw ������
    // ^-����� ����� � ���� agw ( agwPort-1 )
    AGWHeader.clear();
    AGWHeader.fill(0, 37);
    AGWHeader.data()[4] = 0x4b;

    //AXPacket = AXPacket.left( AXPacket.length() - 2 ); //delete CRC
    //����� AX ����� � �������� �� ���� CRC
    AXPacket.remove( AXPacket.length() - 2, 2 );
    AXPacket.prepend( AGWHeader );

    AXPacket.data()[28] = (AXPacket.length() - 36);
    AXPacket.data()[29] = (AXPacket.length() - 36)>>8;

    this->clear();
    this->append( AXPacket );

}


QAPRSAGWPacket::~QAPRSAGWPacket( ) {

}
//////gfrtn AGWRX (����� �� ���� AGW � �������)///////////////////////////////////////////////////////////////
QAPRSAGWRXPacket::QAPRSAGWRXPacket( QObject *parent) {

}

QAPRSAGWRXPacket::QAPRSAGWRXPacket( QString To, QString From, QString MsgText ) {

    QAPRSAXUIPacket AXPacket( To, From, MsgText );

    QByteArray AGWHeader;
    //020000004b00000000000000000000000000000000000000000000002d0000000000000000+����� ��� CRC, eof 0x0d
    //                                                         ^-����� ������ (pack len - 36)
    //         ^- 'K' - ������� raw ������
    // ^-����� ����� � ���� agw ( agwPort-1 )
    AGWHeader.clear();
    AGWHeader.fill(0, 37);
    AGWHeader.data()[4] = 0x4b;

    //AXPacket = AXPacket.left( AXPacket.length() - 2 ); //delete CRC
    //����� AX ����� � �������� �� ���� CRC
    AXPacket.remove( AXPacket.length() - 2, 2 );
    AXPacket.prepend( AGWHeader );

    AXPacket.data()[28] = (AXPacket.length() - 36);
    AXPacket.data()[29] = (AXPacket.length() - 36)>>8;

    this->clear();
    this->append( AXPacket );

}


QAPRSAGWRXPacket::~QAPRSAGWRXPacket( ) {

}

///////����� ��� KISS ������///////////////////////////////////////////
QAPRSKISSPacket::QAPRSKISSPacket( QObject *parent) {

}

QAPRSKISSPacket::QAPRSKISSPacket( QString To, QString From, QString MsgText ) {

    QAPRSAXUIPacket AXPacket( To, From, MsgText );

/*                  ������!!!

  �� ��������� KISS ������������ 4 �����������. ��� ������ ���� ��������,
  ���� ������ ��������� � �� ������. ��� ������ � APRS ������ ������
  ���������, ������� ������ �� ���������� �� ��������� - ������� ���.
  ���� �� ���� ����� �������������� ������������ � �������� ������ �
  ���������� �������� ������, �� ���� ������ ����� ����� ���������!!!

           Abbreviation            Description                    Hex value
              FEND                 Frame  End                         C0
              FESC                 Frame  Escape                      DB
              TFEND                Transposed Frame End               DC
              TFESC                Transposed Frame Escape            DD

 */

    this->clear();
    this->append( 0xC0 );                       //FEND
    this->append( char(0) );                    //KISS Command=0 - Data Frame
    this->append( AXPacket.left( AXPacket.length() - 2 ) );                   //AX25 PACKET
    this->append( 0xC0 );                       //FEND

}


QAPRSKISSPacket::~QAPRSKISSPacket( ) {

}
