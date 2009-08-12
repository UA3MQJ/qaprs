 #ifndef CUSTOMSQLMODELS_H
 #define CUSTOMSQLMODELS_H


#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QPixmap>
#include <QFileIconProvider>
#include <QSize>
#include <QItemDelegate>
#include "include/qaprscore.h"


 class PortsSqlModel : public QSqlQueryModel
 {
     Q_OBJECT

 public:
     PortsSqlModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;
     QFileIconProvider iconProvider;
     QAPRSCore *APRSCore;
 };

 class PacketsSqlModel : public QSqlQueryModel
 {
     Q_OBJECT

 public:
     PacketsSqlModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;
 };

  class MessagesSqlModel : public QSqlQueryModel
 {
     Q_OBJECT

 public:
     MessagesSqlModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;
 };

 class StationsSqlModel : public QSqlQueryModel
 {
     Q_OBJECT

 public:
     StationsSqlModel(QObject *parent = 0);
     QVariant data(const QModelIndex &item, int role) const;
     QAPRSCore *APRSCore;
 };

 #endif
