#ifndef __beaconoption_H__
#define __beaconoption_H__

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

                int beaconNumber; //����� �����

		Q_OBJECT
signals:
                void setBeaconParam( int bNum, QString pName, QString pVal  ); //������ ������� �������������� ���������
public slots:
                void RCVBeaconParam( int bNum, QString pName, QString pVal ); //������ �������� �����

private slots:
                void saveButtonClick(); //��������� ���������
                void closeButtonClick();//������� �� ��������



};



#endif // __beaconoption_H__
