#include <QApplication>
//#include <QMessageBox>
//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>
//#include <QSqlQueryModel>
//#include <QTableView>
#include "include/mainwindow.h"
//


int main(int argc, char ** argv)
{
	QApplication app( argc, argv );

	//if (!createConnection()) return 1;
      static QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
        
    MainWindow win;

    win.db = &db;
	win.show();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
