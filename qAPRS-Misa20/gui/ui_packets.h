/********************************************************************************
** Form generated from reading UI file 'packets.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PACKETS_H
#define UI_PACKETS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PacketsWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableView *packetTableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *truncButton;

    void setupUi(QMainWindow *PacketsWindow)
    {
        if (PacketsWindow->objectName().isEmpty())
            PacketsWindow->setObjectName(QString::fromUtf8("PacketsWindow"));
        PacketsWindow->resize(626, 408);
        centralwidget = new QWidget(PacketsWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        packetTableView = new QTableView(centralwidget);
        packetTableView->setObjectName(QString::fromUtf8("packetTableView"));
        packetTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        packetTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(packetTableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        truncButton = new QPushButton(centralwidget);
        truncButton->setObjectName(QString::fromUtf8("truncButton"));

        horizontalLayout->addWidget(truncButton);


        verticalLayout->addLayout(horizontalLayout);

        PacketsWindow->setCentralWidget(centralwidget);

        retranslateUi(PacketsWindow);

        QMetaObject::connectSlotsByName(PacketsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *PacketsWindow)
    {
        PacketsWindow->setWindowTitle(QApplication::translate("PacketsWindow", "Packets", 0, QApplication::UnicodeUTF8));
        truncButton->setText(QApplication::translate("PacketsWindow", "Truncate Packets Log", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PacketsWindow: public Ui_PacketsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PACKETS_H
