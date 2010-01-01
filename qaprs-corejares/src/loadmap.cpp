#include "loadmap.h"

LoadMapDialog::LoadMapDialog()
{

    QStringList filter, filter2, labels;

    setupUi(this);

    show();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    repaint();

    QDir *directory, *dir2;

    directory = new QDir("maps");
    dir2 = new QDir("maps");

    filter << "*.inf" << "*.bmp" << "*.jpg" << "*.gif";

    QFileInfoList list = directory->entryInfoList( filter );

    tableWidget->setRowCount( 0 );
    tableWidget->setColumnCount(5);
    labels << "Map description" << "File" << "Img file" << "Top Left" << "Bottom Right";
    tableWidget->setHorizontalHeaderLabels( labels );
    tableWidget->setColumnWidth( 0, 120 );
    tableWidget->setColumnWidth( 1, 80 );
    tableWidget->setColumnWidth( 2, 80 );

    int i2=-1, j;
    QString  mapname, t1, t2;
    QTableWidgetItem *newItem5;

    for (int i = 0; i < list.size(); ++i) {

    	QFileInfo fileInfo = list.at(i);

    	//qWarning() << fileInfo.path() +"/"+ fileInfo.fileName();

    	if ( fileInfo.fileName().right(3).toUpper()=="INF" ) {

                if (fileInfo.fileName()!=mapname) {

                    i2++;
                    tableWidget->setRowCount( i2+1 );

                    QTableWidgetItem *newItem1 = new QTableWidgetItem( fileInfo.fileName() );
                    newItem1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
                    tableWidget->setItem(i2, 1, newItem1 );

                    QFile file( fileInfo.path() +"/"+ fileInfo.fileName() );

                    file.open( QIODevice::ReadOnly );
                    QTextStream in( &file );
                    QString str, str2;

                    str = in.readLine();
                    QTableWidgetItem *newItem2 = new QTableWidgetItem( str );
                    newItem2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
                    tableWidget->setItem(i2, 3, newItem2 );

                    str = in.readLine();
                    QTableWidgetItem *newItem3 = new QTableWidgetItem( str );
                    newItem3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
                    tableWidget->setItem(i2, 4, newItem3 );

                    str = in.readLine();
                    QTableWidgetItem *newItem4 = new QTableWidgetItem( str );
                    newItem4->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
                    tableWidget->setItem(i2, 0, newItem4 );

                    file.close();

                    for (j = 0; j < list.size(); ++j) {
    	    		QFileInfo fileInfo2 = list.at(j);
    	    		if (fileInfo.fileName().left( fileInfo.fileName().length()-3 ).toUpper() ==
                            fileInfo2.fileName().left( fileInfo.fileName().length()-3 ).toUpper() ) {

                            if ( (fileInfo2.fileName().right(3).toUpper()=="JPG") or
                                 (fileInfo2.fileName().right(3).toUpper()=="BMP") or
                                 (fileInfo2.fileName().right(3).toUpper()=="GIF") ) {

    	    				newItem5 = new QTableWidgetItem( fileInfo2.fileName());
    	    				newItem5->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    	    				tableWidget->setItem(i2, 2, newItem5 );
    	    			}
    	    		}
                    }
    		}
        }
    }

	QObject::connect( loadButton, SIGNAL( clicked(  ) ),
				      this, SLOT( loadClick( ) ) );

	QObject::connect( previewButton, SIGNAL( clicked(  ) ),
				      this, SLOT( viewClick( ) ) );

        QApplication::restoreOverrideCursor();

};


LoadMapDialog::~LoadMapDialog()
{

}

void LoadMapDialog::loadClick( ) {

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

}

void LoadMapDialog::viewClick( ) {
    close();
}


