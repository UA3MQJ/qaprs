#include "include/storageprogress.h"

StorageProgressDialog::StorageProgressDialog()
{

    //splash = NULL;
    //this->setVisible( false );

    setupUi( this );

    this->pos().setX( 0 );
    this->pos().setY( 0 );

}

StorageProgressDialog::~StorageProgressDialog()
{

}

void StorageProgressDialog::storeProgress( QString msg ) {

    if ( ( msg == "ALL LOADED" ) ){

        this->close();

        //if ( splash != NULL ) {
//
 //           splash->finish( this );

    //    }

    } else {
        label->setText( msg );
        //qDebug() << "msg =" << msg;
        this->repaint();
        label->repaint();
/*
        if ( splash != NULL ) {

            //this->setVisible( false );

            Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

            splash->showMessage( msg, topRight, Qt::white );
            splash->setFocus();

            this->pos().setX( 0 );
            this->pos().setY( 0 );


        }*/

    }

}
