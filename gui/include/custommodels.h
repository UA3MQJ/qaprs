 #ifndef CUSTOMMODELS_H
 #define CUSTOMMODELS_H


#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QList>
#include <QStandardItem>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QPixmap>
#include <QFileIconProvider>
#include <QSize>
#include <QItemDelegate>
#include "../../core/include/qaprssymbols.h"

//модель для отображения портов
 class PortsModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     PortsModel(QObject *parent = 0);
     PortsModel( int rows, int columns, QObject * parent = 0 );
     QVariant data(const QModelIndex &item, int role) const;
     QFileIconProvider iconProvider;

 };

 //модель для отображения маяков
 class BeaconsModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     BeaconsModel(QObject *parent = 0);
     BeaconsModel( int rows, int columns, QObject * parent = 0 );
     QVariant data(const QModelIndex &item, int role) const;
     QFileIconProvider iconProvider;

     //QVariantMap *symNames;
     //QVariantMap *symImgs;
     //символы
     QAPRSSymbols *SysSymbols;


 };

 class PacketsModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     PacketsModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;
 };

 class MessagesModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     MessagesModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;
 };

 class StationsModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     StationsModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;

 };
 #endif
