/********************************************************************************
** Form generated from reading UI file 'maps.ui'
**
** Created: Fri 15. Apr 09:54:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPS_H
#define UI_MAPS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "include/mygraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MapsWindow
{
public:
    QAction *actionExit;
    QAction *actionTerminal;
    QAction *actionStationList;
    QAction *actionMessages;
    QAction *actionLoadMap;
    QAction *actionZoomIN;
    QAction *actionZoomOUT;
    QAction *actionLoadMap_2;
    QAction *action;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionLoad_Map;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    myGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QTextEdit *qthEdit;
    QTextEdit *msgEdit;
    QTextEdit *posEdit;
    QMenuBar *menuBar;
    QMenu *menuMap;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MapsWindow)
    {
        if (MapsWindow->objectName().isEmpty())
            MapsWindow->setObjectName(QString::fromUtf8("MapsWindow"));
        MapsWindow->resize(656, 430);
        MapsWindow->setAutoFillBackground(false);
        actionExit = new QAction(MapsWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionTerminal = new QAction(MapsWindow);
        actionTerminal->setObjectName(QString::fromUtf8("actionTerminal"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/main/toolbar/terminal.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actionTerminal->setIcon(icon);
        actionStationList = new QAction(MapsWindow);
        actionStationList->setObjectName(QString::fromUtf8("actionStationList"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/main/toolbar/list.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actionStationList->setIcon(icon1);
        actionMessages = new QAction(MapsWindow);
        actionMessages->setObjectName(QString::fromUtf8("actionMessages"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/main/toolbar/mesg.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actionMessages->setIcon(icon2);
        actionLoadMap = new QAction(MapsWindow);
        actionLoadMap->setObjectName(QString::fromUtf8("actionLoadMap"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/main/toolbar/map.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadMap->setIcon(icon3);
        actionZoomIN = new QAction(MapsWindow);
        actionZoomIN->setObjectName(QString::fromUtf8("actionZoomIN"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/main/toolbar/z_in.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIN->setIcon(icon4);
        actionZoomOUT = new QAction(MapsWindow);
        actionZoomOUT->setObjectName(QString::fromUtf8("actionZoomOUT"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/main/toolbar/z_out.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOUT->setIcon(icon5);
        actionLoadMap_2 = new QAction(MapsWindow);
        actionLoadMap_2->setObjectName(QString::fromUtf8("actionLoadMap_2"));
        action = new QAction(MapsWindow);
        action->setObjectName(QString::fromUtf8("action"));
        actionZoomIn = new QAction(MapsWindow);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        actionZoomIn->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/z_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIn->setIcon(icon6);
        actionZoomOut = new QAction(MapsWindow);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        actionZoomOut->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/z_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOut->setIcon(icon7);
        actionLoad_Map = new QAction(MapsWindow);
        actionLoad_Map->setObjectName(QString::fromUtf8("actionLoad_Map"));
        actionLoad_Map->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/mopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Map->setIcon(icon8);
        centralwidget = new QWidget(MapsWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new myGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setMouseTracking(true);

        verticalLayout->addWidget(graphicsView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        qthEdit = new QTextEdit(centralwidget);
        qthEdit->setObjectName(QString::fromUtf8("qthEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qthEdit->sizePolicy().hasHeightForWidth());
        qthEdit->setSizePolicy(sizePolicy);
        qthEdit->setMinimumSize(QSize(100, 41));
        qthEdit->setMaximumSize(QSize(71, 41));
        qthEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        qthEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        horizontalLayout->addWidget(qthEdit);

        msgEdit = new QTextEdit(centralwidget);
        msgEdit->setObjectName(QString::fromUtf8("msgEdit"));
        sizePolicy.setHeightForWidth(msgEdit->sizePolicy().hasHeightForWidth());
        msgEdit->setSizePolicy(sizePolicy);
        msgEdit->setMinimumSize(QSize(0, 41));
        msgEdit->setMaximumSize(QSize(16777215, 41));
        msgEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        horizontalLayout->addWidget(msgEdit);

        posEdit = new QTextEdit(centralwidget);
        posEdit->setObjectName(QString::fromUtf8("posEdit"));
        sizePolicy.setHeightForWidth(posEdit->sizePolicy().hasHeightForWidth());
        posEdit->setSizePolicy(sizePolicy);
        posEdit->setMinimumSize(QSize(120, 41));
        posEdit->setMaximumSize(QSize(81, 41));
        posEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        posEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        horizontalLayout->addWidget(posEdit);


        verticalLayout->addLayout(horizontalLayout);

        MapsWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MapsWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 656, 24));
        menuMap = new QMenu(menuBar);
        menuMap->setObjectName(QString::fromUtf8("menuMap"));
        MapsWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MapsWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MapsWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuMap->menuAction());
        menuMap->addAction(actionLoad_Map);
        menuMap->addSeparator();
        menuMap->addAction(actionZoomIn);
        menuMap->addAction(actionZoomOut);
        toolBar->addAction(actionLoad_Map);
        toolBar->addSeparator();
        toolBar->addAction(actionZoomIn);
        toolBar->addAction(actionZoomOut);

        retranslateUi(MapsWindow);

        QMetaObject::connectSlotsByName(MapsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MapsWindow)
    {
        MapsWindow->setWindowTitle(QApplication::translate("MapsWindow", "Map", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MapsWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionTerminal->setText(QApplication::translate("MapsWindow", "Terminal", 0, QApplication::UnicodeUTF8));
        actionStationList->setText(QApplication::translate("MapsWindow", "Station list", 0, QApplication::UnicodeUTF8));
        actionMessages->setText(QApplication::translate("MapsWindow", "Messages", 0, QApplication::UnicodeUTF8));
        actionLoadMap->setText(QApplication::translate("MapsWindow", "Load map", 0, QApplication::UnicodeUTF8));
        actionZoomIN->setText(QApplication::translate("MapsWindow", "Zoom IN", 0, QApplication::UnicodeUTF8));
        actionZoomOUT->setText(QApplication::translate("MapsWindow", "Zoom OUT", 0, QApplication::UnicodeUTF8));
        actionLoadMap_2->setText(QApplication::translate("MapsWindow", "Load Map", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("MapsWindow", "---", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("MapsWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setText(QApplication::translate("MapsWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionLoad_Map->setText(QApplication::translate("MapsWindow", "Load Map", 0, QApplication::UnicodeUTF8));
        msgEdit->setHtml(QApplication::translate("MapsWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        menuMap->setTitle(QApplication::translate("MapsWindow", "Map", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MapsWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapsWindow: public Ui_MapsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPS_H
