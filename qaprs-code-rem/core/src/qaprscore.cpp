#include "../../core/include/qaprscore.h"

QAPRSCore::QAPRSCore(){

    int i;

    tcpGUIPort = 5000; //по умолчанию ставим порт для работы с GUI - 5000
    //иначе перед вызовом метода Start меняем порт setGUIPort( int port );

    APRSCall = "APZ010";

    SYSDBName = "APRS001.base"; //имя системной БД по умолчанию

    tcpGUIServer = new QTcpServer( this );

    tcpGUIServerConnection = new QTcpSocket(this);

    for(i=0;i<256;i++) port[ i ]   = NULL;
    for(i=0;i<256;i++) beacon[ i ] = NULL;


}

QAPRSCore::~QAPRSCore(){


}

//установка номера TCP/IP порта, по которому ядро ждет GUI
void QAPRSCore::setGUIPort( int port ) {

    if ((port>=0)and(port<=65535)) {
        tcpGUIPort = port;
    } else {
        qDebug() << "QAPRSCore::setGUIPort - port number incorrect";
    }
}

//установка имени системной базы данных
void QAPRSCore::setDBName( QString DBName ) {

    SYSDBName = DBName;

}

//запуск ядра
void QAPRSCore::Start() {

    qDebug() << "QAPRSCore::Start";

    qDebug() << "QAPRSCore::Open System Database";

    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( SYSDBName );
    db.open();

    QSqlQuery query;

    //***открытие системной базы данных***
    //проверяем вдруг база пустая
    query.prepare( "select count(*) as cnt from vars" );
    query.exec();
    query.first();

    if ( query.isActive() ) {
        //если не пустая
        qDebug() << "QAPRSCore::System Database.OK";
        query.exec( "VACUUM" );
    } else {
        //если пустая
        qDebug() << "QAPRSCore::Recreate System Database";
        this->recreateSYSDB();
    }

    query.prepare( "select varname, varval from vars where varname=(:p1)" );
    query.bindValue( ":p1", "APRSCall" );
    query.exec();
    if ( query.next() ) {
        APRSCall = query.value(1).toString();
    }

    qDebug() << "QAPRSCore::Start GUI Server";
    inData.clear();
    tcpGUIServer->listen( QHostAddress::QHostAddress("127.0.0.1"), tcpGUIPort );
    connect( tcpGUIServer, SIGNAL( newConnection() ),
             this, SLOT( tcpGUIServerNewConnection() ) );
/*
    createPort( 100, "AXIP" );
    createPort( 101, "AXIP" );
    createPort( 102, "AXIP" );

    setPortName( 100, "Test port 1" );
    setPortName( 101, "Test port 2" );
    setPortName( 102, "Test port 3" );

    ( 100, "RXPort", "95" );
    ( 101, "RXPort", "96" );
    ( 102, "RXPort", "97" );

    ( 100, "TXPort", "195" );
    ( 101, "TXPort", "196" );
    ( 102, "TXPort", "197" );

    ( 100, "IPAddress", "127.0.0.1" );
    ( 101, "IPAddress", "127.0.0.1" );
    ( 102, "IPAddress", "127.0.0.1" ); */
/*
    createPort( 0, "Inet" );
    setPortName( 0, "Inet Test port" );
    createPort( 1, "AXIP" );
    setPortName( 1, "AXIP Test port" );
    createPort( 2, "AGW" );
    setPortName( 2, "AGW Test port" );
    createPort( 3, "KISS" );
    setPortName( 3, "KISS Test port" );
    createPort( 4, "FLDIGI" );
    setPortName( 4, "FLDigi Test port" );*/

    //createBeacon( 250, 250, "/I", "UA3MQJ", "58.01.83N", "038.51.13E",
    //              "WIDE1-1,WIDE2-2", "Alex@alex.ru", "600");


    createAllPorts();
    openAllPorts();

    createAllBeacons();
    openAllBeacons();

    //createBeacon( 3, 0, "/I", "UA3MQJ", "58.01.83N", "038.51.13E",
    //              "WIDE1-1,WIDE2-2", "Alex@alex.ru", "600");


}

//подсоединение GUI
void QAPRSCore::tcpGUIServerNewConnection () {

    qDebug() << "QAPRSCore::Incoming connection from GUI";

    tcpGUIServerConnection = tcpGUIServer->nextPendingConnection();

    connect(tcpGUIServerConnection, SIGNAL( readyRead() ),
            this, SLOT( tcpGUIServerRead() ) );

    connect(tcpGUIServerConnection, SIGNAL( disconnected () ),
            this, SLOT( tcpGUIServerDisconnect () ) );

    tcpGUIServer->close(); //!!!! поддерживаем только одно подключение от GUI (это пока)

    tcpGUIServerConnection->write( (QByteArray)("QAPRS-Core Rem. v.1.0.0\r\n\r\n") );
    tcpGUIServerConnection->write( (QByteArray)("Type 'HELP' or '/?' for help\r\n\r\n") );

}

//отсоединение GUI
void QAPRSCore::tcpGUIServerDisconnect () {

    qDebug() << "QAPRSCore::GUI disconnect";

    tcpGUIServerConnection->close();
    tcpGUIServerConnection->disconnect();

    tcpGUIServer->listen( QHostAddress::QHostAddress("127.0.0.1"), tcpGUIPort );
    qDebug() << "QAPRSCore::GUI server ready for new connection";

}

//чтение сообщения от GUI
void QAPRSCore::tcpGUIServerRead () {

    QByteArray datagram;


    //while (tcpGUIServerConnection->bytesAvailable()==0);
    datagram = tcpGUIServerConnection->readAll();

    //все данные добавляем к глобальной переменной - массиву принятых данных
    inData.append( datagram );

    //qDebug() << datagram.toHex();

    //qDebug() << datagram;


    //обработка символа backspace - для удаления последнего символа команды
    //возможно потребуется для управления через консоль
    if (inData.indexOf('\b') != -1 ) {

        tcpGUIServerConnection->write( (QByteArray)(" \b") );
        inData = inData.left( inData.indexOf("\b") - 1 ) + inData.mid( inData.indexOf("\b") + 1 );

    }

    //пока есть симолы перевода строки, есть и законченные команды для ядра
    if ( inData.indexOf("\n")>-1 ) {

        while ( inData.indexOf("\n")>-1 ) {

            //берем о посылаем на обработку
            this->doCmd( inData.left( inData.indexOf("\n") - 1 )  ); //обработать команду
            //qDebug() << "docmd " << inData.left( inData.indexOf("\n") - 1 ) ;
            inData = inData.mid( inData.indexOf("\n") + 1 ); //отрезать

        }

    }

}

