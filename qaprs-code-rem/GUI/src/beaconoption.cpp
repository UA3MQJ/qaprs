#include "include/beaconoption.h"

BeaconoptionDialog::BeaconoptionDialog()
{

    setupUi(this);

    show();

    connect( saveButton,  SIGNAL( clicked() ), this, SLOT( saveButtonClick() ) );
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( closeButtonClick() ) );

};


BeaconoptionDialog::~BeaconoptionDialog()
{

    this->disconnect();

}

void BeaconoptionDialog::saveButtonClick() {

    //qDebug() << "save beaconNumber=" << beaconNumber;

    emit setBeaconParam( beaconNumber, "CALL", this->bckCallEdit->text() );
    emit setBeaconParam( beaconNumber, "LAT", this->bckLatEdit->text() );
    emit setBeaconParam( beaconNumber, "LNG", this->bckLngEdit->text() );
    emit setBeaconParam( beaconNumber, "INTERVAL", this->bckIntervalEdit->text() );
    emit setBeaconParam( beaconNumber, "UNPROTO", this->bckUnprotoEdit->text() );
    emit setBeaconParam( beaconNumber, "TEXT", this->bckTextEdit->text() );
    emit setBeaconParam( beaconNumber, "SYM", this->bckSymBox->itemData( bckSymBox->currentIndex() ).toString() );
    emit setBeaconParam( beaconNumber, "PORT_NUM", this->portNumBox->itemData( portNumBox->currentIndex() ).toString() );

    close();

}

void BeaconoptionDialog::closeButtonClick() {

    close();

}

//принят параметр порта
void BeaconoptionDialog::RCVBeaconParam( int bNum, QString pName, QString pVal ) {

    //qDebug() << "beaconNumber=" << beaconNumber << "bNum << " << bNum << "pname=" << pName << " pVal=" << pVal;

    if ( beaconNumber == bNum ) {

        if ( pName == "CALL" )      this->bckCallEdit->setText( pVal );
        if ( pName == "LAT" )       this->bckLatEdit->setText( pVal );
        if ( pName == "LNG" )       this->bckLngEdit->setText( pVal );
        if ( pName == "INTERVAL" )  this->bckIntervalEdit->setText( pVal );
        if ( pName == "UNPROTO" )   this->bckUnprotoEdit->setText( pVal );
        if ( pName == "TEXT" )      this->bckTextEdit->setText( pVal );
        if ( pName == "SYM" )       this->bckSymBox->setCurrentIndex( bckSymBox->findData( pVal ) );
        if ( pName == "PORT_NUM" )  this->portNumBox->setCurrentIndex( portNumBox->findData( pVal ) );

    }

}

void BeaconoptionDialog::closeEvent(QCloseEvent *event) {

    //qDebug() << "closeEvent";

}

void BeaconoptionDialog::hideEvent ( QHideEvent * event ) {

    //qDebug() << "hideEvent";
    this->disconnect();
    this->deleteLater();

}

