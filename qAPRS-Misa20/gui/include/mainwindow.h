#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "../../core/include/version.h"
#include "../../core/include/qatomix.h"
#include "../../core/include/qaprscore.h"
#include "../../core/include/qsysstationlist.h"
#include "./include/custommodels.h"
#include "./include/packetswindow.h"
#include "./include/messageswindow.h"
#include "./include/stationswindow.h"
#include "./include/mapswindow.h"
#include "./include/messengerwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QAtomix   *atomGUI;
    QAPRSCore *Core; //����

    //������ �����������
    PortsModel   *pModel;
    BeaconsModel *bModel;
    QStandardItemModel myCallsModel;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *map;

    QTimer *timer; //������ (����� �������� ������� ������)
    char lamps[256];

    PacketsWindowImpl    PacketsWindow; //���� � ��������
    MessagesWindowImpl   MessagesWindow; //���� � �����������
    StationsWindowImpl   StationsWindow; //���� �� ���������
    MessengerWindowImpl  MessengerWindow; //���� ���������

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent( QShowEvent *event );
    void hideEvent (QHideEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayIcon();

    QAction             *showGenOptionAction;
    QAction             *showPacketsAction;
    QAction             *showMessagesAction;
    QAction             *showStationsAction;
    QAction             *showMapsAction;
    QAction             *showMessengerAction;
    QAction             *quitAction;

    //������ ������
    QAction             *openPortAction;
    QAction             *closePortAction;
    QAction             *createPortAction;
    QAction             *editPortAction;
    QAction             *deletePortAction;
    QAction             *openPortsAction;
    QAction             *closePortsAction;

    //������ ������
    QAction             *createBeaconAction;
    QAction             *editBeaconAction;
    QAction             *deleteBeaconAction;
    QAction             *openBeaconAction;
    QAction             *closeBeaconAction;
    QAction             *sendBeaconAction;
    QAction             *sendStatusAction;
    QAction             *openBeaconsAction;
    QAction             *closeBeaconsAction;

    QSystemTrayIcon     *trayIcon;
    QMenu               *trayIconMenu;

public slots:
    void setPortParamFFrm( int pNum, QString pName, QString pVal  ); //���� ����������� ��������� �� ���� �������������� ���������� ������
    void setBeaconParamFFrm( int bNum, QString pName, QString pVal  ); //���� ����������� ��������� �� ���� �������������� ���������� ������

    void updatePortFFrm( int pNum );//���� ����������� ������ � ������������� �������� ��������� ������������� �����
    void updateBeaconFFrm( int bNum );//���� ����������� ������ � ������������� �������� ��������� ������������� �����

    void storeProgress( QString msg );
    void allLoaded( );

    void closeApp();       //�������� ���������� �����

private slots:

    //������ �� ������ ��������
    void saveButtonClick();   //���������� ����������
    void cancelButtonClick(); //�������� ��������� ����������
    void closeButtonClick();  //������� ���������� �����

    void systrayclick ( ); //���� �� ������ � ����

    //�� �������� ports
    void openPortsButtonClick(); //������� ��� �����
    void closePortsButtonClick();//������� ��� �����
    void openPortButtonClick();  //������� ������������ ����
    void closePortButtonClick(); //������� ������������ ����
    void deletePortButtonClick();//������� ������������ ����
    void editPortButtonClick();  //������������� ��������� ������������� �����
    void createPortButtonClick();//�������� ����
    void onPortDoubleClicked( const QModelIndex & index ); //��� ������� ������ �� ����� - ��������� ��� ���������
    void portsCurrentChanged( const QModelIndex & current, const QModelIndex & previous );
    void updPortsButtonState ( );
    void onTimer(); //����� ��������� ��������

    //�� �������� beacons
    void createBeaconButtonClick(); //������� ����
    void editBeaconButtonClick();   //������������� ��������� ������������� �����
    void deleteBeaconButtonClick(); //������� ������������ ����
    void openBeaconButtonClick();  //������� ������������ ����
    void closeBeaconButtonClick(); //������� ������������ ����
    void sendBeaconButtonClick();//������� ����
    void sendStatusButtonClick();//������� ������
    void openBeaconsButtonClick(); //������� ��� �����
    void closeBeaconsButtonClick();//������� ��� �����


    void beaconsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous );
    void updBeaconsButtonState();


    //�� �������� Query
    void qrySaveButtonClick();   //���������� ����������
    void qryCancelButtonClick(); //�������� ��������� ����������
    void qryCloseButtonClick();
    void qryAddCallButtonClick();//�������� �������� � ������
    void qryDeleteCallButton();  //������� �������� �� ������

    void showPackets();    //���������� ���� �������
    void showMessages();   //���������� ���� ���������
    void showStations();   //���������� ���� �������
    void showMaps();       //���������� ���� ����
    void showMessenger();  //���������� ���� ���������

    void weSysvarSet( QString VarName, QString VarVal );
    void weSysportSetParam( int PortNum, QString ParName, QString ParVal );
    void weSysportChangeState( int PortNum, QString PortState );

    void weSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );
    void weSysbeaconChangeState( int BcknNum, QString BcknState );

    void weportRXPacket( int pNum, bool isRX );
    void weportTXPacket( int pNum, bool isTX );

};

#endif // MAINWINDOW_H
