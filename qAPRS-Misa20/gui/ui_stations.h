/********************************************************************************
** Form generated from reading UI file 'stations.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONS_H
#define UI_STATIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationsWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableView *stationsTableView;

    void setupUi(QMainWindow *StationsWindow)
    {
        if (StationsWindow->objectName().isEmpty())
            StationsWindow->setObjectName(QString::fromUtf8("StationsWindow"));
        StationsWindow->resize(626, 408);
        centralwidget = new QWidget(StationsWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stationsTableView = new QTableView(centralwidget);
        stationsTableView->setObjectName(QString::fromUtf8("stationsTableView"));
        stationsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        stationsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(stationsTableView);

        StationsWindow->setCentralWidget(centralwidget);

        retranslateUi(StationsWindow);

        QMetaObject::connectSlotsByName(StationsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StationsWindow)
    {
        StationsWindow->setWindowTitle(QApplication::translate("StationsWindow", "Stations", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StationsWindow: public Ui_StationsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONS_H
