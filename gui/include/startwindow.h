#ifndef STARTWINDOW_H
#define STARTWINDOW_H
//
#include <QMainWindow>
#include <QPaintEvent>
#include <QDebug>
#include "ui_start.h"
#include "include/mainwindow.h"


//
class StartWindowImpl : public QMainWindow, public Ui::StartWindow
{
Q_OBJECT
public:
        StartWindowImpl( QWidget * parent = nullptr );
        ~StartWindowImpl();

        MainWindow *MainWin;
        QTimer *timer;
        int myTimeout;

signals:

public slots:
        void cb1clicked( bool checked );
        void cb2clicked( bool checked );
        void goClick();
        void allLoad();
        void readMsg( QString msg );
        void socketError();
        void timeout();


private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent  *event );

};
#endif




