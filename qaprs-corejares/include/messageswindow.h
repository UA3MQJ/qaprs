#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H
//
#include <QMainWindow>
#include <QtNetwork>
#include <QTextStream>
#include "include/qaprscore.h"
#include "include/customsqlmodels.h"
#include "ui_messages.h"
//
class MessagesWindowImpl : public QMainWindow, public Ui::MessagesWindow
{
Q_OBJECT
public:
        MessagesWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
//	QSqlQueryModel plainModel;

        QSqlDatabase *db;
        QAPRSCore *APRSCore;

        MessagesSqlModel messagesModel;
        QString messagesQuery;
        bool isrequeringMessages;
        void requeryMessages();
        QString getDBVal( QString varname );
        void setDBVal( QString varname, QString varval );


private slots:
        void UpdateMessageList();

private:
        void closeEvent(QCloseEvent *event);
        void showEvent ( QShowEvent * event );


};
#endif




