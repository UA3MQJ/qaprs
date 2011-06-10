/********************************************************************************
** Form generated from reading UI file 'messenger.ui'
**
** Created: Fri 15. Apr 09:54:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSENGER_H
#define UI_MESSENGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessengerWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *toComboBox;
    QSpacerItem *horizontalSpacer;
    QTableView *messagesTableView;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QTableView *callsTableView;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLineEdit *msgToEdit;
    QLabel *label_7;
    QLineEdit *msgFromEdit;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *portComboBox;
    QLabel *label_4;
    QLineEdit *patchEdit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *textEdit;
    QCheckBox *ACKcheckBox;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *sendButton;

    void setupUi(QMainWindow *MessengerWindow)
    {
        if (MessengerWindow->objectName().isEmpty())
            MessengerWindow->setObjectName(QString::fromUtf8("MessengerWindow"));
        MessengerWindow->resize(626, 452);
        centralwidget = new QWidget(MessengerWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(74, 0));
        label->setMaximumSize(QSize(64, 16777215));

        horizontalLayout->addWidget(label);

        toComboBox = new QComboBox(centralwidget);
        toComboBox->setObjectName(QString::fromUtf8("toComboBox"));
        toComboBox->setMinimumSize(QSize(120, 0));
        toComboBox->setMaximumSize(QSize(90, 90));

        horizontalLayout->addWidget(toComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        messagesTableView = new QTableView(centralwidget);
        messagesTableView->setObjectName(QString::fromUtf8("messagesTableView"));

        verticalLayout_2->addWidget(messagesTableView);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(100, 20));
        label_2->setMaximumSize(QSize(100, 20));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        callsTableView = new QTableView(centralwidget);
        callsTableView->setObjectName(QString::fromUtf8("callsTableView"));
        callsTableView->setMinimumSize(QSize(140, 0));
        callsTableView->setMaximumSize(QSize(140, 16777215));
        callsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        callsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(callsTableView);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        msgToEdit = new QLineEdit(centralwidget);
        msgToEdit->setObjectName(QString::fromUtf8("msgToEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(msgToEdit->sizePolicy().hasHeightForWidth());
        msgToEdit->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(msgToEdit);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        msgFromEdit = new QLineEdit(centralwidget);
        msgFromEdit->setObjectName(QString::fromUtf8("msgFromEdit"));
        sizePolicy.setHeightForWidth(msgFromEdit->sizePolicy().hasHeightForWidth());
        msgFromEdit->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(msgFromEdit);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        portComboBox = new QComboBox(centralwidget);
        portComboBox->setObjectName(QString::fromUtf8("portComboBox"));
        portComboBox->setMinimumSize(QSize(100, 0));
        portComboBox->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(portComboBox);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        patchEdit = new QLineEdit(centralwidget);
        patchEdit->setObjectName(QString::fromUtf8("patchEdit"));
        patchEdit->setMaxLength(100);

        horizontalLayout_4->addWidget(patchEdit);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        textEdit = new QLineEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        horizontalLayout_5->addWidget(textEdit);

        ACKcheckBox = new QCheckBox(centralwidget);
        ACKcheckBox->setObjectName(QString::fromUtf8("ACKcheckBox"));

        horizontalLayout_5->addWidget(ACKcheckBox);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        horizontalLayout_6->addWidget(sendButton);


        verticalLayout_3->addLayout(horizontalLayout_6);

        MessengerWindow->setCentralWidget(centralwidget);

        retranslateUi(MessengerWindow);

        QMetaObject::connectSlotsByName(MessengerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MessengerWindow)
    {
        MessengerWindow->setWindowTitle(QApplication::translate("MessengerWindow", "Messenger", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MessengerWindow", "Messages to:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MessengerWindow", "Messages from:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MessengerWindow", "To:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MessengerWindow", "From:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MessengerWindow", "Via Port", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MessengerWindow", "DIGI Patch", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MessengerWindow", "Text (%n)", 0, QApplication::UnicodeUTF8));
        ACKcheckBox->setText(QApplication::translate("MessengerWindow", "ACK", 0, QApplication::UnicodeUTF8));
        sendButton->setText(QApplication::translate("MessengerWindow", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MessengerWindow: public Ui_MessengerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSENGER_H
