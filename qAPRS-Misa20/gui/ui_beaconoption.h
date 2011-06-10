/********************************************************************************
** Form generated from reading UI file 'beaconoption.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEACONOPTION_H
#define UI_BEACONOPTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogBeaconoption
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *_46;
    QLabel *label_93;
    QLineEdit *bckNumEdit;
    QVBoxLayout *_47;
    QLabel *label_94;
    QComboBox *portNumBox;
    QVBoxLayout *_49;
    QLabel *label_96;
    QComboBox *bckSymBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *_54;
    QLabel *label_101;
    QLineEdit *bckCallEdit;
    QVBoxLayout *_48;
    QLabel *label_95;
    QLineEdit *bckLatEdit;
    QVBoxLayout *_50;
    QLabel *label_97;
    QLineEdit *bckLngEdit;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_98;
    QLineEdit *bckIntervalEdit;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QPushButton *mulButton;
    QVBoxLayout *_52;
    QLabel *label_99;
    QLineEdit *bckUnprotoEdit;
    QVBoxLayout *_53;
    QLabel *label_100;
    QLineEdit *bckTextEdit;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *_55;
    QLabel *label_103;
    QLineEdit *statusTextEdit;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_102;
    QLineEdit *statusIntervalEdit;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QPushButton *mul2Button;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QHBoxLayout *_45;
    QSpacerItem *spacerItem;
    QPushButton *saveButton;
    QPushButton *closeButton;

    void setupUi(QMainWindow *DialogBeaconoption)
    {
        if (DialogBeaconoption->objectName().isEmpty())
            DialogBeaconoption->setObjectName(QString::fromUtf8("DialogBeaconoption"));
        DialogBeaconoption->resize(379, 360);
        centralwidget = new QWidget(DialogBeaconoption);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_6 = new QVBoxLayout(centralwidget);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        _46 = new QVBoxLayout();
        _46->setObjectName(QString::fromUtf8("_46"));
        _46->setSizeConstraint(QLayout::SetMinimumSize);
        label_93 = new QLabel(centralwidget);
        label_93->setObjectName(QString::fromUtf8("label_93"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_93->sizePolicy().hasHeightForWidth());
        label_93->setSizePolicy(sizePolicy);
        label_93->setMaximumSize(QSize(70, 16777215));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_93->setFont(font);

        _46->addWidget(label_93);

        bckNumEdit = new QLineEdit(centralwidget);
        bckNumEdit->setObjectName(QString::fromUtf8("bckNumEdit"));
        bckNumEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(bckNumEdit->sizePolicy().hasHeightForWidth());
        bckNumEdit->setSizePolicy(sizePolicy);
        bckNumEdit->setMinimumSize(QSize(0, 0));
        bckNumEdit->setMaximumSize(QSize(70, 16777215));
        bckNumEdit->setReadOnly(true);

        _46->addWidget(bckNumEdit);


        horizontalLayout_2->addLayout(_46);

        _47 = new QVBoxLayout();
        _47->setObjectName(QString::fromUtf8("_47"));
        label_94 = new QLabel(centralwidget);
        label_94->setObjectName(QString::fromUtf8("label_94"));
        label_94->setFont(font);

        _47->addWidget(label_94);

        portNumBox = new QComboBox(centralwidget);
        portNumBox->setObjectName(QString::fromUtf8("portNumBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(portNumBox->sizePolicy().hasHeightForWidth());
        portNumBox->setSizePolicy(sizePolicy1);
        portNumBox->setMinimumSize(QSize(10, 0));
        portNumBox->setMaximumSize(QSize(700, 16777215));

        _47->addWidget(portNumBox);


        horizontalLayout_2->addLayout(_47);

        _49 = new QVBoxLayout();
        _49->setObjectName(QString::fromUtf8("_49"));
        label_96 = new QLabel(centralwidget);
        label_96->setObjectName(QString::fromUtf8("label_96"));
        label_96->setFont(font);

        _49->addWidget(label_96);

        bckSymBox = new QComboBox(centralwidget);
        bckSymBox->setObjectName(QString::fromUtf8("bckSymBox"));
        sizePolicy1.setHeightForWidth(bckSymBox->sizePolicy().hasHeightForWidth());
        bckSymBox->setSizePolicy(sizePolicy1);
        bckSymBox->setMinimumSize(QSize(10, 0));
        bckSymBox->setMaximumSize(QSize(110, 16777215));

        _49->addWidget(bckSymBox);


        horizontalLayout_2->addLayout(_49);


        verticalLayout_6->addLayout(horizontalLayout_2);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_5 = new QVBoxLayout(tab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        _54 = new QVBoxLayout();
        _54->setObjectName(QString::fromUtf8("_54"));
        _54->setSizeConstraint(QLayout::SetMinimumSize);
        label_101 = new QLabel(tab);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        sizePolicy.setHeightForWidth(label_101->sizePolicy().hasHeightForWidth());
        label_101->setSizePolicy(sizePolicy);
        label_101->setMaximumSize(QSize(70, 16777215));
        label_101->setFont(font);

        _54->addWidget(label_101);

        bckCallEdit = new QLineEdit(tab);
        bckCallEdit->setObjectName(QString::fromUtf8("bckCallEdit"));
        bckCallEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(bckCallEdit->sizePolicy().hasHeightForWidth());
        bckCallEdit->setSizePolicy(sizePolicy);
        bckCallEdit->setMinimumSize(QSize(0, 0));
        bckCallEdit->setMaximumSize(QSize(70, 16777215));
        bckCallEdit->setMaxLength(9);
        bckCallEdit->setReadOnly(false);

        _54->addWidget(bckCallEdit);


        horizontalLayout_3->addLayout(_54);

        _48 = new QVBoxLayout();
        _48->setObjectName(QString::fromUtf8("_48"));
        label_95 = new QLabel(tab);
        label_95->setObjectName(QString::fromUtf8("label_95"));
        label_95->setFont(font);

        _48->addWidget(label_95);

        bckLatEdit = new QLineEdit(tab);
        bckLatEdit->setObjectName(QString::fromUtf8("bckLatEdit"));
        bckLatEdit->setMaxLength(9);

        _48->addWidget(bckLatEdit);


        horizontalLayout_3->addLayout(_48);

        _50 = new QVBoxLayout();
        _50->setObjectName(QString::fromUtf8("_50"));
        label_97 = new QLabel(tab);
        label_97->setObjectName(QString::fromUtf8("label_97"));
        label_97->setFont(font);

        _50->addWidget(label_97);

        bckLngEdit = new QLineEdit(tab);
        bckLngEdit->setObjectName(QString::fromUtf8("bckLngEdit"));
        bckLngEdit->setMaxLength(10);

        _50->addWidget(bckLngEdit);


        horizontalLayout_3->addLayout(_50);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_98 = new QLabel(tab);
        label_98->setObjectName(QString::fromUtf8("label_98"));
        label_98->setFont(font);

        verticalLayout_2->addWidget(label_98);

        bckIntervalEdit = new QLineEdit(tab);
        bckIntervalEdit->setObjectName(QString::fromUtf8("bckIntervalEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bckIntervalEdit->sizePolicy().hasHeightForWidth());
        bckIntervalEdit->setSizePolicy(sizePolicy2);
        bckIntervalEdit->setMaximumSize(QSize(70, 16777215));
        bckIntervalEdit->setMaxLength(10);

        verticalLayout_2->addWidget(bckIntervalEdit);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        mulButton = new QPushButton(tab);
        mulButton->setObjectName(QString::fromUtf8("mulButton"));
        sizePolicy2.setHeightForWidth(mulButton->sizePolicy().hasHeightForWidth());
        mulButton->setSizePolicy(sizePolicy2);
        mulButton->setMaximumSize(QSize(30, 100));

        verticalLayout->addWidget(mulButton);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(horizontalLayout_3);

        _52 = new QVBoxLayout();
        _52->setObjectName(QString::fromUtf8("_52"));
        label_99 = new QLabel(tab);
        label_99->setObjectName(QString::fromUtf8("label_99"));
        label_99->setFont(font);

        _52->addWidget(label_99);

        bckUnprotoEdit = new QLineEdit(tab);
        bckUnprotoEdit->setObjectName(QString::fromUtf8("bckUnprotoEdit"));
        bckUnprotoEdit->setMaxLength(255);

        _52->addWidget(bckUnprotoEdit);


        verticalLayout_5->addLayout(_52);

        _53 = new QVBoxLayout();
        _53->setObjectName(QString::fromUtf8("_53"));
        label_100 = new QLabel(tab);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setFont(font);

        _53->addWidget(label_100);

        bckTextEdit = new QLineEdit(tab);
        bckTextEdit->setObjectName(QString::fromUtf8("bckTextEdit"));
        bckTextEdit->setMaxLength(255);

        _53->addWidget(bckTextEdit);


        verticalLayout_5->addLayout(_53);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        _55 = new QVBoxLayout();
        _55->setObjectName(QString::fromUtf8("_55"));
        label_103 = new QLabel(tab);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setFont(font);

        _55->addWidget(label_103);

        statusTextEdit = new QLineEdit(tab);
        statusTextEdit->setObjectName(QString::fromUtf8("statusTextEdit"));
        statusTextEdit->setMaxLength(255);

        _55->addWidget(statusTextEdit);


        horizontalLayout_5->addLayout(_55);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_102 = new QLabel(tab);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setFont(font);

        verticalLayout_3->addWidget(label_102);

        statusIntervalEdit = new QLineEdit(tab);
        statusIntervalEdit->setObjectName(QString::fromUtf8("statusIntervalEdit"));
        sizePolicy2.setHeightForWidth(statusIntervalEdit->sizePolicy().hasHeightForWidth());
        statusIntervalEdit->setSizePolicy(sizePolicy2);
        statusIntervalEdit->setMaximumSize(QSize(70, 16777215));
        statusIntervalEdit->setMaxLength(10);

        verticalLayout_3->addWidget(statusIntervalEdit);


        horizontalLayout_4->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        mul2Button = new QPushButton(tab);
        mul2Button->setObjectName(QString::fromUtf8("mul2Button"));
        sizePolicy2.setHeightForWidth(mul2Button->sizePolicy().hasHeightForWidth());
        mul2Button->setSizePolicy(sizePolicy2);
        mul2Button->setMaximumSize(QSize(30, 100));

        verticalLayout_4->addWidget(mul2Button);


        horizontalLayout_4->addLayout(verticalLayout_4);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(17, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout_6->addWidget(tabWidget);

        _45 = new QHBoxLayout();
        _45->setObjectName(QString::fromUtf8("_45"));
        spacerItem = new QSpacerItem(211, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _45->addItem(spacerItem);

        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        _45->addWidget(saveButton);

        closeButton = new QPushButton(centralwidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        _45->addWidget(closeButton);


        verticalLayout_6->addLayout(_45);

        DialogBeaconoption->setCentralWidget(centralwidget);

        retranslateUi(DialogBeaconoption);

        portNumBox->setCurrentIndex(-1);
        bckSymBox->setCurrentIndex(-1);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogBeaconoption);
    } // setupUi

    void retranslateUi(QMainWindow *DialogBeaconoption)
    {
        DialogBeaconoption->setWindowTitle(QApplication::translate("DialogBeaconoption", "Edit Beacon Options", 0, QApplication::UnicodeUTF8));
        label_93->setText(QApplication::translate("DialogBeaconoption", "Beacon Num", 0, QApplication::UnicodeUTF8));
        label_94->setText(QApplication::translate("DialogBeaconoption", "Port Number", 0, QApplication::UnicodeUTF8));
        label_96->setText(QApplication::translate("DialogBeaconoption", "Beacon Symbol", 0, QApplication::UnicodeUTF8));
        label_101->setText(QApplication::translate("DialogBeaconoption", "Call", 0, QApplication::UnicodeUTF8));
        bckCallEdit->setInputMask(QString());
        label_95->setText(QApplication::translate("DialogBeaconoption", "Latitude", 0, QApplication::UnicodeUTF8));
        bckLatEdit->setInputMask(QApplication::translate("DialogBeaconoption", "00.00.00>A; ", 0, QApplication::UnicodeUTF8));
        bckLatEdit->setText(QApplication::translate("DialogBeaconoption", "00.00.00N", 0, QApplication::UnicodeUTF8));
        label_97->setText(QApplication::translate("DialogBeaconoption", "Langitude", 0, QApplication::UnicodeUTF8));
        bckLngEdit->setInputMask(QApplication::translate("DialogBeaconoption", "000.00.00>A; ", 0, QApplication::UnicodeUTF8));
        bckLngEdit->setText(QApplication::translate("DialogBeaconoption", "000.00.00E", 0, QApplication::UnicodeUTF8));
        label_98->setText(QApplication::translate("DialogBeaconoption", "Interval (s)", 0, QApplication::UnicodeUTF8));
        bckIntervalEdit->setInputMask(QString());
        mulButton->setText(QApplication::translate("DialogBeaconoption", "\321\20560", 0, QApplication::UnicodeUTF8));
        label_99->setText(QApplication::translate("DialogBeaconoption", "Unproto Address", 0, QApplication::UnicodeUTF8));
        bckUnprotoEdit->setInputMask(QString());
        label_100->setText(QApplication::translate("DialogBeaconoption", "Beacon Text", 0, QApplication::UnicodeUTF8));
        label_103->setText(QApplication::translate("DialogBeaconoption", "Status Text", 0, QApplication::UnicodeUTF8));
        label_102->setText(QApplication::translate("DialogBeaconoption", "Interval (s)", 0, QApplication::UnicodeUTF8));
        statusIntervalEdit->setInputMask(QString());
        mul2Button->setText(QApplication::translate("DialogBeaconoption", "\321\20560", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DialogBeaconoption", "General", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DialogBeaconoption", "Objects", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("DialogBeaconoption", "Save", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("DialogBeaconoption", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogBeaconoption: public Ui_DialogBeaconoption {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEACONOPTION_H
