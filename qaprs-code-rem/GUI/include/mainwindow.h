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

    QString CoreAddress;//адрес ядра
    QString CorePort;   //порт ядра

    //модели отображения
    //QStandardItemModel *model;
    PortsModel *pModel;
    //QStandardItemModel *bModel;
    BeaconsModel *bModel;

    //функции блокировки кнопок в зависимости от состояний портов и маяков
    //(например бокировка кнопки включения маяка, если он уже включен)
    void updPortsButtonState ( );
    void updBeaconsButtonState ( );

    //массивы, в которых хранятся соответсвия APRS символов
    QVariantMap *symNames; //их названиям
    QVariantMap *symImgs;  //их изображениям

protected:
    void changeEvent(QEvent *e);

private:
    void setDBValue( QString varname, QString varval ); //сохранение параметра в БД
    Ui::MainWindow *ui;
    QAPRSCore Core; //ядро
    QTcpSocket *tcpGUIClient; //сокет связи с ядром
    QByteArray allmsg; //массив полученных от ядра данных
    QTimer     *timer; //таймер (чтобы лампочки красиво мигали)

signals:
    void RCVPortParam( int pNum, QString pName, QString pVal ); //принят параметр порта
    void RCVBeaconParam( int bNum, QString pName, QString pVal ); //принят параметр маяка

public slots:
    void setPortParamFFrm( int pNum, QString pName, QString pVal  ); //сюда принимаются параметры от форм редактирования параметров портов
    void setBeaconParamFFrm( int bNum, QString pName, QString pVal  ); //сюда принимаются параметры от форм редактирования параметров маяков

private slots:
    //кнопки на первой закладке
    void saveButtonClick();   //сохранение параметров
    void cancelButtonClick(); //отменить изменения параметров
    void closeButtonClick();  //закрытие клиентской части
    //по портам
    void openPortsButtonClick(); //открыть все порты
    void closePortsButtonClick();//закрыть все порты
    void openPortButtonClick();  //открыть определенный порт
    void closePortButtonClick(); //закрыть определенный порт
    void deletePortButtonClick();//удалить определенный порт
    void editPortButtonClick();  //редактировать параметры определенного порта
    void createPortButtonClick();//добавить порт
    void onPortDoubleClicked( const QModelIndex & index ); //при двойном щелчке по порту - открываем его параметры
    //по маякам
    void editBeaconButtonClick();   //редактировать параметры определенного маяка
    void onBeaconButtonClick();     //включить определенный маяк
    void offBeaconButtonClick();    //выключить определенный маяк
    void deleteBeaconButtonClick(); //удалить определенный маяк
    void onAllBeaconsButtonClick(); //включить все маяки
    void offPortsButtonClick();     //выключить все маяки
    void createBeaconButtonClick(); //создать маяк
    void onBeaconDoubleClicked( const QModelIndex & index );//при двойном щелчке по маяку - открываем его параметры

    void ToLog (QString Text); //слот, в который помещаем все логи (не пашет!)
    void stopCore(); //обработчик сигнала STOP от ядра

    void tcpGUIClientconnected () ;
    void tcpGUIClientdisconnected ();
    void tcpGUIClienterror (QAbstractSocket::SocketError socketError);
    void tcpGUIClientread();

    void onTimer(); //гасим зажженные лампочки
    //обработчики событий смены текущей строки в таблицах отображения портов, маяков
    void portsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous );
    void beaconsCurrentChanged ( const QModelIndex & current, const QModelIndex & previous );

};

#endif // MAINWINDOW_H
