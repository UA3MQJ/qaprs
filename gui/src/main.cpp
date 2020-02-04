#include <QApplication>
#include <QTranslator>
#include "./include/mainwindow.h"
#include "./include/startwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize sequence
    qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch() / 1000));

    QTranslator translator;
    if (translator.load("lng_ru", ".")==true) {
        qDebug()<< "Lng OK";
    } else {
        qDebug()<< "Lng false";
    }
    a.installTranslator(&translator);

    //MainWindow w;
    StartWindowImpl w;
    w.show();


    return a.exec();
}
