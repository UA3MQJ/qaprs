#include "../../core/include/qaprscore.h"


QAPRSCore::QAPRSCore(){

    //�������� � start ��� ����, ����� ����� ���� ������� �������� �����
    //atomCORE = new QAtomix::QAtomix();
    //atomCORE->setAtomName( "atomCORE" );

    //atomCORE->beServer( 6000 );

    //SYSDBName = "etc/APRS001.base"; //��� ��������� �� �� ���������

    //sysDecoder = QTextCodec::codecForName("UTF-8");

    //packetDecoder = QTextCodec::codecForName("Windows-1250");

    workPort = 6000; //��� �� ���������


}

QAPRSCore::~QAPRSCore(){

}

//��������� �������� ������ �������� ����� (tcp/ip)
void QAPRSCore::setWorkPort( int port ) {

    workPort = port;

}

//������ ����
void QAPRSCore::Start() {

    atomCORE = new QAtomix::QAtomix();
    atomCORE->setAtomName( "atomCORE" );

    atomCORE->beServer( workPort );

    SYSDBName = "etc/APRS001.base"; //��� ��������� �� �� ���������

    sysDecoder = QTextCodec::codecForName("UTF-8");

    packetDecoder = QTextCodec::codecForName("Windows-1251");


    qDebug() << "QAPRSCore::Start";

    qDebug() << "QAPRSCore::Open System Database";

    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( SYSDBName );
    db.open();

    //atomCORE->setStorageType( 0 ); //SQLite
    atomCORE->addAbStorage();
    atomCORE->addAbSysVars();
}
