/********************************************************************************
** Form generated from reading UI file 'messages.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGES_H
#define UI_MESSAGES_H

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

class Ui_MessagesWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableView *messagesTableView;

    void setupUi(QMainWindow *MessagesWindow)
    {
        if (MessagesWindow->objectName().isEmpty())
            MessagesWindow->setObjectName(QString::fromUtf8("MessagesWindow"));
        MessagesWindow->resize(626, 362);
        centralwidget = new QWidget(MessagesWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        messagesTableView = new QTableView(centralwidget);
        messagesTableView->setObjectName(QString::fromUtf8("messagesTableView"));
        messagesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        messagesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(messagesTableView);

        MessagesWindow->setCentralWidget(centralwidget);

        retranslateUi(MessagesWindow);

        QMetaObject::connectSlotsByName(MessagesWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MessagesWindow)
    {
        MessagesWindow->setWindowTitle(QApplication::translate("MessagesWindow", "Messages", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MessagesWindow: public Ui_MessagesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGES_H