//обработка команды от GUI
void QAPRSCore::doCmd( QByteArray cmdText ){

    QSqlQuery query;

    QByteArray tmp;

    cmdText = cmdText.trimmed();

    QString cmdStr= cmdText;

    QStringList cmd = cmdStr.split(' ');

    QTextCodec* decoder = QTextCodec::codecForName("UTF-8");

    int i;

    //обработка команды HELP
    if ( ( cmd.at(0).toUpper() == "HELP" ) or ( cmd.at(0) == "/?" ) )  {

        qDebug() << "QAPRSCore::HELP";
        if ( cmd.length() == 1 ) {

            tcpGUIServerConnection->write( (QByteArray)("\r\nHelp page\r\n\r\n") );
            tcpGUIServerConnection->write( (QByteArray)("HELP or /? this page\r\n") );
            tcpGUIServerConnection->write( (QByteArray)("STOP - stop QAPRS-Core\r\n") );
            tcpGUIServerConnection->write( (QByteArray)("SYSVAR set ...\r\n") );
            tcpGUIServerConnection->write( (QByteArray)("EXIT - disconnect from GUI server \r\n\r\n") );

            return;

        } else {

            tcpGUIServerConnection->write( (QByteArray)("HELP error \r\n\r\n") );

            return;
        }

    }

    //обработка команды EXIT
    if ( ( cmd.at(0).toUpper() == "EXIT" ) ) {

        qDebug() << "QAPRSCore::EXIT";
        if ( cmd.length() == 1 ) {

            tcpGUIServerConnection->abort();

            return;

        } else {

            tcpGUIServerConnection->write( (QByteArray)("EXIT error \r\n\r\n") );

            return;
        }

    }

    //обработка команды STOP
    if ( ( cmd.at(0).toUpper() ==  "STOP" ) ){

        if ( cmd.length() == 1 ) {

            emit stop();

            return;

        } else {

            tcpGUIServerConnection->write( (QByteArray)("STOP error \r\n\r\n") );

            return;
        }

    }

    //обработка команды SYSVAR
    if ( ( cmd.at(0).toUpper() ==  "SYSVAR" ) ) {

        qDebug() << "QAPRSCore::SYSVAR SET";
        if ( ( cmd.length() == 2 ) and ( cmd.at(1).toUpper() ==  "SET" ) ) {

            //если просто set то выводим значения всех системных переменных
            qDebug() << "QAPRSCore::SYSVAR list";

            query.prepare( "select varname, varval from vars order by varname" );
            query.exec();
            while (query.next()) {

                tcpGUIServerConnection->write( "SYSVAR SET " + query.value(0).toByteArray() + "=" + decoder->fromUnicode( query.value(1).toString() ) + "\r\n");

                //qDebug() << "SYSVAR SET" + query.value(0).toByteArray() + "=" + query.value(1).toByteArray();

            }
            tcpGUIServerConnection->write( (QByteArray)("\r\n") );

            return;

        } else

        if ( ( cmd.length() > 2 ) and ( cmd.at(1).toUpper() ==  "SET" ) ) {
            //если параметров 3 и более значит проверяем дальше

            //если там нет знака = значит там SYSVAR SET <имя_переменной>
            //следовательно выводим ее значение
            if  ( ( cmd.length() == 3 ) and ( cmd.at( 2 ).indexOf( '=' ) == -1 ) ) {
                //если после set только имя переменной, то вывод значения
                query.prepare( "select varname, varval from vars where varname=(:p1)" );
                query.bindValue( ":p1", cmd.at( 2 ) ); //начиная с пробела, после set
                //qDebug() << "Prosmotr peremennoi " << cmdText;
                query.exec();
                if ( query.next() ) {
                    tcpGUIServerConnection->write( "SYSVAR SET " + query.value(0).toByteArray() + "=" + decoder->fromUnicode( query.value(1).toString() ) + "\r\n\r\n");
                } else {
                    tcpGUIServerConnection->write( (QByteArray)("SYSVAR error (unknown sysvar name)\r\n") );
                }

                return;

            }

            //если знак есть, но последний знак в третьем слове это =
            //значит там SYSVAR SET <имя_переменной>= значит мы ее удаляем
            if ( ( cmd.length() == 3 ) and ( cmd.at( 2 ).right( 1 ) == "=" ) ) {
                //если там просто = то удаляем эту переменную
                query.prepare( "delete from vars where varname=(:p1)" );
                query.bindValue( ":p1", cmd.at( 2 ).left( cmd.at( 2 ).length( ) - 1 ) );
                query.exec();

                tmp.clear();
                tmp.append( "SYSVAR " + cmd.at( 2 ).left( cmd.at( 2 ).length( ) - 1 ) + " deleted" + "\r\n\r\n" );
                tcpGUIServerConnection->write( tmp );

                return;

            }

            //если знак есть, но последний знак в третьем слове это не =
            //значит там SYSVAR SET <имя_переменной>=<какое то значение>
            if ( ( cmd.length() > 2 ) and ( cmd.at( 2 ).right( 1 ) != "=" ) ) {

                //а если <имя_переменной>=<Значение> то
                //удаляем старое значение

                //qDebug() << cmd.at( 2 ).split('=').at(0);
                //qDebug() << decoder->toUnicode( cmdText.mid( cmdText.indexOf( "=" ) + 1 ) );

                //decoder->toUnicode(

                query.prepare( "delete from vars where varname=(:p1)" );
                query.bindValue( ":p1", cmd.at( 2 ).split( '=' ).at( 0 ) );
                query.exec();
                //добавляем новое
                query.prepare( "insert into vars ( varname, varval ) values( (:p1), (:p2) ); " );
                query.bindValue( ":p1", cmd.at( 2 ).split( '=' ).at( 0 ) );
                query.bindValue( ":p2", decoder->toUnicode( cmdText.mid( cmdText.indexOf( "=" ) + 1 ) ) );
                query.exec();
                //qDebug() << "insert " << decoder->toUnicode( cmdText.mid( cmdText.indexOf( "=" ) + 1 ) );

                tmp.clear();
                tmp.append( "SYSVAR SET " + cmd.at( 2 ).split( '=' ).at( 0 ) + "=" + decoder->fromUnicode( cmdText.mid( cmdText.indexOf( "=" ) + 1 ) ) + "\r\n\r\n" );
                tcpGUIServerConnection->write( tmp );

                if ( cmd.at( 2 ).split( '=' ).at( 0 ) == "APRSCall" ) {

                    //qDebug() << "APRSCall set";
                    APRSCall = decoder->fromUnicode( cmdText.mid( cmdText.indexOf( "=" ) + 1 ) );
                    for(i=0;i<256;i++) {
                        if ( port[ i ]   != NULL )   port[ i ]->setAPRSCall( APRSCall );
                        if ( beacon[ i ] != NULL ) beacon[ i ]->setAPRSCall( APRSCall );
                    }

                }

                return;

            }

        }

        tcpGUIServerConnection->write( (QByteArray)("SYSVAR error \r\n\r\n") );
        return;

    }



    //обработка команды SYSPORT
    if ( ( cmd.at(0).toUpper() ==  "SYSPORT" ) ) {
        //qDebug() << "QAPRSCore::SYSPORT";
        if ( cmd.length() > 2 ) {

            if ( cmd.at(1).toUpper().split('=').at( 0 ).toUpper() ==  "PORT_NUM" ) {

                if ( cmd.at(2).toUpper() ==  "OPEN" ) {
                    openPort( cmd.at(1).toUpper().split('=').at( 1 ).toInt( ) );
                    qDebug() << "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" OPEN";
                    return;
                } else
                if ( cmd.at(2).toUpper() ==  "CLOSE" ) {
                    closePort( cmd.at(1).toUpper().split('=').at( 1 ).toInt( ) );
                    qDebug() << "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" CLOSE";
                    return;
                }
                if ( cmd.at(2).toUpper() ==  "DELETE" ) {
                    deletePort( cmd.at(1).toUpper().split('=').at( 1 ).toInt( ) );
                    qDebug() << "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" DELETE";
                    return;
                }
                if ( ( cmd.at(2).toUpper() ==  "GET" ) and ( cmd.at(3).toUpper() ==  "STATE" ) ) {

                    //qDebug() << "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" GET STATE";

                    tmp.clear();
                    tmp.append( "EVENT CHANGESTATE PORT_NUM=" + cmd.at(1).toUpper().split('=').at( 1 ) + "; STATUS=" );
                    // 0-включен; 1-отключен; 2-ошибка.
                    if ( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]!=NULL ) {

                        switch (  port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portStatus() ) {
                                case 0 :
                                          tmp.append( "ON" );
                                          break;
                                case 1 :
                                          tmp.append( "OFF" );
                                          break;
                                case 2 :
                                          tmp.append( "ERROR" );
                                          break;
                        };

                    }
                    tmp.append( ";\r\n" );
                    tcpGUIServerConnection->write( tmp );

                    return;
                }

                //qDebug() << cmd;
                if ( cmd.at(2).toUpper() ==  "SET" ) {
                    //qDebug() << "QAPRSCore::SYSPORT SET";
                    if ( cmd.length() == 4 ) {

                        if ( cmd.at(2).indexOf( '=' ) == -1 ) {
                            //если просто SET и имя переменной, то просматриваем ее

                            //за исключением PORT_NAME и PORT_TYPE данные берутся из БД (а где, бля?)
                            if ( cmd.at(3).toUpper() ==  "PORT_NAME" ) {

                                tmp.clear();
                                tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET PORT_NAME=" );
                                // 0-включен; 1-отключен; 2-ошибка.
                                if ( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]!=NULL ) {
                                    tmp.append( decoder->fromUnicode( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portName() ) );
                                } else { /*tmp.append( "{NULL}" );*/ }

                                tmp.append( "\r\n" );
                                tcpGUIServerConnection->write( tmp );

                                return;
                            }
                            if ( cmd.at(3).toUpper() ==  "PORT_TYPE" ) {

                                tmp.clear();
                                tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET PORT_TYPE=" );
                                // 0-включен; 1-отключен; 2-ошибка.
                                if ( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]!=NULL ) {
                                    tmp.append( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portType() );
                                } else { /*tmp.append( "{NULL}" );*/ }

                                tmp.append( "\r\n" );
                                tcpGUIServerConnection->write( tmp );

                                return;
                            }

                        }

                    }

                    //вывод всех параметров порта
                    if ( cmd.length() == 3 ) {
                        qDebug() << "QAPRSCore::SYSPORT list all params";
                        if ( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ] != NULL) {
                            tmp.clear();
                            tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET PORT_TYPE=" + port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portType() + "\r\n" );
                            tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET PORT_NAME=" + decoder->fromUnicode( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portName() ) + "\r\n" );
                            tcpGUIServerConnection->write( tmp );
                        }


                        query.prepare( " select t4.par_name, t3.par_value "
                                       " from ports t1 "
                                       " left join port_types t2 on t1.port_type_id=t2.port_type_id "
                                       " left join port_param_values t3 on t3.port_num=t1.port_num "
                                       " left join port_param_names t4 on t4.par_code=t3.par_code and t4.port_type_id=t1.port_type_id "
                                       " where t1.port_num=(:p1) " );

                        query.bindValue( ":p1", cmd.at(1).toUpper().split('=').at( 1 ).toInt() );
                        query.exec();
                        while (query.next()) {

                            tmp.clear();
                            tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET " + query.value(0).toByteArray() + "=" + decoder->fromUnicode( query.value(1).toString() ) + "\r\n" );
                            tcpGUIServerConnection->write( tmp );

                            //qDebug() << "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET " + query.value(0).toByteArray() + "=" + query.value(1).toByteArray();

                        }

                        return;
                    }

                    //установка параметра порта
                    if ( cmd.length() > 3 ) {
                        //qDebug() << "QAPRSCore::SYSPORT SET PORT_NUM=" << cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9, cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9 ).indexOf( " " ) ).toInt()
                        //         << " " << cmdText.mid( cmdText.indexOf( "SET" ) + 4, cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) - cmdText.indexOf( "SET" ) - 4 )
                        //         << "=" << decoder->toUnicode( cmdText.mid( cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) + 1 ) );
                        //qDebug() << "Set port parameter";
                        //qDebug() << ;
                        //qDebug() << cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9, cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9 ).indexOf( " " ) );
                        //qDebug() << cmdText.mid( cmdText.indexOf( "SET" ) + 4, cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) - cmdText.indexOf( "SET" ) - 4 );
                        //qDebug() << decoder->toUnicode( cmdText.mid( cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) + 1 ) );

                        //if ( port[ cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9, cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9 ).indexOf( " " ) ).toInt() ] ) {

                            this->setPortParam( cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9, cmdText.mid( cmdText.indexOf( "PORT_NUM=" ) + 9 ).indexOf( " " ) ).toInt(),
                                                cmdText.mid( cmdText.indexOf( "SET" ) + 4, cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) - cmdText.indexOf( "SET" ) - 4 ),
                                                decoder->toUnicode( cmdText.mid( cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) + 1 ) ) );

                        //}

                        return;
                    }

                }

                tcpGUIServerConnection->write( (QByteArray)("SYSPORT error\r\n") );
                return;

            }

            if ( cmd.at(1).toUpper().split('=').at( 0 ).toUpper() ==  "ALL" ) {

                if ( cmd.at(2).toUpper() ==  "OPEN" ) {
                    openAllPorts( );
                    qDebug() << "QAPRSCore::SYSPORT ALL OPEN";
                    return;
                } else
                if ( cmd.at(2).toUpper() ==  "CLOSE" ) {
                    closeAllPorts( );
                    qDebug() << "QAPRSCore::SYSPORT ALL OPEN";
                    return;
                }

                tcpGUIServerConnection->write( (QByteArray)("SYSPORT ALL error\r\n") );
                return;

            }

        }
    }

    //обработка команды SYSBEACON
    if ( ( cmd.at(0).toUpper() ==  "SYSBEACON" ) ) {
        qDebug() << "QAPRSCore::SYSBEACON try send";
        if ( cmd.length() > 2 ) {
            if ( cmd.at(1).toUpper().split('=').at( 0 ).toUpper() ==  "BCN_NUM" ) {
                //qDebug() << cmd;

                ///////////////////////////////////////////////////
                //////////////////////////////////////////////



                                    if ( cmd.at(2).toUpper() ==  "OPEN" ) {
                                        openBeacon( cmd.at(1).toUpper().split('=').at( 1 ).toInt( ) );
                                        qDebug() << "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" OPEN";
                                        return;
                                    } else
                                    if ( cmd.at(2).toUpper() ==  "CLOSE" ) {
                                        closeBeacon( cmd.at(1).toUpper().split('=').at( 1 ).toInt( ) );
                                        qDebug() << "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" CLOSE";
                                        return;
                                    }
                                    if ( cmd.at(2).toUpper() ==  "DELETE" ) {
                                        deleteBeacon( cmd.at(1).toUpper().split('=').at( 1 ).toInt( ) );
                                        qDebug() << "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" DELETE";
                                        return;
                                    }
                                    if ( ( cmd.at(2).toUpper() ==  "GET" ) and ( cmd.at(3).toUpper() ==  "STATE" ) ) {

                                        //qDebug() << "SYSPORT BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" GET STATE";

                                        tmp.clear();
                                        tmp.append( "EVENT CHANGESTATE BCN_NUM=" + cmd.at(1).toUpper().split('=').at( 1 ) + "; STATUS=" );
                                        // 0-включен; 1-отключен; 2-ошибка.
                                        if ( beacon[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]!=NULL ) {

                                            switch ( beacon[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->beaconStatus() ) {
                                                    case 0 :
                                                              tmp.append( "ON" );
                                                              break;
                                                    case 1 :
                                                              tmp.append( "OFF" );
                                                              break;
                                                    case 2 :
                                                              tmp.append( "ERROR" );
                                                              break;
                                            };

                                        }
                                        tmp.append( ";\r\n" );
                                        tcpGUIServerConnection->write( tmp );

                                        return;
                                    }



                //////////////////////////////////
                //////////////////////////////////

                if ( cmd.at(2).toUpper() ==  "SET" ) {

                    if ( cmd.length() == 4 ) {

                        //вывод параметров маяка поотдельности пока не сделано. лениво
                        if ( cmd.at(2).indexOf( '=' ) == -1 ) {
                        //
                        //    //если просто SET и имя переменной, то просматриваем ее
                        //    //за исключением PORT_NAME и PORT_TYPE данные берутся из БД
                        //    if ( cmd.at(3).toUpper() ==  "PORT_NAME" ) {
                        //
                        //        tmp.clear();
                        //        tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET PORT_NAME=" );
                        //        // 0-включен; 1-отключен; 2-ошибка.
                        //        if ( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]!=NULL ) {
                        //            tmp.append( decoder->fromUnicode( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portName() ) );
                        //        } else { /*tmp.append( "{NULL}" );*/ }
                        //
                        //        tmp.append( "\r\n" );
                        //        tcpGUIServerConnection->write( tmp );
                        //
                        //        return;
                        //    }
                        //    if ( cmd.at(3).toUpper() ==  "PORT_TYPE" ) {
                        //
                        //        tmp.clear();
                        //        tmp.append( "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET PORT_TYPE=" );
                        //        // 0-включен; 1-отключен; 2-ошибка.
                        //        if ( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]!=NULL ) {
                        //            tmp.append( port[ cmd.at(1).toUpper().split('=').at( 1 ).toInt() ]->portType() );
                        //        } else { /*tmp.append( "{NULL}" );*/ }
                        //
                        //        tmp.append( "\r\n" );
                        //        tcpGUIServerConnection->write( tmp );
                        //
                        //        return;
                        //    }
                        //

                        /*
                            if ( pVar == "CALL" )     query.prepare( " select call "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "LAT" )      query.prepare( " select lat "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "LNG" )      query.prepare( " select lng "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "INTERVAL" ) query.prepare( " select binterval "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "UNPROTO" )  query.prepare( " select bunproto "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "TEXT" )     query.prepare( " select btext "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "SYM" )      query.prepare( " select sym "
                                                                     " where  beacon_num=(:p1) " );

                            if ( pVar == "PORT_NUM" ) query.prepare( " select port_num "
                                                                     " where  beacon_num=(:p1) " );
                          */


                        }

                    }



                    //вывод всех параметров маяка
                    if ( cmd.length() == 3 ) {
                        qDebug() << "QAPRSCore::SYSBEACON list all params";
                        query.prepare( " select t1.port_num, t2.port_note, t1.sym, t1.call, "
                                       " t1.lat, t1.lng, "
                                       " t1.bUnproto, t1.bText, t1.bInterval "
                                       " from beacons t1  "
                                       " left join ports t2 on t2.port_num=t1.port_num"
                                       " where beacon_num=(:p1) " );


                        query.bindValue( ":p1", cmd.at(1).toUpper().split('=').at( 1 ).toInt() );
                        query.exec();
                        while (query.next()) {

                            tmp.clear();

                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET PORT_NUM=" + query.value(0).toByteArray() + "\r\n" );
                            //tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET PORT_NOTE=" + query.value(1).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET SYM=" + query.value(2).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET CALL=" + query.value(3).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET LAT=" + query.value(4).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET LNG=" + query.value(5).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET UNPROTO=" + query.value(6).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET TEXT=" + query.value(7).toByteArray() + "\r\n" );
                            tmp.append( "SYSBEACON BCN_NUM="+cmd.at(1).toUpper().split('=').at( 1 ).trimmed()+" SET INTERVAL=" + query.value(8).toByteArray() + "\r\n" );
                            tcpGUIServerConnection->write( tmp );

                            //qDebug() << "SYSPORT PORT_NUM="+cmd.at(1).toUpper().split('=').at( 1 )+" SET " + query.value(0).toByteArray() + "=" + query.value(1).toByteArray();

                        }

                        return;

                    }



                    //установка параметра маяка
                    if ( cmd.length() > 3 ) {

                            this->setBeaconParam( cmdText.mid( cmdText.indexOf( "BCN_NUM=" ) + 8, cmdText.mid( cmdText.indexOf( "BCN_NUM=" ) + 8 ).indexOf( " " ) ).toInt(),
                                                cmdText.mid( cmdText.indexOf( "SET" ) + 4, cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) - cmdText.indexOf( "SET" ) - 4 ),
                                                decoder->toUnicode( cmdText.mid( cmdText.indexOf("=", cmdText.indexOf("=") + 1 ) + 1 ) ) );

                        return;
                    }

                }

            }


            if ( cmd.at(1).toUpper().split('=').at( 0 ).toUpper() ==  "ALL" ) {

                if ( cmd.at(2).toUpper() ==  "OPEN" ) {
                    openAllBeacons( );
                    qDebug() << "QAPRSCore::SYSBEACON ALL OPEN";
                    return;
                } else
                if ( cmd.at(2).toUpper() ==  "CLOSE" ) {
                    closeAllBeacons( );
                    qDebug() << "QAPRSCore::SYSBEACON ALL CLOSE";
                    return;
                }

                tcpGUIServerConnection->write( (QByteArray)("SYSBEACON ALL error\r\n") );
                return;

            }

        }

    }

    //qDebug() << cmdText;
    tcpGUIServerConnection->write( (QByteArray)("Error\r\n") );

}

