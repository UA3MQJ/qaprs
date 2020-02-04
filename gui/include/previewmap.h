#ifndef PREVIEWMAP_H
#define PREVIEWMAP_H

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



#endif // PREVIEWMAP_H
