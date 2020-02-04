#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QSystemTrayIcon>

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
    QAPRSCore *Core; //ядро

    //модели отображения
    PortsModel   *pModel;
    BeaconsModel *bModel;
    QStandardItemModel myCallsModel;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *map;

    QTimer *timer; //таймер (чтобы лампочки красиво мигали)
    char lamps[256];

    PacketsWindowImpl    PacketsWindow; //окно с пакетами
    MessagesWindowImpl   MessagesWindow; //окно с сообщениями
    StationsWindowImpl   StationsWindow; //окно со станциями
    MessengerWindowImpl  MessengerWindow; //окно почтовика

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

    //экшены портов
    QAction             *openPortAction;
    QAction             *closePortAction;
    QAction             *createPortAction;
    QAction             *editPortAction;
    QAction             *deletePortAction;
    QAction             *openPortsAction;
    QAction             *closePortsAction;

    //экшены маяков
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
    void setPortParamFFrm( int pNum, QString pName, QString pVal  ); //сюда принимаются параметры от форм редактирования параметров портов
    void setBeaconParamFFrm( int bNum, QString pName, QString pVal  ); //сюда принимаются параметры от форм редактирования параметров маяков

    void updatePortFFrm( int pNum );//сюда принимается сигнал о необходимости обновить параметры определенного порта
    void updateBeaconFFrm( int bNum );//сюда принимается сигнал о необходимости обновить параметры определенного маяка

    void storeProgress( QString msg );
    void allLoaded( );

    void closeApp();       //закрытие клиентской части

private slots:

    //кнопки на первой закладке
    void saveButtonClick();   //сохранение параметров
    void cancelButtonClick(); //отменить изменения параметров
    void closeButtonClick();  //скрытие клиентской части

    void systrayclick ( ); //клик по иконке в трее

    //на закладке ports
    void openPortsButtonClick(); //открыть все порты
    void closePortsButtonClick();//закрыть все порты
    void openPortButtonClick();  //открыть определенный порт
    void closePortButtonClick(); //закрыть определенный порт
    void deletePortButtonClick();//удалить определенный порт
    void editPortButtonClick();  //редактировать параметры определенного порта
    void createPortButtonClick();//добавить порт
    void onPortDoubleClicked( const QModelIndex & index ); //при двойном щелчке по порту - открываем его параметры
    void portsCurrentChanged( const QModelIndex & current, const QModelIndex & previous );
    void updPortsButtonState ( );
    void onTimer(); //гасим зажженные лампочки

    //на закладке beacons
    void createBeaconButtonClick(); //создать маяк
    void editBeaconButtonClick();   //редактировать параметры определенного маяка
    void deleteBeaconButtonClick(); //удалить определенный маяк
    void openBeaconButtonClick();  //открыть определенный маяк
    void closeBeaconButtonClick(); //закрыть определенный маяк
    void sendBeaconButtonClick();//послать маяк
    void sendStatusButtonClick();//послать статус
    void openBeaconsButtonClick(); //открыть все маяки
    void closeBeaconsButtonClick();//закрыть все маяки


    void beaconsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous );
    void updBeaconsButtonState();


    //на закладке Query
    void qrySaveButtonClick();   //сохранение параметров
    void qryCancelButtonClick(); //отменить изменения параметров
    void qryCloseButtonClick();
    void qryAddCallButtonClick();//добавить позывной в список
    void qryDeleteCallButton();  //удалить позывной из списка

    void showPackets();    //показываем окно пакетов
    void showMessages();   //показываем окно сообщений
    void showStations();   //показываем окно станций
    void showMaps();       //показываем окно карт
    void showMessenger();  //показываем окно почтовика

    void weSysvarSet( QString VarName, QString VarVal );
    void weSysportSetParam( int PortNum, QString ParName, QString ParVal );
    void weSysportChangeState( int PortNum, QString PortState );

    void weSysbeaconSetParam( int BcknNum, QString ParName, QString ParVal );
    void weSysbeaconChangeState( int BcknNum, QString BcknState );

    void weportRXPacket( int pNum, bool isRX );
    void weportTXPacket( int pNum, bool isTX );

};

#endif // MAINWINDOW_H
