#include "include/portoption.h"

PortoptionDialog::PortoptionDialog()
{

    setupUi(this);

    show();

    connect( saveButton,  SIGNAL( clicked() ), this, SLOT( saveButtonClick() ) );
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeButtonClick() ) );
    connect( EasySetupComboBox, SIGNAL( currentIndexChanged( int ) ),
             this,              SLOT( KISSEasySetup( int ) ) );
    connect( proxyCheckBox,     SIGNAL( stateChanged( int ) ),
             this,              SLOT( onProxyCheck( int ) ) );
    connect( proxyAuthCheckBox, SIGNAL( stateChanged( int ) ),
             this,              SLOT( onProxyCheck( int ) ) );

    //загрузка списка серверов APRS
    QDomDocument doc("qAPRS_APRS_INTERNET_SERVER_LIST");
    QFile fileConfig("etc/aprs_servers.xml");
    if (!fileConfig.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&fileConfig)) {
        fileConfig.close();
        return;
    }
    fileConfig.close();

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if (e.tagName()=="SERVER1")  hostNameEdit ->setText( e.text() );
            if (e.tagName()=="SERVER2")  hostName2Edit->setText( e.text() );
            if (e.tagName()=="SERVER3")  hostName3Edit->setText( e.text() );
            if (e.tagName()=="filter")   APRSFilter   ->setText( e.text() );
        }
        n = n.nextSibling();
    }

    onProxyCheck( );
};


PortoptionDialog::~PortoptionDialog()
{

    this->disconnect();

}

