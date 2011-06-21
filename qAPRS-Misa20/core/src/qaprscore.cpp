#include "../../core/include/qaprscore.h"


QAPRSCore::QAPRSCore(){

    //перенесу в start дл€ того, чтобы можно было указать значение порта
    //atomCORE = new QAtomix::QAtomix();
    //atomCORE->setAtomName( "atomCORE" );

    //atomCORE->beServer( 6000 );

    //SYSDBName = "etc/APRS001.base"; //им€ системной Ѕƒ по умолчанию

    //sysDecoder = QTextCodec::codecForName("UTF-8");

    //packetDecoder = QTextCodec::codecForName("Windows-1250");

    workPort = 6000; //это по умолчанию


}

QAPRSCore::~QAPRSCore(){

}

//установка значени€ номера рабочего порта (tcp/ip)
void QAPRSCore::setWorkPort( int port ) {

    workPort = port;

}

//запуск €дра
void QAPRSCore::Start() {

    atomCORE = new QAtomix::QAtomix();
    atomCORE->setAtomName( "atomCORE" );

    atomCORE->beServer( workPort );

    SYSDBName = "etc/APRS001.base"; //им€ системной Ѕƒ по умолчанию

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
