/********************************************************************************
** Form generated from reading UI file 'loadmap.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADMAP_H
#define UI_LOADMAP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogLoadMap
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *mapsView;
    QProgressBar *progressBar;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *loadButton;
    QPushButton *previewButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *DialogLoadMap)
    {
        if (DialogLoadMap->objectName().isEmpty())
            DialogLoadMap->setObjectName(QString::fromUtf8("DialogLoadMap"));
        DialogLoadMap->setWindowModality(Qt::ApplicationModal);
        DialogLoadMap->resize(611, 522);
        DialogLoadMap->setMouseTracking(false);
        DialogLoadMap->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/main/toolbar/map.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        DialogLoadMap->setWindowIcon(icon);
        DialogLoadMap->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(DialogLoadMap);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mapsView = new QTableView(DialogLoadMap);
        mapsView->setObjectName(QString::fromUtf8("mapsView"));
        mapsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mapsView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(mapsView);

        progressBar = new QProgressBar(DialogLoadMap);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(231, 26, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        loadButton = new QPushButton(DialogLoadMap);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setMinimumSize(QSize(97, 0));
        loadButton->setMaximumSize(QSize(97, 16777215));

        hboxLayout->addWidget(loadButton);

        previewButton = new QPushButton(DialogLoadMap);
        previewButton->setObjectName(QString::fromUtf8("previewButton"));
        previewButton->setEnabled(false);
        previewButton->setMinimumSize(QSize(97, 0));

        hboxLayout->addWidget(previewButton);

        cancelButton = new QPushButton(DialogLoadMap);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(97, 0));
        cancelButton->setMaximumSize(QSize(97, 16777215));

        hboxLayout->addWidget(cancelButton);


        verticalLayout->addLayout(hboxLayout);


        retranslateUi(DialogLoadMap);
        QObject::connect(cancelButton, SIGNAL(clicked()), DialogLoadMap, SLOT(close()));

        QMetaObject::connectSlotsByName(DialogLoadMap);
    } // setupUi

    void retranslateUi(QDialog *DialogLoadMap)
    {
        DialogLoadMap->setWindowTitle(QApplication::translate("DialogLoadMap", "Select a map", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("DialogLoadMap", "Load", 0, QApplication::UnicodeUTF8));
        previewButton->setText(QApplication::translate("DialogLoadMap", "Preview", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("DialogLoadMap", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogLoadMap: public Ui_DialogLoadMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADMAP_H
