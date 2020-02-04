#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include <QCloseEvent>
#include <QStandardItemModel>
#include "./include/custommodels.h"
#include "ui_messages.h"
//
class MessagesWindowImpl : public QMainWindow, public Ui::MessagesWindow
{
Q_OBJECT
public:
        MessagesWindowImpl( QWidget * parent = nullptr );

        //QStandardItemModel messagesModel;
        MessagesModel messagesModel;
        PortsModel   *pModel;

private slots:
        //void RXAPRSMessage( int pNum, QString PType, QString PNote, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
        //void TXAPRSMessage( int pNum, QString PType, QString PNote, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
        void   RXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
        void   TXAPRSMessage( int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );
        void   addMessage( QString TRX, int pNum, int K, QString DateTime, QString To, QString From, QString MsgText, QString MID );

private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent  *event );


};
#endif




