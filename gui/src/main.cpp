#include <QApplication>
#include <QTranslator>
#include <QRandomGenerator>
#include "./include/startwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    if (translator.load("lng_ru", ".")==true) {
        qDebug()<< "Lng OK";
    } else {
        qDebug()<< "Lng false";
    }
    a.installTranslator(&translator);

    StartWindowImpl w;
    w.show();

    return a.exec();
}
