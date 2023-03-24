#include "../../core/include/qpubsub.h"

QPubsub::QPubsub(QObject *parent) :
    QThread(parent)
{
    uid        = QRandomGenerator::global()->bounded(65536);
    atomID     = QString::number(uid);
    qDebug() << "atomID" << atomID;
    started = false;

    sysDecoder   = QTextCodec::codecForName("UTF-8");
    sysDecoder16 = QTextCodec::codecForName("UTF-16");
    inData.clear();
}

void QPubsub::setName(QString sName) {
    this->name = sName;
}

QString QPubsub::getName() {
    return this->name;
}

//реализация настройки связи
void QPubsub::beServer(quint16 port) {
    im = 0;

    tcpServer = new QTcpServer(this);
    tcpServerConnection = new QTcpSocket(this);

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(tcpServerNewConnection()));

    tcpServer->listen(QHostAddress::Any, port);

    tcpPort = port;
}

void QPubsub::beClient( QString address, int port ) {
    im = 1;

    tcpClient = new QTcpSocket();

    tcpClient->connectToHost(address, port);

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpClientconnected()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientread()));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this,      SLOT(SocketError(QAbstractSocket::SocketError)));
}


void QPubsub::tcpServerNewConnection () {
//подсоединение
    debug("tcpServerNewConnection");

    tcpServerConnection = tcpServer->nextPendingConnection();

    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(tcpServerRead()));
    connect(tcpServerConnection, SIGNAL(disconnected()), this, SLOT(tcpServerDisconnect()));

    tcpServer->close(); //!!!! поддерживаем только двойную связь
}

void QPubsub::tcpServerRead (){
//чтение сообщения
    debug("tcpServerRead");
    QByteArray datagram;
    datagram = tcpServerConnection->readAll();
    inData.append(datagram);

    //пока есть симолы перевода строки
    if (inData.indexOf("\n") > -1) {
        while (inData.indexOf("\n") > -1) {
            QString tStr;
            tStr = sysDecoder->toUnicode( inData.left( inData.indexOf("\n") - 1 ) ) ;

            debug("tcpServerRead Receive" + tStr);

            inData = inData.mid( inData.indexOf("\n") + 1 ); //отрезать
        }
    }
}

void QPubsub::tcpServerDisconnect () {
//отсоединение
    debug("tcpServerDisconnect");
    tcpServerConnection->close();
    tcpServerConnection->disconnect();
    connected = false;
    tcpServer->listen( QHostAddress::Any, tcpPort );
}

void QPubsub::tcpClientconnected() {
    debug("tcpClientconnected");
    connected = true;
}

void QPubsub::tcpClientread(){
    //чтение сообщения
    debug("tcpClientread");
    QByteArray datagram;
    datagram = tcpClient->readAll();
    inData.append( datagram );

    //пока есть симолы перевода строки
    if ( inData.indexOf("\n")>-1 ) {
        while ( inData.indexOf("\n")>-1 ) {
            //берем о посылаем на обработку
            QString tStr;
            tStr = sysDecoder->toUnicode( inData.left( inData.indexOf("\n") - 1 ) ) ;
            debug("tcpClientread Receive" + tStr);

            inData = inData.mid( inData.indexOf("\n") + 1 ); //отрезать
        }
    }
}

void QPubsub::SocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    //код скопировал из Fortune Client
    /*
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    getFortuneButton->setEnabled(true);
    */

    emit SIGSocketError();
}

//трид. использовать для какой-нибудь фоновой фигни
void QPubsub::run() {
    debug("RUN start");
    started = true;


    started = false;
    debug("RUN finish");
}

void QPubsub::debug(QString msg) {
    qDebug() << QDateTime::currentDateTime().toString( "dd.MM.yyyy hh:mm:ss.zzz" )
             << " - QPubsub(" << uid <<":"<< getName() << ")::" << msg;

};
