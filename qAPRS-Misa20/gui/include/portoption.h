#ifndef __portoption_H__
#define __portoption_H__

#include "ui_portoption.h"
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

                int portNumber;   //����� �����
                QString portType; //��� �����
                QString portTypeID; //��� �����

		Q_OBJECT
signals:
                void setPortParam( int pNum, QString pName, QString pVal  ); //������ ������� �������������� ���������
                void updatePort( int pNum ); //��������� ��������� �����

public slots:
                void RCVPortParam( int pNum, QString pName, QString pVal ); //������ �������� �����

private slots:
                void saveButtonClick(); //���������
                void closeButtonClick();//������� �� ��������
                void KISSEasySetup(int index);//���������� ��������� ���������� ��� KISS
                void onProxyCheck(int index = 0);//���������� ��������� ������


};



#endif // __portoption_H__