void PortoptionDialog::saveButtonClick() {

    //qDebug() << "save";
    portType = "n\\a";
    portTypeID = "-1";

    switch ( portTypeBox->currentIndex() ) {
        case 1 :
            portType = "Inet";
            portTypeID = "0";
            break;
        case 2 :
            portType = "KISS";
            portTypeID = "1";
            break;
        case 3 :
            portType = "AGW";
            portTypeID = "2";
            break;
        case 6 :
            portType = "AXIP";
            portTypeID = "5";
            break;
        case 7 :
            portType = "FLDIGI";
            portTypeID = "6";
            break;
        case 8 :
            portType = "VASKA";
            portTypeID = "7";
            break;
    }

    if ((portTypeBox->currentIndex() == 0) ||
        (portTypeBox->currentIndex() == 4) ||
        (portTypeBox->currentIndex() == 5)) {

        QMessageBox::warning(this, tr("Error"),
                                       tr("Select supported port!"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok );

        return;

    }

    if ( portType == "AXIP" ) {

        emit setPortParam( portNumber, "PORT_TYPE", portTypeID );
        emit setPortParam( portNumber, "PORT_NAME", this->portNameEdit->text() );
        emit setPortParam( portNumber, "RXPort", this->AXIPRXPortBox->text() );
        emit setPortParam( portNumber, "TXPort", this->AXIPTXPortBox->text() );
        emit setPortParam( portNumber, "IPAddress", this->AXIPTXHostEdit->text() );

    }

    if ( portType == "Inet" ) {

        emit setPortParam( portNumber, "PORT_TYPE", portTypeID );
        emit setPortParam( portNumber, "PORT_NAME", this->portNameEdit->text() );

        emit setPortParam( portNumber, "Server1", this->hostNameEdit->text(  ) );
        emit setPortParam( portNumber, "Server2", this->hostName2Edit->text(  ) );
        emit setPortParam( portNumber, "Server3", this->hostName3Edit->text(  ) );
        emit setPortParam( portNumber, "User", this->userEdit->text(  ) );
        emit setPortParam( portNumber, "Pass", this->passEdit->text(  ) );
        emit setPortParam( portNumber, "Filter", this->APRSFilter->text(  ) );
        if ( this->proxyCheckBox->isChecked() ) {
            emit setPortParam( portNumber, "Proxy", "T" );
        } else {
            emit setPortParam( portNumber, "Proxy", "F" );
        }
        if ( this->proxyAuthCheckBox->isChecked(  ) ) {
            emit setPortParam( portNumber, "PAuth", "T" );
        } else {
            emit setPortParam( portNumber, "PAuth", "F" );
        }

        emit setPortParam( portNumber, "PName", this->proxyNameEdit->text(  ) );
        emit setPortParam( portNumber, "PPort", this->proxyPortBox->text() );
        emit setPortParam( portNumber, "PUser", this->proxyUserEdit->text(  ) );
        emit setPortParam( portNumber, "PPass", this->proxyPassEdit->text(  ) );

    }

    if ( portType == "AGW" ) {

        emit setPortParam( portNumber, "PORT_TYPE", portTypeID );
        emit setPortParam( portNumber, "PORT_NAME", this->portNameEdit->text() );
        emit setPortParam( portNumber, "Host", this->AGWHostEdit->text() );
        emit setPortParam( portNumber, "AGWPort", this->AGWPortBox->text() );

    }

    if ( portType == "KISS" ) {

        emit setPortParam( portNumber, "PORT_TYPE", portTypeID );
        emit setPortParam( portNumber, "PORT_NAME",     this->portNameEdit->text() );
        emit setPortParam( portNumber, "COM",           this->COMPortNumBox->currentText() );
        emit setPortParam( portNumber, "Speed",         this->COMPortSpeedBox->currentText() );
        emit setPortParam( portNumber, "Command1",      this->KISSComm1Edit->text() );
        emit setPortParam( portNumber, "Command2",      this->KISSComm2Edit->text() );
        emit setPortParam( portNumber, "Command3",      this->KISSComm3Edit->text() );
        emit setPortParam( portNumber, "Command4",      this->KISSComm4Edit->text() );
        emit setPortParam( portNumber, "TXDelay",       this->TXDelayEdit->text() );
        emit setPortParam( portNumber, "TXTail",        this->TXTailEdit->text() );
        emit setPortParam( portNumber, "SlotTime",      this->SlotTimeEdit->text() );
        emit setPortParam( portNumber, "Persist",       this->PersistEdit->text() );
        if ( this->FullDuplexCheckBox->isChecked(  ) ) {
            emit setPortParam( portNumber, "FullDuplex", "T" );
        } else {
            emit setPortParam( portNumber, "FullDuplex", "F" );
        }
        emit setPortParam( portNumber, "ExitFromKISS",  this->KISSCommEXITEdit->text() );

    }

    if ( portType == "FLDIGI" ) {

        emit setPortParam( portNumber, "PORT_TYPE", portTypeID );
        emit setPortParam( portNumber, "PORT_NAME", this->portNameEdit->text() );
        emit setPortParam( portNumber, "Host", this->FLDIGIHostEdit->text() );
        emit setPortParam( portNumber, "Port", this->FLDIGIPortBox->text() );

    }

    if ( portType == "VASKA" ) {

        emit setPortParam( portNumber, "PORT_TYPE", portTypeID );
        emit setPortParam( portNumber, "PORT_NAME", this->portNameEdit->text() );
        emit setPortParam( portNumber, "Host", this->VAskaHostEdit->text() );
        emit setPortParam( portNumber, "Port", this->VAskaPortBox->text() );
        emit setPortParam( portNumber, "Mode", QString::number( VAskaModeBox->currentIndex() ) );

    }

    //qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    //qDebug() << "PortoptionDialog portType=" << portType << " portTypeID= " << portTypeID;

    emit updatePort( portNumber );

    close();

}

void PortoptionDialog::closeButtonClick() {

    close();

}

//принят параметр порта
void PortoptionDialog::RCVPortParam( int pNum, QString pName, QString pVal ) {

    //qDebug() << "form << " << pNum << "pname=" << pName << " pVal=" << pVal;

    if ( portNumber == pNum ) {
//===============================================================
//определяем имя порта
        if ( pName == "PORT_NAME" ) {
            this->portNameEdit->setText( pVal );
            return;
        }
//===============================================================
//определяем тип порта

        if ( ( pName == "PORT_TYPE" ) && ( pVal == "5" ) ) {
            //portTypeID = "5";
            this->portTypeBox->setCurrentIndex( 6 );
            this->stackedWidget->setCurrentIndex( 6 );
            portType = "AXIP";
            return;
        }

        if ( ( pName == "PORT_TYPE" ) && ( pVal == "0" ) ) {
            //portTypeID = "0";
            this->portTypeBox->setCurrentIndex( 1 );
            this->stackedWidget->setCurrentIndex( 1 );
            portType = "Inet";
            return;
        }

        if ( ( pName == "PORT_TYPE" ) && ( pVal == "2" ) ) {
            //portTypeID = "2";
            this->portTypeBox->setCurrentIndex( 3 );
            this->stackedWidget->setCurrentIndex( 3 );
            portType = "AGW";
            return;
        }

        if ( ( pName == "PORT_TYPE" ) && ( pVal == "1" ) ) {
            //portTypeID = "1";
            this->portTypeBox->setCurrentIndex( 2 );
            this->stackedWidget->setCurrentIndex( 2 );
            portType = "KISS";
            return;
        }

        if ( ( pName == "PORT_TYPE" ) && ( pVal == "6" ) ) {
            //portTypeID = "6";
            this->portTypeBox->setCurrentIndex( 7 );
            this->stackedWidget->setCurrentIndex( 7 );
            portType = "FLDIGI";
            return;
        }

        if ( ( pName == "PORT_TYPE" ) && ( pVal == "7" ) ) {
            //portTypeID = "7";
            this->portTypeBox->setCurrentIndex( 8 );
            this->stackedWidget->setCurrentIndex( 8 );
            portType = "VASKA";
            return;
        }

//===============================================================
//заполняем элементы окна в зависимости от типа порта
        if ( portType == "AXIP" ) {

            if ( pName == "RXPort" ) this->AXIPRXPortBox->setValue( pVal.toInt() );
            if ( pName == "TXPort" ) this->AXIPTXPortBox->setValue( pVal.toInt() );
            if ( pName == "IPAddress" ) this->AXIPTXHostEdit->setText( pVal );

            return;
        }

        if ( portType == "Inet" ) {

            if ( pName == "Server1" ) this->hostNameEdit->setText( pVal );
            if ( pName == "Server2" ) this->hostName2Edit->setText( pVal );
            if ( pName == "Server3" ) this->hostName3Edit->setText( pVal );
            if ( pName == "User" ) this->userEdit->setText( pVal );
            if ( pName == "Pass" ) this->passEdit->setText( pVal );
            if ( pName == "Filter" ) this->APRSFilter->setText( pVal );
            if ( pName == "Proxy" ) this->proxyCheckBox->setChecked( pVal=="T" );
            if ( pName == "PAuth" ) this->proxyAuthCheckBox->setChecked( pVal=="T" );
            if ( pName == "PName" ) this->proxyNameEdit->setText( pVal );
            if ( pName == "PPort" ) this->proxyPortBox->setValue( pVal.toInt() );
            if ( pName == "PUser" ) this->proxyUserEdit->setText( pVal );
            if ( pName == "PPass" ) this->proxyPassEdit->setText( pVal );

            return;
        }

        if ( portType == "AGW" ) {

            if ( pName == "Host" )    this->AGWHostEdit->setText( pVal );
            if ( pName == "AGWPort" ) this->AGWPortBox->setValue( pVal.toInt() );

            return;
        }

        if ( portType == "KISS" ) {

            if ( pName == "COM" )               this->COMPortNumBox->setEditText( pVal );
            if ( pName == "Speed" )             this->COMPortSpeedBox->setCurrentIndex( this->COMPortSpeedBox->findText( pVal ) );
            if ( pName == "Command1" )          this->KISSComm1Edit->setText( pVal );
            if ( pName == "Command2" )          this->KISSComm2Edit->setText( pVal );
            if ( pName == "Command3" )          this->KISSComm3Edit->setText( pVal );
            if ( pName == "Command4" )          this->KISSComm4Edit->setText( pVal );
            if ( pName == "TXDelay" )           this->TXDelayEdit->setText( pVal );
            if ( pName == "TXTail" )            this->TXTailEdit->setText( pVal );
            if ( pName == "SlotTime" )          this->SlotTimeEdit->setText( pVal );
            if ( pName == "Persist" )           this->PersistEdit->setText( pVal );
            if ( pName == "FullDuplex" )        this->FullDuplexCheckBox->setChecked( pVal=="T" );
            if ( pName == "ExitFromKISS" )      this->KISSCommEXITEdit->setText( pVal );

            return;
        }

        if ( portType == "FLDIGI" ) {

            if ( pName == "Host" )    this->FLDIGIHostEdit->setText( pVal );
            if ( pName == "Port" )    this->FLDIGIPortBox->setValue( pVal.toInt() );

            return;
        }

        if ( portType == "VASKA" ) {

            if ( pName == "Host" )    this->VAskaHostEdit->setText( pVal );
            if ( pName == "Port" )    this->VAskaPortBox->setValue( pVal.toInt() );
            if ( pName == "Mode" )    this->VAskaModeBox->setCurrentIndex( pVal.toInt() );

            return;
        }

    }

}

void PortoptionDialog::KISSEasySetup(int index) {

    switch ( index ) {
        case 0 : //TNC2
                 KISSComm1Edit->setText("^M");
                 KISSComm2Edit->setText("KISS ON");
                 KISSComm3Edit->setText("RESTART");
                 KISSComm4Edit->setText("");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 1 : //PK
                 KISSComm1Edit->setText("*~");
                 KISSComm2Edit->setText("EXPERT ON");
                 KISSComm3Edit->setText("KISS ON");
                 KISSComm4Edit->setText("RESTART");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 2 : //D700
                 KISSComm1Edit->setText("^C^MAI 1^MTC 1^MTNC 2^MTC 0^M^M^M^M");
                 KISSComm2Edit->setText("MON OFF");
                 KISSComm3Edit->setText("KISS ON");
                 KISSComm4Edit->setText("RESTART");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 3 : //Kan
                 KISSComm1Edit->setText("^M");
                 KISSComm2Edit->setText("INT KISS");
                 KISSComm3Edit->setText("RESET");
                 KISSComm4Edit->setText("");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 4 : //TF
                 KISSComm1Edit->setText("^M");
                 KISSComm2Edit->setText("^[@K");
                 KISSComm3Edit->setText("");
                 KISSComm4Edit->setText("");
                 KISSCommEXITEdit->setText("192,255,192,13");
                 break;
    }

}


void PortoptionDialog::closeEvent(QCloseEvent *event) {

    //qDebug() << "closeEvent";

}

void PortoptionDialog::hideEvent ( QHideEvent * event ) {

    //qDebug() << "hideEvent";
    this->disconnect();
    this->deleteLater();

}

//обработчик включения прокси
void PortoptionDialog::onProxyCheck(int index) {

    if ( proxyCheckBox->isChecked() ) {
        proxyNameEdit->setEnabled( true );
        label_102->setEnabled( true );
        proxyPortBox->setEnabled( true );
        proxyAuthCheckBox->setEnabled( true );
        label_103->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
        proxyUserEdit->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
        label_104->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
        proxyPassEdit->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
    } else {
        proxyNameEdit->setEnabled( false );
        label_102->setEnabled( false );
        proxyPortBox->setEnabled( false );
        proxyAuthCheckBox->setEnabled( false );
        label_103->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
        proxyUserEdit->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
        label_104->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
        proxyPassEdit->setEnabled( proxyAuthCheckBox->isChecked() && proxyCheckBox->isChecked() );
    }

}

