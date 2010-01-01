#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDomDocument>
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


    //загружаем конфиг из XML

    QDomDocument doc("qAPRS_Conf");
    QFile fileConfig("./config.xml");
    if (!fileConfig.open(QIODevice::ReadOnly))
        return 1;
    if (!doc.setContent(&fileConfig)) {
        fileConfig.close();
        return 1;
    }
    fileConfig.close();

    QDomElement docElem = doc.documentElement();
    QString lang;

    lang="en";

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if (e.tagName()=="Lang")    lang = e.text();
        }
        n = n.nextSibling();
    }


     QTranslator translator;
     if (translator.load("qaprs-core_"+lang, ".")==TRUE) {
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
