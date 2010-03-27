 #include <QtGui>

 #include "./include/custommodels.h"



//----------------
 PortsModel::PortsModel(QObject *parent)
     : QStandardItemModel(parent)
 {


 }

  PortsModel::PortsModel( int rows, int columns, QObject * parent  )
     : QStandardItemModel( rows,  columns, parent)
  {


  }

  QVariant PortsModel::data(const QModelIndex &index, int role) const
 {

      QVariant value = QStandardItemModel::data(index, role);
      QString  picName;

      if (role == Qt::DecorationRole) {

          if (index.column() == 0) {
              picName.clear();
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "n\\a" )
                      picName.append( ":images/port.png" );
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "Inet" )
                      picName.append( ":images/portinet.png" );
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "KISS" )
                      picName.append( ":images/portkiss.png" );
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "AGW" )
                      picName.append( ":images/portagw.png" );
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "AXIP" )
                      picName.append( ":images/portaxip.png" );
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "FLDIGI" )
                      picName.append( ":images/portfldigi.png" );
              if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "VASKA" )
                      picName.append( ":images/portvaska.png" );


              return QIcon( picName );
          }

          if (index.column() == 2) {

              //qDebug () << this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString();

              if ( this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "ON" )
                      return QIcon( ":images/porton.png" );

              if ( this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "OFF" )
                      return QIcon( ":images/portnot.png" );

              if ( this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "RX" )
                      return QIcon( ":images/portrx.png" );

              if ( this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "TX" )
                      return QIcon( ":images/porttx.png" );

              if ( this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString() == "ERROR" )
                      return QIcon( ":images/portoff.png" );


          }

      }

     return value;
 }

  //----------------
   BeaconsModel::BeaconsModel(QObject *parent)
       : QStandardItemModel(parent)
   {

       symNames = NULL;
       symImgs = NULL;

   }

    BeaconsModel::BeaconsModel( int rows, int columns, QObject * parent  )
       : QStandardItemModel( rows,  columns, parent)
    {


    }

    QVariant BeaconsModel::data(const QModelIndex &index, int role) const
   {

        QVariant value = QStandardItemModel::data(index, role);

        if (role == Qt::DecorationRole) {

            if (index.column() == 0) {

                //qDebug () << this->data( this->index( index.row(), 2, QModelIndex()), Qt::DisplayRole ).toString();

                if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "ON" )
                        return QIcon( ":images/porton.png" );

                if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "OFF" )
                        return QIcon( ":images/portnot.png" );

                if ( this->data( this->index( index.row(), 0, QModelIndex()), Qt::DisplayRole ).toString() == "TX" )
                        return QIcon( ":images/porttx.png" );


            }

            if (index.column() == 3) {

                if ( this->data( this->index( index.row(), 3, QModelIndex()), Qt::DisplayRole ).toString() != "" ) {
                    //qDebug() << symImgs->value( this->data( this->index( index.row(), 3, QModelIndex()), Qt::DisplayRole ).toString().left(2) ).toString().prepend(":images/");
                    return QIcon( symImgs->value( this->data( this->index( index.row(), 3, QModelIndex()), Qt::DisplayRole ).toString().left(2) ).toString().prepend("./symbols/") );
                }

            }

        }


       return value;
   }

