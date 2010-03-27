#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QCheckBox>
#include <QTcpSocket>
#include <QDate>
#include <QTime>
#include <QTextCodec>
#include "../../core/include/qaprscore.h"
#include "./include/custommodels.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString CoreAddress;//����� ����
    QString CorePort;   //���� ����

    //������ �����������
    //QStandardItemModel *model;
    PortsModel *pModel;
    //QStandardItemModel *bModel;
    BeaconsModel *bModel;

    //������� ���������� ������ � ����������� �� ��������� ������ � ������
    //(�������� ��������� ������ ��������� �����, ���� �� ��� �������)
    void updPortsButtonState ( );
    void updBeaconsButtonState ( );

    //�������, � ������� �������� ����������� APRS ��������
    QVariantMap *symNames; //�� ���������
    QVariantMap *symImgs;  //�� ������������

protected:
    void changeEvent(QEvent *e);

private:
    void setDBValue( QString varname, QString varval ); //���������� ��������� � ��
    Ui::MainWindow *ui;
    QAPRSCore Core; //����
    QTcpSocket *tcpGUIClient; //����� ����� � �����
    QByteArray allmsg; //������ ���������� �� ���� ������
    QTimer     *timer; //������ (����� �������� ������� ������)

signals:
    void RCVPortParam( int pNum, QString pName, QString pVal ); //������ �������� �����
    void RCVBeaconParam( int bNum, QString pName, QString pVal ); //������ �������� �����

public slots:
    void setPortParamFFrm( int pNum, QString pName, QString pVal  ); //���� ����������� ��������� �� ���� �������������� ���������� ������
    void setBeaconParamFFrm( int bNum, QString pName, QString pVal  ); //���� ����������� ��������� �� ���� �������������� ���������� ������

private slots:
    //������ �� ������ ��������
    void saveButtonClick();   //���������� ����������
    void cancelButtonClick(); //�������� ��������� ����������
    void closeButtonClick();  //�������� ���������� �����
    //�� ������
    void openPortsButtonClick(); //������� ��� �����
    void closePortsButtonClick();//������� ��� �����
    void openPortButtonClick();  //������� ������������ ����
    void closePortButtonClick(); //������� ������������ ����
    void deletePortButtonClick();//������� ������������ ����
    void editPortButtonClick();  //������������� ��������� ������������� �����
    void createPortButtonClick();//�������� ����
    void onPortDoubleClicked( const QModelIndex & index ); //��� ������� ������ �� ����� - ��������� ��� ���������
    //�� ������
    void editBeaconButtonClick();   //������������� ��������� ������������� �����
    void onBeaconButtonClick();     //�������� ������������ ����
    void offBeaconButtonClick();    //��������� ������������ ����
    void deleteBeaconButtonClick(); //������� ������������ ����
    void onAllBeaconsButtonClick(); //�������� ��� �����
    void offPortsButtonClick();     //��������� ��� �����
    void createBeaconButtonClick(); //������� ����
    void onBeaconDoubleClicked( const QModelIndex & index );//��� ������� ������ �� ����� - ��������� ��� ���������

    void ToLog (QString Text); //����, � ������� �������� ��� ���� (�� �����!)
    void stopCore(); //���������� ������� STOP �� ����

    void tcpGUIClientconnected () ;
    void tcpGUIClientdisconnected ();
    void tcpGUIClienterror (QAbstractSocket::SocketError socketError);
    void tcpGUIClientread();

    void onTimer(); //����� ��������� ��������
    //����������� ������� ����� ������� ������ � �������� ����������� ������, ������
    void portsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous );
    void beaconsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous );

};

#endif // MAINWINDOW_H
