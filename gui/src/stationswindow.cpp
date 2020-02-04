#include "include/stationswindow.h"
#include "../../core/include/version.h"
//
StationsWindowImpl::StationsWindowImpl( QWidget * parent)
        : QMainWindow(parent)
{
	setupUi(this);

        //symNames = nullptr;
        //symImgs  = nullptr;
        SysSymbols = nullptr;
        SysVars    = nullptr;

        genLat   = "";
        genLng   = "";

        stationsModel.setColumnCount( 13 );

        QStringList Labels;

        //Call, Sym, Comment, Lat, Lng, Grid, Via, StComment, QRB, Deg, LH, MinAgo "

        Labels << tr("Call") << tr("Sym") << tr("Comment") << tr("Lat") << tr("Lng") << tr("Grid") << tr("Via") << tr("StComment") << tr("StStatus") << tr("QRB(km)") << tr("Deg") << tr("LH") << tr("MinAgo");

        stationsModel.setHorizontalHeaderLabels( Labels );

        this->stationsTableView->setModel( &stationsModel );

        stationsTableView->setColumnWidth(  0,  80 );
        stationsTableView->setColumnWidth(  1,  30 );
        stationsTableView->setColumnWidth(  2,  70 );
        stationsTableView->setColumnWidth(  3,  60 );
        stationsTableView->setColumnWidth(  4,  67 );
        stationsTableView->setColumnWidth(  5,  55 );
        stationsTableView->setColumnWidth(  6,  110 );
        stationsTableView->setColumnWidth(  7,  110 );
        stationsTableView->setColumnWidth(  8,  110 );
        stationsTableView->setColumnWidth(  9,  55 );
        stationsTableView->setColumnWidth( 10,  55 );
        stationsTableView->setColumnWidth( 11,  110 );
        stationsTableView->setColumnWidth( 12,  55 );

        stationsTableView->setAlternatingRowColors(true);
        stationsTableView->setSortingEnabled(true);

        stationsModel.sort( 0, Qt::AscendingOrder );

        refresh.start( 10000 );

        connect( &refresh, SIGNAL( timeout() ), this, SLOT( onTimer() ) );


}

void StationsWindowImpl::showEvent ( QShowEvent * event ) {

    int i;

    qDebug() << "Stations show";

    //восстановление размеров и положения окна
    QSettings settings( versionName, "StationsWindow" );
    restoreGeometry( settings.value("geometry").toByteArray() );
    restoreState( settings.value("windowState").toByteArray() );

    //восстановление ширины столбцов
    for(i=0;i<=12;i++)
        if ( !settings.value("StationsWindowColumn" + QString::number( i ) + "Width").isNull() )
            stationsTableView->setColumnWidth( i, settings.value("StationsWindowColumn" + QString::number( i ) + "Width").toInt() );

    stationsTableView->sortByColumn( 0, Qt::AscendingOrder );

}

void StationsWindowImpl::closeEvent(QCloseEvent *event) {

    int i;

    qDebug()<<"Stations Close event";

    //сохранение размеров и положения окна
    QSettings settings( versionName, "StationsWindow" );
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );

    //сохранение ширины столбцов
    for(i=0;i<=12;i++)
        settings.setValue( "StationsWindowColumn" + QString::number( i ) + "Width", stationsTableView->columnWidth( i ) );

    this->hide();
    event->ignore();

}

