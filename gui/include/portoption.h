#ifndef PORTOPTION_H
#define PORTOPTION_H

#include "ui_portoption.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QFile>
#include <QDomDocument>
#include <QRect>
#include <QDebug>


class PortoptionDialog : public QMainWindow, public Ui::DialogPortoption
{

public:
		QSqlQueryModel plainModel;
		QString genQuery;

		PortoptionDialog();
		~PortoptionDialog();

                void closeEvent(QCloseEvent *event);
                void hideEvent ( QHideEvent * event );

                int portNumber;   //номер порта
                QString portType; //тип порта
                QString portTypeID; //тип порта

		Q_OBJECT
signals:
                void setPortParam( int pNum, QString pName, QString pVal  ); //сингал посылки установленного параметра
                void updatePort( int pNum ); //применить параметры порта

public slots:
                void RCVPortParam( int pNum, QString pName, QString pVal ); //принят параметр порта

private slots:
                void saveButtonClick(); //сохранить
                void closeButtonClick();//закрыть не сохраняя
                void KISSEasySetup(int index);//обработчик установки параметров для KISS
                void onProxyCheck(int index = 0);//обработчик включения прокси


};



#endif // PORTOPTION_H
