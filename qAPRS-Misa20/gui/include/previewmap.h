#ifndef __PREVIEWMAP_H__
#define __PREVIEWMAP_H__

#include "ui_previewmap.h"
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QFileInfo>
#include <QTextStream>
#include <QStandardItemModel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <qdebug.h>


class PreviewMapDialog : public QDialog, public Ui::DialogPreviewMap
{

    Q_OBJECT

public:
//		QSqlQueryModel plainModel;

                PreviewMapDialog();
                ~PreviewMapDialog();

private slots:
		void loadClick( );

};



#endif // __PREVIEWMAP_H__