//пересоздание системной базы данных
void QAPRSCore::recreateSYSDB() {

    QSqlQuery query;

    //таблица с типами портов
    query.exec( "drop table port_types" );
    query.exec( "create table port_types (port_type_id int primary key, "
                "port_type_note varchar(100), port_type_not varchar(15) ) " );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(0,'APRS Internet Server Connection', 'Inet')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(1,'KISS TNC', 'KISS')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(2,'AGW CORE', 'AGW')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(3,'FLEX CORE(!)', 'FLEX')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(4,'[X]-NET CORE(!)', 'XNET')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(5,'AXIP', 'AXIP')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(6,'FL Digi', 'FLDIGI')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(7,'VAska', 'VASKA')" );
    query.exec( "insert into port_types (port_type_id, port_type_note, port_type_not) "
                "values(-1,'n\\a', 'n\\a')" );

    //порты
    query.exec( "drop table ports" );
    query.exec( "create table ports (port_num int primary key, "
                "port_type_id int, port_note varchar(50) ) " );

    query.prepare( "insert into ports (port_num, port_type_id, port_note) "
                   "values ((:p1), (:p2), (:p3)) " );

    int i;
    for(i=0;i<256;i++) {
        query.bindValue(":p1", i );
        query.bindValue(":p2", -1 );
        query.bindValue(":p3", "n\\a" );
        query.exec();
        qDebug() << i << "/255";
    }

    //маяки
    query.exec( "drop table beacons" );
    query.exec( "create table beacons (beacon_num int primary key, "
                "port_num int, sym char(50), call char(12), "
                "lat char(9), lng char(10), "
                "bUnproto char(250), bText char(250), bInterval int ) " );

    query.prepare( "insert into beacons (beacon_num) "
                   "values ((:p1)) " );

    for(i=0;i<256;i++) {
        query.bindValue(":p1", i );
        query.exec();
        qDebug() << i << "/255";
    }

    //примеры заполнения
/*
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(0,0,'APRS.NET', 'UA3MAD', 'ua3mad@mail.ru', 3000, '', '57.01.83N', '038.51.13E', '/I') " );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(1,1,'KISS (MIX COM3<->COM7)', 'UA3MAD', 'QRM', 3000, 'WIDE1-1,WIDE2-2', '57.01.83N', '038.51.13E', '\\-') " );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(2,2,'AGW PORT1 - 145 MHz', 'UA3MAD', 'ping', 3000, 'WIDE1-1,WIDE2-2', '57.01.83N', '038.51.13E', '/-')" );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(3,2,'AGW PORT2 - 430 MHz', 'UA3MAD', 'ping', 3000, 'WIDE1-1,WIDE2-2', '57.01.83N', '038.51.13E', '/-')" );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(4,5,'AXIP 1200 MHz host', 'UA3MAD', 'ua3mad@mail.ru', 5000, 'RK3MWI-3', '57.01.83N', '038.51.13E', '/I' )" );
    query.exec( "insert into ports (port_num, port_type_id, port_note, port_call, port_beacon_text, port_beacon_interval, port_unproto_address, port_latitude, port_longitude, port_symbol) "
                "values(5,5,'AXIP RK3MWI host', 'UA3MAD', 'ua3mad@mail.ru', 3000, 'RK3MWI-2', '57.01.83N', '038.51.13E', '/I')" );
*/

    //таблица имен параметров портов
    query.exec( "drop table port_param_names; " );
    query.exec( "create table port_param_names (par_code int, port_type_id int, par_name varchar(20) )" );
    //параметры AXIP порта
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,5,'RXPort')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,5,'IPAddress')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,5,'TXPort')" );
    //параметры порта APRS Internet Server
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,0,'Server1')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,0,'Server2')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,0,'Server3')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(4,0,'User')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(5,0,'Pass')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(6,0,'Filter')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(7,0,'Proxy')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(8,0,'PAuth')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(9,0,'PName')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(10,0,'PPort')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(11,0,'PUser')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(12,0,'PPass')" );

    //параметры порта AGW
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,2,'Host')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,2,'AGWPort')" );
    //параметры порта FLDIGI
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,6,'Host')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,6,'Port')" );
    //параметры порта VASKA
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,7,'Host')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,7,'Port')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,7,'Mode')" );
    //параметры KISS порта
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(1,1,'COM')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(2,1,'Speed')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(3,1,'Command1')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(4,1,'Command2')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(5,1,'Command3')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(6,1,'Command4')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(7,1,'TXDelay')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(8,1,'TXTail')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(9,1,'SlotTime')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(10,1,'Persist')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(11,1,'FullDuplex')" );
    query.exec( "insert into port_param_names (par_code, port_type_id, par_name) "
                "values(12,1,'ExitFromKISS')" );

    //таблица параметров портов
    query.exec( "drop table port_param_values; " );
    query.exec( "create table port_param_values (port_num int , "
                "par_code int, par_value varchar(50) ); " );

    //пример заполнения
    /*
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,1,'127.0.0.1:14000') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,2,'www.aprs.org:14580') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,3,'www.aprs.qrz.ru:14580') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,4,'UA3MQJ') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,5,'-1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(0,6,'filter R/UA') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(1,1,'1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(1,2,'2400') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(2,1,'127.0.0.1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(2,2,'1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(3,1,'localhost') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(3,2,'2') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(4,1,'65000') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(4,2,'127.0.0.1') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(4,3,'65001') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(5,1,'60000') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(5,2,'192.168.0.103') " );
    query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                "values(5,3,'60000') " );
*/

    //таблица с APRS символами
    query.exec( "drop table symbols" );


    //таблица пакетов
    query.exec( "drop table packets" );
    query.exec( "create table packets (K integer primary key, "
                "DT datetime, port_num int, trx char(10), PTo char(20),"
                "PFrom char(20), PVia char(50), Message char(80) ) " );

    //таблица сообщений
    query.exec( "drop table messages" );
    query.exec( "create table messages (packet_K int, MTo varchar(10),Message varchar(250), Mess_ID varchar(6) )" );
    //пример
    //query.exec( "insert into  messages (packet_K, MTo, Message, Mess_ID ) values(0, 'UA3MM', 'Test message', '001') " );

    //таблица со списком станций
    query.exec( "drop table stations" );
    //query.exec( "create table stations (call varchar(10), sym varchar(2), lat varchar(9), lng varchar(10), grid varchar(7), Via varchar(50), length float, deg float, LH DateTime, StComment varchar(100) )" );
    //length and deg for station no need for save. this parameters must be calculated
    query.exec( "create table stations (call varchar(10), sym varchar(2), lat varchar(9), lng varchar(10), grid varchar(7), Via varchar(50), LH DateTime, StComment varchar(100) )" );

    //пример
    //query.exec( "insert into stations (call, sym, lat, lng, grid, length, deg, LH, Via ) "
    //            "values('UA3MAD', '/-', '58.01.84N', '038.51.13E', 'KO98KA', 0, 0, '2009-01-01 08:00:00', 'APU25N,RR3MD')" );

    //таблица с системными переменными
    query.exec( "drop table vars" );
    query.exec( "create table vars (varval varchar(50), varname varchar(50) )" );

    //!!заменить все параметры при релизе!!!
    query.exec( "insert into vars ( varname, varval ) values( 'Call',     'CALL' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'Name',     'NAME' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'QTHN',     'QTH-Name' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'Lat',      '00.00.00N' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'Lng',      '000.00.00E' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'Unproto',  'WIDE1-1,WIDE2-2' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'Beacon',   'NAME, QTH-NAME' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'Symbol',   '/I' ); " );
    query.exec( "insert into vars ( varname, varval ) values( 'APRSCall', 'APZ010' ); " );

    qDebug() << "QAPRSCore::System database created";

}

