/********************************************************************************
** Form generated from reading UI file 'start.ui'
**
** Created: Fri 15. Apr 09:54:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_START_H
#define UI_START_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QRadioButton *inprCB;
    QRadioButton *outprCB;
    QLabel *label;
    QLineEdit *hostEdit;
    QLabel *label_2;
    QSpinBox *portBox;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QTextEdit *textEdit;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *goButton;

    void setupUi(QMainWindow *StartWindow)
    {
        if (StartWindow->objectName().isEmpty())
            StartWindow->setObjectName(QString::fromUtf8("StartWindow"));
        StartWindow->resize(617, 219);
        centralWidget = new QWidget(StartWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(280, 0));
        inprCB = new QRadioButton(groupBox);
        inprCB->setObjectName(QString::fromUtf8("inprCB"));
        inprCB->setGeometry(QRect(10, 30, 231, 20));
        inprCB->setChecked(true);
        outprCB = new QRadioButton(groupBox);
        outprCB->setObjectName(QString::fromUtf8("outprCB"));
        outprCB->setGeometry(QRect(10, 50, 211, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 90, 41, 16));
        hostEdit = new QLineEdit(groupBox);
        hostEdit->setObjectName(QString::fromUtf8("hostEdit"));
        hostEdit->setEnabled(false);
        hostEdit->setGeometry(QRect(50, 90, 113, 22));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 90, 31, 16));
        portBox = new QSpinBox(groupBox);
        portBox->setObjectName(QString::fromUtf8("portBox"));
        portBox->setGeometry(QRect(210, 90, 61, 22));
        portBox->setMaximum(65535);
        portBox->setValue(6000);

        horizontalLayout->addWidget(groupBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximum(0);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        verticalLayout_2->addWidget(progressBar);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        goButton = new QPushButton(centralWidget);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        horizontalLayout_2->addWidget(goButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        StartWindow->setCentralWidget(centralWidget);

        retranslateUi(StartWindow);

        QMetaObject::connectSlotsByName(StartWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StartWindow)
    {
        StartWindow->setWindowTitle(QApplication::translate("StartWindow", "GUI Start options ", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StartWindow", "Core starting parameters", 0, QApplication::UnicodeUTF8));
        inprCB->setText(QApplication::translate("StartWindow", "Create Core in GUI process", 0, QApplication::UnicodeUTF8));
        outprCB->setText(QApplication::translate("StartWindow", "Connect to exist Core", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StartWindow", "Host:", 0, QApplication::UnicodeUTF8));
        hostEdit->setText(QApplication::translate("StartWindow", "localhost", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StartWindow", "Port:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("StartWindow", "Comment", 0, QApplication::UnicodeUTF8));
        goButton->setText(QApplication::translate("StartWindow", "Go", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StartWindow: public Ui_StartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_START_H
