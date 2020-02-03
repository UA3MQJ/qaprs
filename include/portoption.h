#ifndef PORTOPTION_H
#define PORTOPTION_H

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
private slots:
                void saveButtonClick();
                void closeButtonClick();
                void KISSEasySetup(int index);

};



#endif // PORTOPTION_H
