#ifndef BEACONOPTION_H
#define BEACONOPTION_H

#include "ui_beaconoption.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QDebug>


class BeaconoptionDialog : public QMainWindow, public Ui::DialogBeaconoption
{

public:
		QSqlQueryModel plainModel;
		QString genQuery;

                BeaconoptionDialog();
                ~BeaconoptionDialog();

                void closeEvent(QCloseEvent *event);
                void hideEvent ( QHideEvent * event );

                int beaconNumber; //номер маяка

		Q_OBJECT
signals:
                void setBeaconParam( int bNum, QString pName, QString pVal  ); //сингал посылки установленного параметра
                void updateBeacon( int bNum ); //применить параметры маяка
public slots:
                void RCVBeaconParam( int bNum, QString pName, QString pVal ); //принят параметр порта

private slots:
                void saveButtonClick(); //сохранить параметры
                void closeButtonClick();//закрыть не сохраняя
                void mulButtonClick(); //умножить введенное время на 60
                void mul2ButtonClick(); //умножить введенное время на 60



};



#endif // BEACONOPTION_H