//создание порта определенного типа
int QAPRSCore::createPort( int pNum, QString pType ) {

    //первым делом все сохраним в системную БД
    QSqlQuery query;
    query.prepare( "update ports set port_type_id = (:p1) where port_num=(:p2) " );
    query.bindValue( ":p2", pNum );

    //если порт с этим номером уже есть, то старый удаляется
    if ( port[ pNum ] != NULL) {

        port[ pNum ]->closePort();
        port[ pNum ]->disconnect();
        delete port[ pNum ];
        port[ pNum ] = NULL;

    }

    if ( pType == "AXIP" ) {

        //всетаки сохраняем
        query.bindValue(":p1", 5 ); //5 - id типа AXIP по таблице port_types
        query.exec();

        port[ pNum ] = new QAPRSAXIPPORT();

        //return 0;
    }

    if ( pType == "Inet" ) {

        //всетаки сохраняем
        query.bindValue(":p1", 0 ); //0 - id типа Inet по таблице port_types
        query.exec();

        port[ pNum ] = new QAPRSINTERNETPORT();

        //return 0;
    }

    if ( pType == "AGW" ) {

        //всетаки сохраняем
        query.bindValue(":p1", 2 ); //2 - id типа AGW по таблице port_types
        query.exec();

        port[ pNum ] = new QAPRSAGWPORT();

        //return 0;
    }

    if ( pType == "KISS" ) {

        //всетаки сохраняем
        query.bindValue(":p1", 1 ); //1 - id типа KISS по таблице port_types
        query.exec();

        port[ pNum ] = new QAPRSKISSPORT();

        //return 0;
    }

    if ( pType == "FLDIGI" ) {

        //всетаки сохраняем
        query.bindValue(":p1", 6 ); //6 - id типа FLDigi по таблице port_types
        query.exec();

        port[ pNum ] = new QAPRSFLDIGIPORT();

        //return 0;
    }

    if ( pType == "VASKA" ) {

        //всетаки сохраняем
        query.bindValue(":p1", 7 ); //6 - id типа VASKA по таблице port_types
        query.exec();

        port[ pNum ] = new QAPRSVASKAPORT();

        //return 0;
    }

    if ( port[ pNum ] != NULL) {

        port[ pNum ]->setPortNum( pNum );
        port[ pNum ]->setAPRSCall( APRSCall );

        connect ( port[ pNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );

        //передать параметры порту
        query.prepare( " select t3.par_name, t2.par_value "
                       " from ports t1 "
                       " left join port_param_values t2 on t1.port_num=t2.port_num "
                       " left join port_param_names t3 on (t3.par_code=t2.par_code and t3.port_type_id=t1.port_type_id) "
                       " where t1.port_num=(:p1) " );
        query.bindValue(":p1", pNum );

        query.exec();
        while (query.next()) {
                          //query.value(6).toString(), query.value(7).toString(), query.value(8).toString() );
            port[ pNum ]->setParam( query.value(0).toString(), query.value(1).toString() );

        }



        return 0;

    } else {

        return -1;

    }

    return -1; //порт не создан

}

//создание порта (данные берутся из БД)
int QAPRSCore::createPort( int pNum ) {

    //если порт с этим номером уже есть, то старый удаляется
    if ( port[ pNum ] != NULL) {

        port[ pNum ]->closePort();
        port[ pNum ]->disconnect();
        delete port[ pNum ];
        port[ pNum ] = NULL;

    }

    QSqlQuery query;
    query.prepare( " select t2.port_type_not, t1.port_note "
                   " from ports t1 "
                   " left join port_types t2 on t1.port_type_id=t2.port_type_id "
                   " where t1.port_num=(:p1) " );

    query.bindValue( ":p1", pNum );

    query.exec();
    query.first();
    QString portType= query.value( 0 ).toString();

    this->createPort( pNum, portType);

    /*
    if ( portType == "AXIP" ) {

        port[ pNum ] = new QAPRSAXIPPORT();
        port[ pNum ]->setPortNum( pNum );
        port[ pNum ]->setPortName( query.value( 1 ).toString() );

        connect ( port[ pNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );

        return 0;
    }

    if ( portType == "Inet" ) {

        port[ pNum ] = new QAPRSINTERNETPORT();
        port[ pNum ]->setPortNum( pNum );
        port[ pNum ]->setPortName( query.value( 1 ).toString() );

        connect ( port[ pNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );

        return 0;
    }

    if ( portType == "AGW" ) {

        port[ pNum ] = new QAPRSAGWPORT();
        port[ pNum ]->setPortNum( pNum );
        port[ pNum ]->setPortName( query.value( 1 ).toString() );

        connect ( port[ pNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );

        return 0;
    }

    if ( portType == "KISS" ) {

        port[ pNum ] = new QAPRSKISSPORT();
        port[ pNum ]->setPortNum( pNum );
        port[ pNum ]->setPortName( query.value( 1 ).toString() );

        connect ( port[ pNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );

        return 0;
    }

    if ( portType == "FLDIGI" ) {

        port[ pNum ] = new QAPRSFLDIGIPORT();
        port[ pNum ]->setPortNum( pNum );
        port[ pNum ]->setPortName( query.value( 1 ).toString() );

        connect ( port[ pNum ], SIGNAL( RXPacket( int, bool ) ), this, SLOT( portRXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( TXPacket( int, bool ) ), this, SLOT( portTXPacket( int, bool ) ) );
        connect ( port[ pNum ], SIGNAL( changeState( int, int ) ), this, SLOT( portChangeState( int, int ) ) );

        return 0;
    } */

    return -1;

}

//задать пользовательское им порта
int QAPRSCore::setPortName( int pNum, QString pName ) {

    //если порт с этим номером уже есть, то задаем имя
    if ( port[ pNum ] != NULL) {

        //в системную базу тоже сохраним
        QSqlQuery query;
        query.prepare( "update ports set port_note = (:p1) where port_num=(:p2) " );
        query.bindValue( ":p1", pName );
        query.bindValue( ":p2", pNum );
        query.exec();

        port[ pNum ]->setPortName( pName );
        return 0;

    } else return -1;

}

//задать параметр порта
int QAPRSCore::setPortParam( int pNum, QString pVar, QString pVal ) {

    QByteArray tmp;
    QSqlQuery query;
    QTextCodec* decoder = QTextCodec::codecForName("UTF-8");

    //если port_type смотрим если отличается от установленного
    if ( pVar == "PORT_TYPE" ) {

        qDebug() << "PORT_TYPE";
        if ( port[ pNum ] == NULL ) {
            this->createPort( pNum, pVal );
        }

        if ( port[ pNum ]->portType() != pVal ) {
            //удаляем порт и создаем порт другого типа
            this->deletePort( pNum );
            this->createPort( pNum, pVal );
        }

        tmp.clear();
        tmp.append( "EVENT CHANGESTATE PORT_NUM=" + QString::number( pNum ) + "; STATUS=OFF;\r\n" );
        tmp.append( "SYSPORT PORT_NUM=" + QString::number( pNum ) + " SET PORT_TYPE=" + pVal + "\r\n" );
        tcpGUIServerConnection->write( tmp );

        return 0;

    }

    //если порт с этим номером уже есть, то задаем параметр
    if ( port[ pNum ] != NULL) {

        //qDebug() << "pNum=" << pNum << "; pVar=" << pVar << "; pVal=" << pVal;

        if ( pVar == "PORT_NAME" ) {

            port[ pNum ]->setPortName( pVal );

            //в системную базу тоже сохраним
            query.prepare( " update ports set port_note=(:p1) "
                           " where port_num=(:p2) " );

            query.bindValue( ":p1", pVal );
            query.bindValue( ":p2", pNum );
            query.exec();

            tmp.clear();
            tmp.append( "SYSPORT PORT_NUM=" + QString::number( pNum ) + " SET " + pVar + "=" + decoder->fromUnicode( pVal ) + "\r\n" );
            tcpGUIServerConnection->write( tmp );

            return 0;

        }


        //в системную базу тоже сохраним
        query.prepare( " delete from port_param_values "
                       " where port_num=(:p1) and par_code in "
                       " (select par_code "
                       " from port_types t1 "
                       " left join port_param_names t2 on t1.port_type_id=t2.port_type_id "
                       " where t1.port_type_not=(:p2) and t2.par_name=(:p3) ) " );

        query.bindValue( ":p1", pNum );
        query.bindValue( ":p2", port[ pNum ]->portType() );
        query.bindValue( ":p3", pVar );
        query.exec();

        query.prepare( " insert into port_param_values( par_code, port_num, par_value ) "
                       " select par_code, (:p1), (:p2) "
                       " from port_types t1 "
                       " left join port_param_names t2 on t1.port_type_id=t2.port_type_id "
                       " where t1.port_type_not=(:p3) and t2.par_name=(:p4)  " );

        query.bindValue( ":p1", pNum );
        query.bindValue( ":p2", pVal );
        query.bindValue( ":p3", port[ pNum ]->portType() );
        query.bindValue( ":p4", pVar );
        query.exec();


        port[ pNum ]->setParam( pVar, pVal );

        tmp.clear();
        tmp.append( "SYSPORT PORT_NUM=" + QString::number( pNum ) + " SET " + pVar + "=" + decoder->fromUnicode( pVal ) + "\r\n" );
        tcpGUIServerConnection->write( tmp );

        return 0;

    } else return -1;

}

//открытие порта
int QAPRSCore::openPort( int pNum ) {

    if ( port[ pNum ] != NULL) {

        // 0-включен; 1-отключен; 2-ошибка.
        port[ pNum ]->openPort();

        return 0;

    } else return -1;

}

//закрытие порта
int QAPRSCore::closePort( int pNum ) {

    //qDebug() << "closePort" << pNum << (port[ pNum ] == NULL);
    if ( port[ pNum ] != NULL) {

        port[ pNum ]->closePort();

        return 0;

    } else return -1;

}

//создание всех портов
void QAPRSCore::createAllPorts( ) {

    QSqlQuery query;

    query.prepare( " select port_num, port_type_id, port_note "
                   " from ports "
                   " where not(port_type_id=-1) "
                   " order by port_num" );

    query.exec();
    while (query.next()) {
        createPort( query.value(0).toInt() );
    }

}

//открытие всех портов
void QAPRSCore::openAllPorts( ) {

    int i;

    for(i=0;i<256;i++) {

        openPort( i );

    }

}

//закрытие всех портов
void QAPRSCore::closeAllPorts( ) {

    int i;

    for(i=0;i<256;i++) {

        closePort( i );

    }

}

//удаление порта
int QAPRSCore::deletePort( int pNum ) {

    //если порт существует, то удаляем его
    if ( port[ pNum ] != NULL) {

        //в системную базу тоже сохраним
        QSqlQuery query;
        query.prepare( "update ports set port_note = (:p1), port_type_id = (:p2) where port_num=(:p3) " );
        query.bindValue( ":p1", "n\\a" );
        query.bindValue( ":p2", -1 );
        query.bindValue( ":p3", pNum );
        query.exec();

        query.prepare( "delete from port_param_values where port_num = (:p1) " );
        query.bindValue( ":p1", pNum );
        //qDebug() << "delete pnum=" << pNum;
        query.exec();

        port[ pNum ]->closePort();
        port[ pNum ]->disconnect();
        delete port[ pNum ];
        port[ pNum ] = NULL;

        QByteArray tmp;

        tmp.clear();
        tmp.append( "EVENT CHANGESTATE PORT_NUM=" + QString::number( pNum ) + "; STATUS=DELETED;\r\n" );
        tcpGUIServerConnection->write( tmp );

        //tmp.append( tr("SYSPORT port_num=%1; port_type_note='';  port_note=''; status=;\r\n").arg( pNum ) );
        //tcpGUIServerConnection->write( tmp );

        return 0;

    } else return -1;

}



void QAPRSCore::portRXPacket( int pNum,  bool isRX ) {

    QByteArray msg;

    //qDebug() << pNum;

    if ( isRX ) {
        msg.append( "EVENT RXPACKET PORT_NUM=" + QString::number( pNum )+"; RX=TRUE;\r\n" );
    } else {
        msg.append( "EVENT RXPACKET PORT_NUM=" + QString::number( pNum )+"; RX=FALSE;\r\n" );
    }

    tcpGUIServerConnection->write( msg );

}

void QAPRSCore::portTXPacket( int pNum,  bool isRX ) {

    QByteArray msg;

    //qDebug() << pNum;

    if ( isRX ) {
        msg.append( "EVENT TXPACKET PORT_NUM=" + QString::number( pNum )+"; TX=TRUE;\r\n" );
    } else {
        msg.append( "EVENT TXPACKET PORT_NUM=" + QString::number( pNum )+"; TX=FALSE;\r\n" );
    }

    tcpGUIServerConnection->write( msg );

}

void QAPRSCore::portChangeState( int pNum,  int state ) {

    QByteArray msg;

    //qDebug() << "portChangeState";

    msg.append( "EVENT CHANGESTATE PORT_NUM=" + QString::number( pNum )+"; STATUS=" );

    // 0-включен; 1-отключен; 2-ошибка.
    switch ( state ) {
            case 0 :
                      msg.append( "ON" );
                      break;
            case 1 :
                      msg.append( "OFF" );
                      break;
            case 2 :
                      msg.append( "ERROR" );
                      break;
    };

    msg.append( ";\r\n" );

    tcpGUIServerConnection->write( msg );

}

//Смена статуса маяка (0-включен/1-отключен);
void QAPRSCore::beaconChangeState( int bNum,  int state ) {

    QByteArray msg;

    //qDebug() << "beaconChangeState";

    msg.append( "EVENT CHANGESTATE BCN_NUM=" + QString::number( bNum )+"; STATUS=" );

    // 0-включен; 1-отключен; 2-ошибка.
    switch ( state ) {
            case 0 :
                      msg.append( "ON" );
                      break;
            case 1 :
                      msg.append( "OFF" );
                      break;
    };

    msg.append( ";\r\n" );

    tcpGUIServerConnection->write( msg );

}


int QAPRSCore::createBeacon( int num, int port_num, QString bSym, QString bCall, QString bLat, QString bLng,
                             QString bUnproto, QString bText, QString bInterval) {


    //первым делом все сохраним в системную БД
    QSqlQuery query;
    query.prepare( " update beacons set port_num=(:P1), sym=(:P2), "
                   "                   call=(:P3), lat=(:P4), lng=(:P5), "
                   "                   bUnproto=(:P6), bText=(:P7), "
                   "                   bInterval=(:P8) "
                   " where beacon_num=(:P9) " );

    query.bindValue( ":p1", port_num );
    query.bindValue( ":p2", bSym );
    query.bindValue( ":p3", bCall );
    query.bindValue( ":p4", bLat );
    query.bindValue( ":p5", bLng );
    query.bindValue( ":p6", bUnproto );
    query.bindValue( ":p7", bText );
    query.bindValue( ":p8", bInterval );
    query.bindValue( ":p9", num );

    query.exec();

    //если порт с этим номером уже есть, то старый удаляется
    if ( beacon[ num ] != NULL) {

        beacon[ num ]->disconnect();
        delete beacon[ num ];
        beacon[ num ] = NULL;

    }

    beacon[ num ] = new QAPRSBeacon();
    beacon[ num ]->setBeaconNum( num );
    beacon[ num ]->setAPRSCall( APRSCall );
    beacon[ num ]->setParam( "PORT_NUM", QString::number( port_num ) );
    beacon[ num ]->setParam( "SYM", bSym );
    beacon[ num ]->setParam( "CALL", bCall );
    beacon[ num ]->setParam( "LAT", bLat );
    beacon[ num ]->setParam( "LNG", bLng );
    beacon[ num ]->setParam( "UNPROTO", bUnproto );
    beacon[ num ]->setParam( "TEXT", bText );
    beacon[ num ]->setParam( "INTERVAL", bInterval );

    connect ( beacon[ num ], SIGNAL( changeState( int, int ) ),
              this,          SLOT( beaconChangeState( int, int ) ) );

    connect ( beacon[ num ], SIGNAL( sendBeacon( int,  int, QString, QString, QString ) ),
              this,          SLOT( beaconSendBeacon( int,  int, QString, QString, QString ) ) );



    return 0;

}

//задать параметр маяка
int QAPRSCore::setBeaconParam( int bNum, QString pVar, QString pVal ) {

    QByteArray tmp;
    QSqlQuery query;
    QTextCodec* decoder = QTextCodec::codecForName("UTF-8");

    if ( beacon[ bNum ] == NULL ) {
        qDebug() << "create beacon bnum=" << bNum;

        beacon[ bNum ] = new QAPRSBeacon();
        beacon[ bNum ]->setBeaconNum( bNum );
        beacon[ bNum ]->setAPRSCall( APRSCall );

        connect ( beacon[ bNum ], SIGNAL( changeState( int, int ) ),
                  this,           SLOT( beaconChangeState( int, int ) ) );

        connect ( beacon[ bNum ], SIGNAL( sendBeacon( int,  int, QString, QString, QString ) ),
                  this,           SLOT( beaconSendBeacon( int,  int, QString, QString, QString ) ) );

        tmp.clear();
        tmp.append( "EVENT CHANGESTATE BCN_NUM=" + QString::number( bNum ) + "; STATUS=OFF;\r\n" );
        tcpGUIServerConnection->write( tmp );

    }

    //если порт с этим номером уже есть, то задаем параметр
    if ( beacon[ bNum ] != NULL) {

        //qDebug() << "pNum=" << bNum << "; pVar=" << pVar << "; pVal=" << pVal;
        beacon[ bNum ]->setParam( pVar, decoder->fromUnicode( pVal ) );

        if ( pVar == "CALL" )     query.prepare( " update beacons set call=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "LAT" )      query.prepare( " update beacons set lat=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "LNG" )      query.prepare( " update beacons set lng=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "INTERVAL" ) query.prepare( " update beacons set binterval=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "UNPROTO" )  query.prepare( " update beacons set bunproto=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "TEXT" )     query.prepare( " update beacons set btext=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "SYM" )      query.prepare( " update beacons set sym=(:p1) "
                                                 " where  beacon_num=(:p2) " );

        if ( pVar == "PORT_NUM" ) query.prepare( " update beacons set port_num=(:p1) "
                                                 " where  beacon_num=(:p2) " );



        query.bindValue( ":p1", pVal );
        query.bindValue( ":p2", bNum );
        query.exec();

        tmp.clear();
        tmp.append( "SYSBEACON BCN_NUM=" + QString::number( bNum ) + " SET " + pVar + "=" + decoder->fromUnicode( pVal ) + "\r\n" );
        tcpGUIServerConnection->write( tmp );


        return 0;

    } else return -1;

}

//создание всех маяков
void QAPRSCore::createAllBeacons( ) {

    QSqlQuery query;

    query.prepare( " select beacon_num, port_num, sym, call, lat, lng, bunproto, btext, binterval "
                   " from beacons "
                   " where not(port_num is null) "
                   " order by beacon_num" );

    query.exec();
    while (query.next()) {
        createBeacon( query.value(0).toInt(), query.value(1).toInt(), query.value(2).toString(),
                      query.value(3).toString(), query.value(4).toString(), query.value(5).toString(),
                      query.value(6).toString(), query.value(7).toString(), query.value(8).toString() );
    }

}

//открытие всех маяков
void QAPRSCore::openAllBeacons( ) {

    int i;

    for(i=0;i<256;i++) {

        openBeacon( i );

    }

}

//закрытие всех маяков
void QAPRSCore::closeAllBeacons( ) {

    int i;

    for(i=0;i<256;i++) {

        closeBeacon( i );

    }

}

//открытие маяка
int QAPRSCore::openBeacon( int bNum ) {


    if ( beacon[ bNum ] != NULL) {

        beacon[ bNum ]->openBeacon();

        return 0;

    } else return -1;

}

//закрытие маяка
int QAPRSCore::closeBeacon( int bNum ) {

    if ( beacon[ bNum ] != NULL) {

        beacon[ bNum ]->closeBeacon();

        return 0;

    } else return -1;

}

//удаление порта
int QAPRSCore::deleteBeacon( int bNum ) {

    //если порт существует, то удаляем его
    if ( beacon[ bNum ] != NULL) {

        //в системную базу тоже сохраним
        QSqlQuery query;
        query.prepare( "update beacons set port_num=NULL, sym=(:p2), call=(:p3), lat=(:p4), lng=(:p5), bunproto=(:p6), btext=(:p7), binterval=(NULL) "
                       "where beacon_num=(:p7);" );
        query.bindValue( ":p2", "" );
        query.bindValue( ":p3", "" );
        query.bindValue( ":p4", "" );
        query.bindValue( ":p5", "" );
        query.bindValue( ":p6", "" );
        query.bindValue( ":p7", bNum );
        query.exec();

        beacon[ bNum ]->closeBeacon();
        beacon[ bNum ]->disconnect();
        delete beacon[ bNum ];
        beacon[ bNum ] = NULL;

        QByteArray tmp;

        tmp.clear();
        tmp.append( "EVENT CHANGESTATE BCN_NUM=" +  QString::number( bNum ) + "; STATUS=DELETED;\r\n" );
        tcpGUIServerConnection->write( tmp );

        return 0;

    } else return -1;

}

//Посылка маяка
void QAPRSCore::beaconSendBeacon( int bNum,  int pNum, QString To, QString From, QString MsgText ) {


    qDebug() << "QAPRSCore::SYSBEACON send bNum = " << bNum << "; pNum = " << pNum << "; To = " << To << "; From = " << From << "; MsgText = " << MsgText;

    if ( port[ pNum ]!=NULL ) {

        port[ pNum ]->sendAPRSPacket( To, From, MsgText );

    }

}

//////////////////
QString QAPRSCore::QStringToHEX( QString Text ){

    QByteArray tmp;

    tmp.append( Text );

    return (QString)(tmp.toHex());

}
