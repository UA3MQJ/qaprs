#include "include/startwindow.h"
#include <QStatusBar>
#include <QtGui>
#include <QFile>


//
StartWindowImpl::StartWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{

    QApplication::setStyle( "plastique" ); //���������� ����������� �����
    QApplication::setPalette( QApplication::style()->standardPalette() ); //�� ����������� ��������
    this->setWindowIcon( QIcon(":/images/logo.png") );

    MainWin = NULL;

    setupUi(this);
    statusBar()->clearMessage();


    this->progressBar->setVisible( false );

    cb1clicked( true );

    connect( inprCB, SIGNAL( clicked ( bool ) ),
             this, SLOT( cb1clicked( bool ) ) );

    connect( outprCB, SIGNAL( clicked ( bool ) ),
             this, SLOT( cb2clicked( bool ) ) );

    connect( goButton ,  SIGNAL( clicked() ),
             this,       SLOT( goClick() ) );

    //������ ���� ���������� �������
    QFile file("start.ini");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //���� ���� �� ������
    } else {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();

            if ((line.split('=').at(0) == "autostart")&&(line.split('=').at(1) == "GUI")) {
                outprCB->setChecked( true );
            }
        }
    }

    myTimeout = 10;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1000);

    //goClick();

}

StartWindowImpl::~StartWindowImpl() {

    //if ( Core != NULL ) {

    //    this->Core->atomCORE->wait();

    //}

}


void StartWindowImpl::showEvent ( QShowEvent * event ) {


}

void StartWindowImpl::closeEvent(QCloseEvent *event) {

    if ( MainWin != NULL ) {
        hide();
        event->ignore();
    }



}

void StartWindowImpl::cb1clicked( bool checked ) {

    qDebug() << "cb1" << checked;

    this->hostEdit->setEnabled( false );

    //!��������� �������� �� ������������
    this->textEdit->setText( tr( "Start Graphic User Interface and create APRS Core in program. "
                                 "Select TCP/IP port for connect GUI<->CORE (6000 recomended).") );

}

void StartWindowImpl::cb2clicked( bool checked ) {

    qDebug() << "cb2" << checked;

    this->hostEdit->setEnabled( true );

    //!��������� �������� �� ������������
    this->textEdit->setText( tr( "Start Graphic User Interface and connect to already started APRS on this or on the other host. "
                                 "Select TCP/IP port and HostName for connect GUI<->CORE (6000 recomended). "
                                 "If APRS Core started on this computer in console - use 'localhost'" ) );

}

void StartWindowImpl::goClick() {

    qDebug() << "GO!";

    timer->stop();
    timer->disconnect();

    //MainWindow *w;

    QFile file("start.ini");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) ;//���� �� ������, �� ������ ���������

    if ( this->inprCB->isChecked() ) {
        //������ core ������ gui

        file.write( "autostart=GUI+CORE" );

        //this->progressBar->setVisible( true );
        this->goButton->setEnabled( false );
        this->goButton->repaint();
        QApplication::setOverrideCursor(Qt::WaitCursor);


        QAPRSCore  *Core;


        Core = new QAPRSCore::QAPRSCore();
        Core->setWorkPort( portBox->text().toInt() );
        Core->Start();

        MainWin = new MainWindow::MainWindow();

        MainWin->Core = Core;

        MainWin->atomGUI->beClient( "127.0.0.1", portBox->text().toInt() );


        connect( MainWin->atomGUI ,  SIGNAL( SIGallLoaded() ),
                 this,       SLOT( allLoad() ) );

        connect( MainWin->atomGUI ,  SIGNAL( storeProgress( QString ) ),
                 this,       SLOT( readMsg( QString ) ) );

        connect( MainWin->atomGUI ,  SIGNAL( SIGSocketError( ) ),
                 this,       SLOT( socketError( ) ) );

        //this->progressBar->setMinimum( 0 );
        //this->progressBar->setMaximum( 0 );
        //this->progressBar->setValue( 0 );

        //MainWin->show();

        //this->hide();

    }

    if ( this->outprCB->isChecked() ) {
        //���������� � core, ���������� �������
        file.write( "autostart=GUI" );

        //this->progressBar->setVisible( true );
        this->goButton->setEnabled( false );
        this->goButton->repaint();
        QApplication::setOverrideCursor(Qt::WaitCursor);


        MainWin = new MainWindow::MainWindow();


        MainWin->atomGUI->beClient( hostEdit->text(), portBox->text().toInt() );


        connect( MainWin->atomGUI ,  SIGNAL( SIGallLoaded() ),
                 this,       SLOT( allLoad() ) );

        connect( MainWin->atomGUI ,  SIGNAL( storeProgress( QString ) ),
                 this,       SLOT( readMsg( QString ) ) );

        connect( MainWin->atomGUI ,  SIGNAL( SIGSocketError( ) ),
                 this,       SLOT( socketError( ) ) );


    }

}

void StartWindowImpl::allLoad() {

    qDebug() << "ALL LOADED!!!";

    QApplication::restoreOverrideCursor();

    MainWin->show();

    this->hide();

}

void StartWindowImpl::readMsg( QString msg ) {

    //qDebug() << "!!!!!" << msg;
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

    this->goButton->setText( tr("Go (%1)").arg( myTimeout ) );

    if ( myTimeout == 0 ) {
        timer->stop();
        timer->disconnect();
        goClick();
    }

    myTimeout = myTimeout - 1;

}