void StationsWindowImpl::RCVStationPositionInfo( QString Call, QString Sym, QString LH, QString Lat, QString Lng, QString Grid, QString Via, QString Stcomment ) {

   /* qDebug() << "Call=" << Call
            << "; Sym=" << Sym
            << "; LH=" << LH
            << "; Lat=" << Lat
            << "; Lng=" << Lng
            << "; Grid=" << Grid
            << "; Via=" << Via
            << "; Stcomment=" << Stcomment; */


    //Call, Sym, Comment, Lat, Lng, Grid, Via, StComment, QRB, Deg, LH, MinAgo "

    int i;
    int ind=-1;

    for (i=0;i<stationsModel.rowCount();i++) {

        //qDebug() << stationsModel.data( stationsModel.index( i, 0 ), Qt::DisplayRole  );

        if ( stationsModel.data( stationsModel.index( i, 0 ), Qt::DisplayRole ) == Call ) {
            ind = i;
            break;
        }

    }

    if ( ind == -1 ) {
        ind = 0;
        stationsModel.insertRow( 0 );
    }

    stationsModel.setData( stationsModel.index( ind,  0 ), Call );

    if ( SysSymbols != nullptr)  {

        //по новому
        QString sym_img;
        sym_img = SysSymbols->getSymImg( Sym );

        if ( ( Sym.mid( 1, 1 ) == "#" ) &&
            !( ( Sym.mid( 0, 1 ) == "/" ) || ( Sym.mid( 0, 1 ) == "\\" ) ) ) {

            QPixmap ticon( "./symbols/alternate/02.png" );
            QPainter paint;
            paint.begin(&ticon);

            QPen  p;

            p.setColor( Qt::white );
            paint.setPen( p );
            paint.drawText( 8, 15, Sym.mid( 0, 1) );

            paint.drawText( 8+1, 15, Sym.mid( 0, 1) );
            paint.drawText( 8-1, 15, Sym.mid( 0, 1) );
            paint.drawText( 8, 15+1, Sym.mid( 0, 1) );
            paint.drawText( 8, 15-1, Sym.mid( 0, 1) );

            p.setColor( Qt::black );
            paint.setPen( p );
            paint.drawText( 8  , 15, Sym.mid( 0, 1) );

            paint.end();

            //st->symPic->setPixmap( ticon );
            stationsModel.setData( stationsModel.index( ind,  0 ), ticon, Qt::DecorationRole );

        } else {

            QPixmap ticon( "./symbols/" + sym_img );

            //st->symPic->setPixmap( ticon );
            stationsModel.setData( stationsModel.index( ind,  0 ), ticon, Qt::DecorationRole );

        }


    }


    stationsModel.setData( stationsModel.index( ind,  1 ), Sym );
    stationsModel.item( ind, 1 )->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    QString sym_name;
    sym_name = SysSymbols->getSymName( Sym );
    if ( sym_name != "")  {
        stationsModel.setData( stationsModel.index( ind,  2 ), sym_name );
    } else {
        stationsModel.setData( stationsModel.index( ind,  2 ), "" );
    }



    stationsModel.setData( stationsModel.index( ind,  3 ), Lat );
    stationsModel.setData( stationsModel.index( ind,  4 ), Lng );
    stationsModel.setData( stationsModel.index( ind,  5 ), Grid );
    stationsModel.setData( stationsModel.index( ind,  6 ), Via );
    stationsModel.setData( stationsModel.index( ind,  7 ), Stcomment );
    stationsModel.setData( stationsModel.index( ind, 11 ), LH.left( LH.length()-4 ) );

    //расчетные поля

    //расстояние
    if ( this->SysVars != nullptr ) genLat = this->SysVars->getVal( "Lat" );
    if ( this->SysVars != nullptr ) genLng = this->SysVars->getVal( "Lng" );

    if ( ( Lat == "" ) || ( Lng == "" ) || ( genLat == "" ) | ( genLng == "" ) ) {
        stationsModel.setData( stationsModel.index( ind,  9 ), "?" );
    } else {
        qreal lenn = (calc_distance_law_of_cosines( lat2real( genLat ), long2real( genLng ),
                                                    lat2real( Lat ),
                                                     long2real( Lng ) ));

        stationsModel.setData( stationsModel.index( ind,  9 ), (int)lenn / 1000 );

    }

    //угол
    if ( ( Lat == "" ) || ( Lng == "" ) || ( genLat == "" ) || ( genLng == "" ) ) {
        stationsModel.setData( stationsModel.index( ind,  10 ), "?" );
    } else {
        qreal gradd = (calc_angle( lat2real( genLat ), long2real( genLng ),
                              lat2real( Lat ),
                              long2real( Lng ) ) );

        stationsModel.setData( stationsModel.index( ind,  10 ), (int)gradd );

        if ( (int)gradd == 0 ) {

            stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/degNO.png" ), Qt::DecorationRole );

        } else {
            /*
            if ( ((int)gradd > 337) ||  ((int)gradd <  23) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg000.png" ), Qt::DecorationRole );
            if ( ((int)gradd >  22) && ((int)gradd <  67) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg045.png" ), Qt::DecorationRole );
            if ( ((int)gradd >  66) && ((int)gradd < 113) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg090.png" ), Qt::DecorationRole );
            if ( ((int)gradd > 112) && ((int)gradd < 158) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg135.png" ), Qt::DecorationRole );
            if ( ((int)gradd > 157) && ((int)gradd < 203) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg180.png" ), Qt::DecorationRole );
            if ( ((int)gradd > 202) && ((int)gradd < 248) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg225.png" ), Qt::DecorationRole );
            if ( ((int)gradd > 247) && ((int)gradd < 293) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg270.png" ), Qt::DecorationRole );
            if ( ((int)gradd > 292) && ((int)gradd < 338) ) stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ":images/deg315.png" ), Qt::DecorationRole );
            */

            QPixmap ticon( ":images/degNO.png" );
            //QPixmap ticon( 32, 32 );



            QPainter paint;

            paint.begin(&ticon);
            paint.setRenderHint( QPainter::HighQualityAntialiasing );
            paint.setTransform( QTransform().scale( 1, 1 ).translate( 16, 16 ).rotate( gradd ).translate( -16, -16 ) );
            paint.drawPixmap( 0, 0, 32, 32, QPixmap( ":images/deg000.png" ) );
            paint.end();

            stationsModel.setData( stationsModel.index( ind,  10 ), QIcon( ticon ), Qt::DecorationRole );

        }

        stationsModel.item( ind, 10 )->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    }

    //сколько минут назат слышал
    for (i=0;i<stationsModel.rowCount();i++) {

        stationsModel.setData( stationsModel.index( i, 12 ), QString::number( QDateTime::fromString( stationsModel.data( stationsModel.index( i, 11 ), Qt::DisplayRole ).toString(), "dd.MM.yyyy HH:mm:ss" ).secsTo( QDateTime::currentDateTime() ) / 60 ) );
    }


    //stationsTableView->sortByColumn( 0, Qt::AscendingOrder );


}

void StationsWindowImpl::RCVStationStatus( QString Call, QString Ststatus ) {


    int i;
    int ind=-1;

    for (i=0;i<stationsModel.rowCount();i++) {

        //qDebug() << stationsModel.data( stationsModel.index( i, 0 ), Qt::DisplayRole  );

        if ( stationsModel.data( stationsModel.index( i, 0 ), Qt::DisplayRole ) == Call ) {
            ind = i;
            break;
        }

    }

    if ( ind != -1 ) {

        stationsModel.setData( stationsModel.index( ind,  8 ), Ststatus );

    }

}

void StationsWindowImpl::onTimer() {

    int i;

    for (i=0;i<stationsModel.rowCount();i++) {
        stationsModel.setData( stationsModel.index( i, 12 ), QString::number( QDateTime::fromString( stationsModel.data( stationsModel.index( i, 11 ), Qt::DisplayRole ).toString(), "dd.MM.yyyy HH:mm:ss" ).secsTo( QDateTime::currentDateTime() ) / 60 ) );
    }

}

