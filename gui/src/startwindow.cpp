#include "include/startwindow.h"
#include <QStatusBar>
#include <QtGui>
#include <QFile>
#include <QMessageBox>


//
StartWindowImpl::StartWindowImpl(QWidget * parent)
        : QMainWindow(parent)
{
    QApplication::setStyle("plastique"); //установить пластиковый стиль
    QApplication::setPalette(QApplication::style()->standardPalette()); //со стандартной палитрой
    this->setWindowIcon(QIcon(":/images/logo.png") );

    MainWin = nullptr;

    setupUi(this);
    statusBar()->clearMessage();

    this->progressBar->setVisible(false);

    cb1clicked(true);

    connect(inprCB, SIGNAL(clicked(bool)),
            this,   SLOT(cb1clicked(bool)));

    connect(outprCB, SIGNAL(clicked(bool)),
            this,    SLOT(cb2clicked(bool)));

    connect(goButton, SIGNAL(clicked()),
            this,     SLOT(goClick()));

    //читаем файл параметров запуска
    QFile file("start.ini");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //если файл не открыт
    } else {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();

            if ((line.split('=').at(0) == "autostart")&&(line.split('=').at(1) == "GUI")) {
                outprCB->setChecked( true );
            }
        }
    }

    myTimeout = 10;
    updateGoButton();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1000);

//    pubsub = new QPubsub();
//    pubsub->setName("GUI");
    QPubsub *p1;
    QPubsub *p2;
    p1 = new QPubsub();
    p2 = new QPubsub();
    p1->setName("Server");
    p2->setName("Client");
    p1->beServer(8899);
    p2->beClient("127.0.0.1", 8899);
}

StartWindowImpl::~StartWindowImpl() {
    //if ( Core != nullptr ) {
    //    this->Core->atomCORE->wait();
    //}
}


void StartWindowImpl::showEvent (QShowEvent *event) {
    Q_UNUSED(event)
}

void StartWindowImpl::closeEvent(QCloseEvent *event) {
    if (MainWin != nullptr) {
        hide();
        event->ignore();
    }
}

void StartWindowImpl::cb1clicked(bool checked) {
    qDebug() << "cb1" << checked;
    myTimeout = 10;
    updateGoButton();
    this->hostEdit->setEnabled(false);
    this->textEdit->setText(tr("Start Graphic User Interface and create APRS Core in program. "
                                "Select TCP/IP port for connect GUI<->CORE (6000 recomended)."));
}

void StartWindowImpl::cb2clicked(bool checked) {
    qDebug() << "cb2" << checked;
    myTimeout = 10;
    updateGoButton();
    this->hostEdit->setEnabled(true);
    this->textEdit->setText(tr( "Start Graphic User Interface and connect to already started APRS on this or on the other host. "
                                "Select TCP/IP port and HostName for connect GUI<->CORE (6000 recomended). "
                                "If APRS Core started on this computer in console - use 'localhost'" ) );
}

void StartWindowImpl::goClick() {
    qDebug() << "GO!";

    timer->stop();
    timer->disconnect();

    QFile file("start.ini");

    //если не открыл, то ничего страшного
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "File open error";
    }

    if (this->inprCB->isChecked()) {
        //запуск core внутри gui
        file.write( "autostart=GUI+CORE" );

        this->goButton->setEnabled(false);
        this->goButton->repaint();
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QAPRSCore  *Core;

        Core = new QAPRSCore();
        Core->setWorkPort(portBox->text().toInt());
        Core->Start();

        MainWin = new MainWindow();
        MainWin->Core = Core;
        MainWin->atomGUI->beClient("127.0.0.1", portBox->text().toInt());


        connect(MainWin->atomGUI,  SIGNAL(SIGallLoaded()),
                this,              SLOT(allLoad()));

        connect(MainWin->atomGUI,  SIGNAL(storeProgress(QString)),
                this,              SLOT(readMsg(QString)));

        connect(MainWin->atomGUI , SIGNAL(SIGSocketError()),
                this,              SLOT(socketError()));
    }

    if ( this->outprCB->isChecked() ) {
        //соединение с core, запущенным снаружи
        file.write( "autostart=GUI" );

        this->goButton->setEnabled( false );
        this->goButton->repaint();
        QApplication::setOverrideCursor(Qt::WaitCursor);

        MainWin = new MainWindow();
        MainWin->atomGUI->beClient(hostEdit->text(), portBox->text().toInt());


        connect(MainWin->atomGUI, SIGNAL(SIGallLoaded()),
                this,             SLOT(allLoad()));

        connect(MainWin->atomGUI, SIGNAL(storeProgress(QString)),
                this,             SLOT(readMsg(QString)));

        connect(MainWin->atomGUI, SIGNAL(SIGSocketError()),
                this,             SLOT(socketError()));


    }

    //!!!
    allLoad();
}

void StartWindowImpl::allLoad() {
    qDebug() << "ALL LOADED!!!";
    QApplication::restoreOverrideCursor();
    MainWin->show();
    this->hide();
}

void StartWindowImpl::readMsg(QString msg) {
    qDebug() << "!!!!!" << msg;
    //this->statusBar()->showMessage(tr("Ready"));
}

void StartWindowImpl::socketError() {
    qDebug() << "CONNECTION ERROR";

    QApplication::restoreOverrideCursor();

    QMessageBox msgBox;
    msgBox.setText("Connection error!");
    msgBox.exec();

    MainWin->closeApp();
    this->close();
}

void StartWindowImpl::timeout() {
    qDebug() << myTimeout;

    updateGoButton();

    if ( myTimeout == 0 ) {
        timer->stop();
        timer->disconnect();
        goClick();
    }

    myTimeout = myTimeout - 1;
}

void StartWindowImpl::updateGoButton() {
    this->goButton->setText(tr("Go (%1)").arg(myTimeout));
}
