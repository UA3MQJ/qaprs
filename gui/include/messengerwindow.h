#ifndef MESSENGERWINDOW_H
#define MESSENGERWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include <QCloseEvent>
#include <QSettings>
#include <QStandardItemModel>
#include <QList>
#include "./include/custommodels.h"
#include "../../core/include/qatomix.h"
#include "ui_messenger.h"
//
class MessengerWindowImpl : public QMainWindow, public Ui::MessengerWindow
{
Q_OBJECT
public:
        MessengerWindowImpl( QWidget * parent = nullptr );

        //станции
        QAPRSstationList *SysStations;
        //принятые/переданные пакеты
        QAPRSPacketList  *SysPackets;
        //системные переменные
        QSysVars         *SysVars;
        //atom
        QAtomix          *atomGUI;

        QStandardItemModel stationsModel;
        QStandardItemModel messagesModel;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent( QShowEvent *event );
    void hideEvent (QHideEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:

public slots:
        void   RXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
        void   TXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );

        void   callCurrentChanged( const QModelIndex & current, const QModelIndex & previous );

        void   toFromChanged();
        void   msgTextChanged();

        void   msgSend();

        void updateStationList();
        void updateMessageList();

private:



};
#endif




