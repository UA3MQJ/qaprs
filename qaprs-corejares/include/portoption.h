#ifndef __portoption_H__
#define __portoption_H__

#include "ui_portoption.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QItemSelectionModel>

class PortoptionDialog : public QDialog, public Ui::DialogPortoption
{

public:
		QSqlQueryModel plainModel;
		QString genQuery;

		PortoptionDialog();
		~PortoptionDialog();
      	QSqlDatabase *db;

		Q_OBJECT
signals:
                void savePort( int pnum );

private slots:
                void saveButtonClick();
                void closeButtonClick();
                void KISSEasySetup(int index);

};



#endif // __portoption_H__
