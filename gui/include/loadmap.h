#ifndef LOADMAP_H
#define LOADMAP_H

#include "ui_loadmap.h"
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QFileInfo>
#include <QTextStream>
#include <QStandardItemModel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <qdebug.h>
#include "include/previewmap.h"


class LoadMapDialog : public QDialog, public Ui::DialogLoadMap
{

    Q_OBJECT

public:

		LoadMapDialog();
		~LoadMapDialog();
                QStandardItemModel mapsModel;

signals:
                void selectMap( QString mapInfFile, QString mapImgFile );

private slots:
		void loadClick( );
		void viewClick( );

};



#endif // LOADMAP_H
