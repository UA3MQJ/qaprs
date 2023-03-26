#ifndef STARTWINDOW_H
#define STARTWINDOW_H
//
#include <QMainWindow>
#include <QPaintEvent>
#include <QDebug>
#include "ui_start.h"
#include "include/optionswindow.h"
#include "../../core/include/qpubsub.h"


//
class StartWindowImpl : public QMainWindow, public Ui::StartWindow
{
Q_OBJECT
public:
        StartWindowImpl( QWidget * parent = nullptr );
        ~StartWindowImpl();

        OptionsWindow *optionsWindow;
        QTimer *timer;
        int myTimeout;
        QPubsub *pubsub;

signals:

public slots:
        void cb1clicked( bool checked );
        void cb2clicked( bool checked );
        void goClick();
        void allLoad();
        void readMsg( QString msg );
        void socketError();
        void timeout();
        void updateGoButton();

private:
        void closeEvent( QCloseEvent *event );
        void showEvent ( QShowEvent  *event );

};
#endif




