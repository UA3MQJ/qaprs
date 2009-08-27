#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTranslator>
#include "include/mainwindowimpl.h"
//


int main(int argc, char *argv[]) {

       QApplication app( argc, argv );


	//if (!createConnection()) return 1;
        static QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
        //QSqlDatabase *db;
        //db = new QSqlDatabase::QSqlDatabase();
        //db->addDatabase( "QSQLITE" );


     QTranslator translator;
     if (translator.load("qaprs-core_ru", ".")==TRUE) {
         qDebug()<< "Lng OK";
     } else {
         qDebug()<< "Lng FALSE";
     }
     app.installTranslator(&translator);

        MainWindowImpl win;

        win.db = &db;
        //win.db = db;

	win.show();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();

}
