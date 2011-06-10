#include <QtGui/QApplication>
#include <QTranslator>
#include "./include/mainwindow.h"
#include "./include/startwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTime midnight( 0, 0, 0 );
    qsrand( QTime( 0, 0, 0 ).secsTo( QTime::currentTime() ) );

    QTranslator translator;
    if (translator.load("lng_ru", ".")==TRUE) {
        qDebug()<< "Lng OK";
    } else {
        qDebug()<< "Lng FALSE";
    }
    a.installTranslator(&translator);

    //MainWindow w;
    StartWindowImpl w;
    w.show();


    return a.exec();
}
