#include <QtCore/QCoreApplication>
#include "../../core/include/qaprscore.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QAPRSCore Core;

    a.connect(&Core, SIGNAL(stop()), &a, SLOT(quit()));

    Core.Start();

    return a.exec();
}
