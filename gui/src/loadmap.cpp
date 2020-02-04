#include "include/loadmap.h"

LoadMapDialog::LoadMapDialog()
{

    setupUi(this);

    show();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDir *directory;
    directory = new QDir("maps");

    QStringList filter1;
    QStringList filter2;
    filter1 << "*.inf";
    filter2 << "*.bmp" << "*.jpg" << "*.gif";

    QFileInfoList inflist = directory->entryInfoList( filter1 );
    QFileInfoList imglist = directory->entryInfoList( filter2 );

	qDebug() << "inflist.size()=" << inflist.size();
	qDebug() << "imglist.size()=" << imglist.size();

    mapsModel.setColumnCount( 5 );

    QStringList labels;
    labels << tr("Map description") << tr("File") << tr("Img file") << tr("Top Left") << tr("Bottom Right");

    mapsModel.setHorizontalHeaderLabels( labels );

    mapsView->setModel( &mapsModel );

    mapsView->setColumnWidth( 0, 120 );
    mapsView->setColumnWidth( 1, 80 );
    mapsView->setColumnWidth( 2, 80 );

    int rowCount = 0;

    for (int i = 0; i < inflist.size(); i++) {

        QFileInfo infFileInfo = inflist.at( i );

        QString mapName = infFileInfo.fileName().left( infFileInfo.fileName().indexOf( "." ) );

        for (int j = 0; j < imglist.size(); j++) {

            QFileInfo picFileInfo = imglist.at( j );

            if ( mapName.toUpper() == picFileInfo.fileName().left( picFileInfo.fileName().indexOf( "." ) ).toUpper() ) {
                rowCount = rowCount + 1;
            }

        }

    }

    mapsModel.setRowCount( rowCount );

    progressBar->setMaximum( rowCount );

    int rowNum = 0;

    for (int i = 0; i < inflist.size(); i++) {

        QFileInfo infFileInfo = inflist.at( i );

        QString mapName = infFileInfo.fileName().left( infFileInfo.fileName().indexOf( "." ) );

        for (int j = 0; j < imglist.size(); j++) {

            QFileInfo picFileInfo = imglist.at( j );

            if ( mapName.toUpper() == picFileInfo.fileName().left( picFileInfo.fileName().indexOf( "." ) ).toUpper() ) {

                //qDebug() << infFileInfo.path() + "/" + infFileInfo.fileName() + "==" + picFileInfo.path() + "/" + picFileInfo.fileName();

                mapsModel.setData( mapsModel.index( rowNum, 1 ), infFileInfo.fileName() );
                mapsModel.setData( mapsModel.index( rowNum, 2 ), picFileInfo.fileName() );

                progressBar->setValue( rowNum );
                //tableWidget->setRowCount( tableWidget->rowCount() + 1 );

                //tableWidget->setItem(i2, 1, newItem1 );
                //QTableWidgetItem *infFileName = new QTableWidgetItem( infFileInfo.fileName() );
                //QTableWidgetItem *picFileName = new QTableWidgetItem( picFileInfo.fileName() );

                //tableWidget->setItem( tableWidget->rowCount() - 1, 1, infFileName );
                //tableWidget->setItem( tableWidget->rowCount() - 1, 2, picFileName );

                QFile file( infFileInfo.path() +"/"+ infFileInfo.fileName() );

                file.open( QIODevice::ReadOnly );
                QTextStream in( &file );

                QString topLeft     = in.readLine();
                QString bottomRight = in.readLine();
                QString mapName     = in.readLine();

                mapsModel.setData( mapsModel.index( rowNum, 0 ), mapName );
                mapsModel.setData( mapsModel.index( rowNum, 3 ), topLeft.split(',').at(0) + "\n" + (QString)(topLeft.split(',').at(1)).trimmed() );
                mapsModel.setData( mapsModel.index( rowNum, 4 ), bottomRight.split(',').at(0) + "\n" + (QString)(bottomRight.split(',').at(1)).trimmed() );

                file.close();

                qApp->processEvents ();

                rowNum = rowNum + 1;

            }

        }

    }

//    ui->treeView->setCurrentIndex( model2->index( 0, 0, QModelIndex() ) );
    mapsView->setCurrentIndex( mapsModel.index( 0, 0, QModelIndex() ) );

    progressBar->setVisible( false );

    connect( loadButton,    SIGNAL( clicked(  ) ),
	     this,          SLOT( loadClick( ) ) );

    connect( previewButton, SIGNAL( clicked(  ) ),
             this,          SLOT( viewClick( ) ) );
    
    connect( mapsView,      SIGNAL( doubleClicked(QModelIndex) ),
             this,          SLOT( loadClick() ) );    

    previewButton->setEnabled( true );

    QApplication::restoreOverrideCursor();

};


