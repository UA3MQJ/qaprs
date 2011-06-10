/********************************************************************************
** Form generated from reading UI file 'previewmap.ui'
**
** Created: Mon 1. Nov 11:18:44 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEWMAP_H
#define UI_PREVIEWMAP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogPreviewMap
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout;
    QPushButton *loadButton;
    QSpacerItem *spacerItem;
    QPushButton *cancelButton;

    void setupUi(QDialog *DialogPreviewMap)
    {
        if (DialogPreviewMap->objectName().isEmpty())
            DialogPreviewMap->setObjectName(QString::fromUtf8("DialogPreviewMap"));
        DialogPreviewMap->setWindowModality(Qt::ApplicationModal);
        DialogPreviewMap->resize(611, 522);
        DialogPreviewMap->setMouseTracking(false);
        DialogPreviewMap->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/main/toolbar/map.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        DialogPreviewMap->setWindowIcon(icon);
        DialogPreviewMap->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(DialogPreviewMap);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(DialogPreviewMap);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(graphicsView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(DialogPreviewMap);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label);

        label_4 = new QLabel(DialogPreviewMap);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(DialogPreviewMap);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_2);

        label_5 = new QLabel(DialogPreviewMap);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(DialogPreviewMap);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_3);

        label_6 = new QLabel(DialogPreviewMap);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_4->addWidget(label_6);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        loadButton = new QPushButton(DialogPreviewMap);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setMinimumSize(QSize(97, 0));
        loadButton->setMaximumSize(QSize(97, 16777215));

        horizontalLayout->addWidget(loadButton);

        spacerItem = new QSpacerItem(231, 26, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacerItem);

        cancelButton = new QPushButton(DialogPreviewMap);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(97, 0));
        cancelButton->setMaximumSize(QSize(97, 16777215));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DialogPreviewMap);
        QObject::connect(cancelButton, SIGNAL(clicked()), DialogPreviewMap, SLOT(close()));

        QMetaObject::connectSlotsByName(DialogPreviewMap);
    } // setupUi

    void retranslateUi(QDialog *DialogPreviewMap)
    {
        DialogPreviewMap->setWindowTitle(QApplication::translate("DialogPreviewMap", "Preview map", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogPreviewMap", "Description:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogPreviewMap", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogPreviewMap", "Top Left:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogPreviewMap", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogPreviewMap", "Bottom Right:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogPreviewMap", "TextLabel", 0, QApplication::UnicodeUTF8));
        loadButton->setText(QApplication::translate("DialogPreviewMap", "Load", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("DialogPreviewMap", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogPreviewMap: public Ui_DialogPreviewMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEWMAP_H
