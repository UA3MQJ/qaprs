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

//������ ��� ����������� ������
 class PortsModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     PortsModel(QObject *parent = 0);
     PortsModel( int rows, int columns, QObject * parent = 0 );
     QVariant data(const QModelIndex &item, int role) const;
     QFileIconProvider iconProvider;

 };

 //������ ��� ����������� ������
 class BeaconsModel : public QStandardItemModel
 {
     Q_OBJECT

 public:
     BeaconsModel(QObject *parent = 0);
     BeaconsModel( int rows, int columns, QObject * parent = 0 );
     QVariant data(const QModelIndex &item, int role) const;
     QFileIconProvider iconProvider;

     QVariantMap *symNames;
     QVariantMap *symImgs;


 };


 #endif