LoadMapDialog::~LoadMapDialog()
{

}

void LoadMapDialog::loadClick( ) {

    qDebug() << "Load Click";

    emit selectMap( mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 1, QModelIndex() ) , Qt::DisplayRole ).toString(),
                    mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 2, QModelIndex() ) , Qt::DisplayRole ).toString() );

    close();

/*
	int ind;
	QString mapName;

	ind = tableWidget->currentIndex().row();

	if (tableWidget->item(tableWidget->currentIndex().row(), 2)->text()!="") {
                if (ind==-1) mapName = "./maps/world.jpg" ;
                        else mapName = "./maps/"+tableWidget->item(tableWidget->currentIndex().row(), 2)->text() ;

                scene->loadMap( mapName );
		scene->drawMap();

                QSqlQuery query;
                query.exec("update vars set varval=\"" + mapName + "\" "
                          "where varname=\"map\" ;" );

		close();

	}
*/

}

void LoadMapDialog::viewClick( ) {

    PreviewMapDialog win;

    QString TL = mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 3, QModelIndex() ) , Qt::DisplayRole ).toString();
    QString BR = mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 4, QModelIndex() ) , Qt::DisplayRole ).toString();

    win.label_4->setText( mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 0, QModelIndex() ) , Qt::DisplayRole ).toString() );
    win.label_5->setText( TL.split( '\n' ).at(0) + ", " + TL.split( '\n' ).at(1) );
    win.label_6->setText( BR.split( '\n' ).at(0) + ", " + BR.split( '\n' ).at(1) );

    win.setWindowTitle( tr("Preview map") + " - " + mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 0, QModelIndex() ) , Qt::DisplayRole ).toString() );

    QGraphicsScene scene;

    win.graphicsView->setScene( &scene );

    QGraphicsPixmapItem img;

    img.setPixmap( QPixmap( "./maps/" + mapsModel.data( mapsModel.index( mapsView->currentIndex().row(), 2, QModelIndex() ) , Qt::DisplayRole ).toString() ) );

    img.setTransformationMode ( Qt::SmoothTransformation );

    scene.addItem( &img );

    //win.graphicsView->viewport()->width()

    if ( ((double)win.graphicsView->viewport()->width()/img.pixmap().width()) <
         ((double)win.graphicsView->viewport()->height()/img.pixmap().height()) ) {

//        img.scale( (((double)win.graphicsView->viewport()->width())/img.pixmap().width()), (((double)win.graphicsView->viewport()->width())/img.pixmap().width()) );

    } else {

//        img.scale( (((double)win.graphicsView->viewport()->height())/img.pixmap().height()), (((double)win.graphicsView->viewport()->height())/img.pixmap().height()) );

    }

    scene.setSceneRect( 0, 0,
                        img.mapToParent( img.pixmap().width(), img.pixmap().height() ).x(),
                        img.mapToParent( img.pixmap().width(), img.pixmap().height() ).y()
                        );

    connect( win.loadButton, SIGNAL( clicked(  ) ),
                                      this, SLOT( loadClick( ) ) );

    win.exec();

    qDebug() << "Dialog closed";

}


