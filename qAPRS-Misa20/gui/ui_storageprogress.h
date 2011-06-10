/********************************************************************************
** Form generated from reading UI file 'storageprogress.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STORAGEPROGRESS_H
#define UI_STORAGEPROGRESS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogStorageProgress
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QDialog *DialogStorageProgress)
    {
        if (DialogStorageProgress->objectName().isEmpty())
            DialogStorageProgress->setObjectName(QString::fromUtf8("DialogStorageProgress"));
        DialogStorageProgress->setWindowModality(Qt::ApplicationModal);
        DialogStorageProgress->resize(24, 31);
        DialogStorageProgress->setMouseTracking(false);
        DialogStorageProgress->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/main/toolbar/map.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        DialogStorageProgress->setWindowIcon(icon);
        DialogStorageProgress->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(DialogStorageProgress);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(DialogStorageProgress);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);


        retranslateUi(DialogStorageProgress);

        QMetaObject::connectSlotsByName(DialogStorageProgress);
    } // setupUi

    void retranslateUi(QDialog *DialogStorageProgress)
    {
        DialogStorageProgress->setWindowTitle(QApplication::translate("DialogStorageProgress", "Please Wait. Storage in progress...", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DialogStorageProgress: public Ui_DialogStorageProgress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STORAGEPROGRESS_H
