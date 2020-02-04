#include "../../core/include/qaprscore.h"


QAPRSCore::QAPRSCore(){

    //перенесу в start для того, чтобы можно было указать значение порта
    //atomCORE = new QAtomix::QAtomix();
    //atomCORE->setAtomName( "atomCORE" );

    //atomCORE->beServer( 6000 );

    //SYSDBName = "etc/APRS001.base"; //имя системной БД по умолчанию

    //sysDecoder = QTextCodec::codecForName("UTF-8");

    //packetDecoder = QTextCodec::codecForName("Windows-1250");

    workPort = 6000; //это по умолчанию


}

QAPRSCore::~QAPRSCore(){

}

//установка значения номера рабочего порта (tcp/ip)
void QAPRSCore::setWorkPort( int port ) {

    workPort = port;

}

//запуск ядра
void QAPRSCore::Start() {

    atomCORE = new QAtomix();
    atomCORE->setAtomName( "atomCORE" );

    atomCORE->beServer( workPort );

    SYSDBName = "etc/APRS001.base"; //имя системной БД по умолчанию
    atomCORE->SYSDBName = SYSDBName;

    sysDecoder = QTextCodec::codecForName("UTF-8");

    packetDecoder = QTextCodec::codecForName("Windows-1251");


    qDebug() << "QAPRSCore::Start";

    qDebug() << "QAPRSCore::Open System Database";

    if (!QDir("etc").exists()) {
        QDir().mkdir("etc");
    }

    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( SYSDBName );
    db.open();

    //atomCORE->setStorageType( 0 ); //SQLite
    atomCORE->addAbStorage();
    atomCORE->addAbSysVars();
}
