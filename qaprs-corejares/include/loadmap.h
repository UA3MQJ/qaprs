#ifndef __LOADMAP_H__
#define __LOADMAP_H__

#include "ui_loadmap.h"
#include "mainwindowimpl.h"
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QFileInfo>
#include <QTextStream>
#include <qdebug.h>


class LoadMapDialog : public QDialog, public Ui::DialogLoadMap
{


public:
//		QSqlQueryModel plainModel;

		LoadMapDialog();
		~LoadMapDialog();
		MvScene *scene;

		Q_OBJECT
private slots:
		void loadClick( );
		void viewClick( );

};



#endif // __LOADMAP_H__
