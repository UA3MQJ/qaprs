 #include <QtGui>

 #include "customsqlmodels.h"



//----------------
 PortsSqlModel::PortsSqlModel(QObject *parent)
     : QSqlQueryModel(parent)
 {


 }

  QVariant PortsSqlModel::data(const QModelIndex &index, int role) const
 {
     QVariant value = QSqlQueryModel::data(index, role);
     QString  picName;
     QByteArray Sym;
     int symindex;
     
     if (role == Qt::DecorationRole) {
         if (index.column() == 10) {
             picName.clear();

             Sym.clear();
             Sym.append( this->record(index.row()).value("Psym").toString() );

             picName = ":images/";

             if (Sym.data()[0] == '/' )
                picName.append( "primary/" ); else
                picName.append( "alternate/" );

             symindex = (uchar( Sym.data()[1] )-33) ;

             Sym.clear();
             Sym.append( symindex );

             picName.append( Sym.toHex().toUpper() );
             picName.append( ".bmp" );

             return QIcon( picName );\
         }
         if (index.column() == 2) {
             picName.clear();
             if ( this->record(index.row()).value("PTID").toString() == "-1" )
                     picName.append( ":images/port.jpg" );
             if ( this->record(index.row()).value("PTID").toString() == "0" )
                     picName.append( ":images/portinet.jpg" );
             if ( this->record(index.row()).value("PTID").toString() == "1" )
                     picName.append( ":images/portkiss.jpg" );
             if ( this->record(index.row()).value("PTID").toString() == "2" )
                     picName.append( ":images/portagw.jpg" );
             if ( this->record(index.row()).value("PTID").toString() == "5" )
                     picName.append( ":images/portaxip.jpg" );

             return QIcon( picName );
         }
         if (index.column() == 11) {
             picName.clear();

             if ( this->APRSCore->Port[ this->record(index.row()).value("PN").toInt() ]==NULL ) {
                 picName.append( ":images/portnot.png" );
             }
             else {

                 if ( this->APRSCore->Port[ this->record(index.row()).value("PN").toInt() ]->active == TRUE ) {
                     picName.append( ":images/porton.png" );
                 } else {
                     picName.append( ":images/portoff.png" );
                 }

             }

             return QIcon( picName );
         }
     }

     return value;
 }


 PacketsSqlModel::PacketsSqlModel(QObject *parent)
     : QSqlQueryModel(parent)
 {


 }

 QVariant PacketsSqlModel::data(const QModelIndex &index, int role) const
 {
     QVariant value = QSqlQueryModel::data(index, role);
     QColor RowColor;

     if ( role == Qt::TextColorRole ) {
         if ( this->record(index.row()).value("TRX").toString()=="TX" ) RowColor.setRgb( 190, 0, 0 );
         if ( this->record(index.row()).value("TRX").toString()=="RX" ) RowColor.setRgb( 0, 144, 0 );
         return qVariantFromValue( RowColor );
     };

     return value;
 }

 MessagesSqlModel::MessagesSqlModel(QObject *parent)
     : QSqlQueryModel(parent)
 {


 }

 QVariant MessagesSqlModel::data(const QModelIndex &index, int role) const
 {
     QVariant value = QSqlQueryModel::data(index, role);
     QColor RowColor;

     if ( role == Qt::TextColorRole ) {
         if ( this->record(index.row()).value("TRX").toString()=="TX" ) RowColor.setRgb( 190, 0, 0 );
         if ( this->record(index.row()).value("TRX").toString()=="RX" ) RowColor.setRgb( 0, 144, 0 );
         return qVariantFromValue( RowColor );
     };

     return value;
 }


  StationsSqlModel::StationsSqlModel(QObject *parent)
     : QSqlQueryModel(parent)
 {


 }

 QVariant StationsSqlModel::data(const QModelIndex &index, int role) const
 {
     QVariant value = QSqlQueryModel::data(index, role);
     QString  picName;
     QByteArray Sym;
     int symindex;
     QDateTime LHDT;

     if (role == Qt::DecorationRole) {
         if (index.column() == 1) {
             picName.clear();

             Sym.clear();
             Sym.append( this->record(index.row()).value("sym").toString() );

             picName = ":images/";

             if (Sym.data()[0] == '/' )
                picName.append( "primary/" ); else
                picName.append( "alternate/" );

             symindex = (uchar( Sym.data()[1] )-33) ;

             Sym.clear();
             Sym.append( symindex );

             picName.append( Sym.toHex().toUpper() );
             picName.append( ".bmp" );

             return QIcon( picName );\
         }
     }


     if (value.isValid() && role == Qt::DisplayRole) {


         if (index.column() == 8) {

             if (this->record(index.row()).value("Lat").toString()=="" or
                 this->record(index.row()).value("Lng").toString()=="") {

                 value = "?";

             } else {


                qreal lenn = (calc_distance_law_of_cosines( lat2real( this->APRSCore->Lat ), long2real( this->APRSCore->Lng ),
                                                            lat2real( this->record(index.row()).value("Lat").toString() ),
                                                             long2real( this->record(index.row()).value("Lng").toString() ) ));
                value = lenn / 1000;

             }

             return  value;

         };

         if (index.column() == 9) {

             if (this->record(index.row()).value("Lat").toString()=="" or
                 this->record(index.row()).value("Lng").toString()=="") {

                 value = "?";

             } else {
                 qreal gradd = (calc_angle( lat2real( APRSCore->Lat ), long2real( APRSCore->Lng ),
                                       lat2real( this->record(index.row()).value("Lat").toString() ),
                                       long2real( this->record(index.row()).value("Lng").toString() ) ));
                 value = gradd;

             }

             return  value;

         };

         if (index.column() == 10) {

             LHDT = QDateTime::fromString( value.toString(), "yyyy-MM-dd HH:mm:ss" );
             value = LHDT.toString( "dd.MM.yyyy HH:mm:ss" );

             return  value;

         };

         if (index.column() == 11) {


             LHDT = QDateTime::fromString( this->record(index.row()).value("LH").toString(), "yyyy-MM-dd HH:mm:ss" );
             value =  LHDT.secsTo( QDateTime::currentDateTime() ) / 60 ;

             return  value;

         };

     }

     return value;
 }
