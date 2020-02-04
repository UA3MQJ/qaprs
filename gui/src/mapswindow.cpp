#include "include/mapswindow.h"
#include "include/loadmap.h"
#include "../../core/include/version.h"


//
MapsWindowImpl::MapsWindowImpl( QWidget * parent)
        : QMainWindow(parent)
{

    setupUi(this);

    SysStations = nullptr;
    SysSymbols  = nullptr;

    rz = 1;

    //scene = new QGraphicsScene();
    scene = new MvScene();
    scene->setBackgroundBrush( QColor(127, 127, 127) );
    graphicsView->setScene( scene );
    graphicsView->setDragMode( QGraphicsView::ScrollHandDrag );
    graphicsView->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

    map   = new QGraphicsPixmapItem();
    scene->addItem( map );

    actionLoad_Map  ->setEnabled( true );
    actionZoomIn    ->setEnabled( true );
    actionZoomOut   ->setEnabled( true );

    connect( actionLoad_Map, SIGNAL( activated() ),
             this, SLOT( loadMapClick() ) );

    connect( scene, SIGNAL( onMouseMove( QPointF ) ),
             this, SLOT( onMouseMove( QPointF ) ) );

    connect( actionZoomIn, SIGNAL( activated() ),
             this, SLOT( zoomInClick() ) );

    connect( actionZoomOut, SIGNAL( activated() ),
             this, SLOT( zoomOutClick() ) );

    connect( this->graphicsView, SIGNAL( onMouseZoomInOut() ),
             this, SLOT( zoomInOut() ) );



    QSettings settings( versionName, "MapWindow" );
    restoreGeometry( settings.value("geometry").toByteArray() );
    restoreState( settings.value("windowState").toByteArray() );

    InfFile = settings.value("InfFile").toString();
    ImgFile = settings.value("ImgFile").toString();

    if ( ( InfFile=="" ) || ( ImgFile=="" ) ) {
        //если первый запуск, то карты по умолчанию ставим
        InfFile = "world.inf";
        ImgFile = "world.jpg";
    }

    qDebug() << InfFile;
    qDebug() << ImgFile;

    setMap( InfFile, ImgFile );

}

MapsWindowImpl::~MapsWindowImpl() {

    SysStations->disconnect();
    this->disconnect();

}


void MapsWindowImpl::zoomInClick(){
    graphicsView->scale( 1.25, 1.25 );
    drawMap();
}

void MapsWindowImpl::zoomOutClick(){
    graphicsView->scale( 0.75, 0.75 );
    drawMap();
}

void MapsWindowImpl::zoomInOut(){
    drawMap();
}

void MapsWindowImpl::drawMap() {

    QAPRSStation *st;

    if ( SysStations != nullptr ) {

        qDebug() << "SysStations->stationCount=" << SysStations->stationCount;

        st = SysStations->firstStation;

        while ( st != nullptr ) {
            if ( st->sym != "" ) {

                updateStationOnMap( st->call );
                qDebug() << st->call << st->sym<< st->lat<< st->lng<< st->rlat<< st->rlng;

                //if ( st->call == "UA3MQJ" ) {

                    //qDebug() << st->symPic->transform();
                    //qDebug() << this->graphicsView->transform();
                    //qDebug() << (qreal)1/this->graphicsView->transform().m11();

                    qreal scaleFactor = (qreal)1/this->graphicsView->transform().m11();

                    st->symPic->setTransform( QTransform().scale( scaleFactor , scaleFactor ) );
                    st->callText->setTransform( QTransform().scale( scaleFactor , scaleFactor ) );
                    st->callRect->setTransform( QTransform().scale( scaleFactor , scaleFactor ) );

                //}

            }

            st = st->nextStation;
        }

    }

}

void MapsWindowImpl::updateStationOnMap( QString Call ) {

    QAPRSStation *st;

    st = SysStations->getStation( Call );

    if ( st != nullptr ) {

        if ( st->sym != "" ) {

            if ( st->symPic == nullptr ) {

                QString sym_img;
                sym_img = SysSymbols->getSymImg( st->sym );

                st->symPic = new QGraphicsPixmapItem();

                if ( ( st->sym.mid( 1, 1 ) == "#" ) &&
                    !( ( st->sym.mid( 0, 1 ) == "/" ) || ( st->sym.mid( 0, 1 ) == "\\" ) ) ) {

                    QPixmap ticon( "./symbols/alternate/02.png" );
                    QPainter paint;
                    paint.begin(&ticon);
                    //paint.drawText( QPointF(8 , 15), st->sym.mid( 0, 1) );
                    //paint.end();

                    QPen  p;
                    p.setColor( Qt::white );
                    paint.setPen( p );
                    paint.drawText( 8, 15, st->sym.mid( 0, 1) );

                    paint.drawText( 8+1, 15, st->sym.mid( 0, 1) );
                    paint.drawText( 8-1, 15, st->sym.mid( 0, 1) );
                    paint.drawText( 8, 15+1, st->sym.mid( 0, 1) );
                    paint.drawText( 8, 15-1, st->sym.mid( 0, 1) );

                    p.setColor( Qt::black );
                    paint.setPen( p );
                    paint.drawText( 8  , 15, st->sym.mid( 0, 1) );

                    paint.end();


                    st->symPic->setPixmap( ticon );

                } else {

                    QPixmap ticon( "./symbols/" + sym_img );

                    st->symPic->setPixmap( ticon );

                }



                scene->addItem( st->symPic );
                //чуть позже
                st->callText = scene->addText( st->call );

                QPen mypen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
                QBrush mybrush( Qt::white, Qt::SolidPattern );

                st->callRect = scene->addRect( QRectF( st->callText->boundingRect().x(),     st->callText->boundingRect().y() + 4,
                                                       st->callText->boundingRect().width(), st->callText->boundingRect().height() - 8 ), mypen, mybrush );
            }

            //наверное самый древний код во всем проекте
            //математика граф.части была написана еще до того,
            //как было решено писать ядро
            qreal ox = (((( (st->rlng) -shx)*sdx)-mmx1)/mdx)*mWidth;
            qreal oy = (((( (st->rlat) -shy)*sdy)-mmy1)/mdy)*mHeight;

            qreal scaleFactor = (qreal)1/this->graphicsView->transform().m11();

            st->symPic->setPos( map->mapToParent( ox, oy ).x() - ( scaleFactor * st->symPic->pixmap().width() / 2 ), map->mapToParent( ox, oy ).y() - ( scaleFactor * st->symPic->pixmap().height() / 2 ) );
            st->symPic->setZValue( (qreal)rz );

            st->callText->setPos( map->mapToParent( ox, oy ).x() + ( scaleFactor * st->symPic->pixmap().width() / 2 ) , map->mapToParent( ox, oy ).y() - ( scaleFactor * st->symPic->pixmap().height() / 2 ) - 2 * scaleFactor );
            st->callText->setZValue( (qreal)rz + 0.2 );

            st->callRect->setPos( st->callText->pos() );
            st->callRect->setZValue( (qreal)rz + 0.1 );

            rz = rz+1;

        }

    }

}

void MapsWindowImpl::loadMapClick(){

    qDebug() << "Load MAP";

    LoadMapDialog win;

    win.setWindowModality(Qt::ApplicationModal);

    connect( &win, SIGNAL( selectMap( QString, QString ) ),
             this, SLOT( selectMap( QString, QString ) ) );

    win.exec();

}

void MapsWindowImpl::selectMap( QString mapInfFile, QString mapImgFile ) {

    qDebug() << "select map - " << mapInfFile << " - " << mapImgFile;

    InfFile = mapInfFile;
    ImgFile = mapImgFile;

    setMap( mapInfFile, mapImgFile );

}

void MapsWindowImpl::setMap( QString mapInfFile, QString mapImgFile ) {

    map->setPixmap( QPixmap( "./maps/" + mapImgFile ) );
    map->setTransformationMode ( Qt::SmoothTransformation );

    map->setScale( 1 );

    scene->setSceneRect( -((qreal)map->pixmap().width()/2), -((qreal)map->pixmap().height()/2),
                         map->pixmap().width(), map->pixmap().height() );

    map->setZValue( 0 );
    map->setPos( -((qreal)map->pixmap().width()/2), -((qreal)map->pixmap().height()/2) );

    //уменьшаем карту под размер окошка
    if ( ((double)graphicsView->viewport()->width()/map->pixmap().width()) <
         ((double)graphicsView->viewport()->height()/map->pixmap().height()) ) {


        graphicsView->setTransform( QTransform().scale( (((double)graphicsView->viewport()->width())/map->pixmap().width()),
                                                        (((double)graphicsView->viewport()->width())/map->pixmap().width())
                                                       )
                                   );
    } else {

        graphicsView->setTransform( QTransform().scale( (((double)graphicsView->viewport()->height())/map->pixmap().height()),
                                                        (((double)graphicsView->viewport()->height())/map->pixmap().height())
                                                       )
                                   );
    }


    //и тут самый древний код во всем проекте
    //математика граф.части была написана еще до того,
    //как было решено писать ядро


    //делаем расчеты для привязки координат на экране к мировым
    QString tname, p1, p2, p3, p4;
    QFile file( "./maps/" + mapInfFile );

    file.open( QIODevice::ReadOnly );
    QTextStream in( &file );
    QString str1, str2, str3;

    str1 = in.readLine();
    str2 = in.readLine();
    str3 = in.readLine();

    p1 = str1.section( ",", 0, 0 ).trimmed();
    p2 = str1.section( ",", 1, 1 ).trimmed();

    p3 = str2.section( ",", 0, 0 ).trimmed();
    p4 = str2.section( ",", 1, 1 ).trimmed();

    // p1=x1 p2=y1 p3=x2 p4=y2 если буржуи перепутали координаты, то меняем местами x&y
    if ( ( p1.right( 1 ) == "N" ) || ( p1.right( 1 ) == "S" ) ) {
        str1 = p1; p1 = p2; p2 = str1;
        str2 = p3; p3 = p4; p4 = str2;
    }
    qDebug() << "p1=" << p1;
    qDebug() << "p2=" << p2;
    qDebug() << "p3=" << p3;
    qDebug() << "p4=" << p4;

    mrx1 = long2real( p1 );
    mry1 = lat2real( p2 );
    mrx2 = long2real( p3 );
    mry2 = lat2real( p4 );

    qDebug() << "mrx1=" << mrx1;
    qDebug() << "mry1=" << mry1;
    qDebug() << "mrx2=" << mrx2;
    qDebug() << "mry2=" << mry2;

    // размеры текущей карты
    mWidth  = map->pixmap().width();   // 0..map.sizeX
    mHeight = map->pixmap().height();  // 0..map.sizeY

    qDebug() << "mWidth=" << mWidth;
    qDebug() << "mHeight=" << mHeight;


    mdx = mWidth;
    mdy = mHeight;

    qDebug() << "mdx=" << mdx;
    qDebug() << "mdy=" << mdy;

    mrWidth  = mrx2 - mrx1;
    mrHeight = mry2 - mry1;

    qDebug() << "mrWidth=" << mrWidth;
    qDebug() << "mrHeight=" << mrHeight;

    sdx = mdx / mrWidth;
    sdy = mdy / mrHeight;

    qDebug() << "sdx=" << sdx;
    qDebug() << "sdy=" << sdy;

    mmx1 = 0;
    mmy1 = 0;

    shx = mrx1 - (mmx1 / sdx);
    shy = mry1 - (mmy1 / sdy);

    qDebug() << "shx=" << shx;
    qDebug() << "shy=" << shy;

    drawMap();

}

void MapsWindowImpl::closeEvent(QCloseEvent *event) {

    qDebug()<<"Maps Close event";

    QSettings settings( versionName, "MapWindow" );
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );
    settings.setValue( "InfFile", InfFile );
    settings.setValue( "ImgFile", ImgFile );

    hide();
    event->ignore();

    SysStations->disconnect();
    this->disconnect();
    this->close();

}

void MapsWindowImpl::showEvent ( QShowEvent * event ) {

    //уменьшаем карту под размер окошка
    if ( ((double)graphicsView->viewport()->width()/map->pixmap().width()) <
         ((double)graphicsView->viewport()->height()/map->pixmap().height()) ) {


        graphicsView->setTransform( QTransform().scale( (((double)graphicsView->viewport()->width())/map->pixmap().width()),
                                                        (((double)graphicsView->viewport()->width())/map->pixmap().width())
                                                       )
                                   );
    } else {

        graphicsView->setTransform( QTransform().scale( (((double)graphicsView->viewport()->height())/map->pixmap().height()),
                                                        (((double)graphicsView->viewport()->height())/map->pixmap().height())
                                                       )
                                   );
    }

    drawMap();

}

//void MapsWindowImpl::RCVStationInfo( QString Call, QString Sym, QString LH, QString Lat, QString Lng, QString Grid, QString Via, QString Stcomment ) {
void MapsWindowImpl::RCVStationInfo( QString Call ) {
    /*
   qDebug() << "MapsWindowImpl::RCVStationInfo Call=" << Call
                << "; Sym=" << Sym
                << "; LH=" << LH
                << "; Lat=" << Lat
                << "; Lng=" << Lng
                << "; Grid=" << Grid
                << "; Via=" << Via
                << "; Stcomment=" << Stcomment; */

    qDebug() << "MapsWindowImpl::RCVStationInfo Call=" << Call;

    updateStationOnMap( Call );

}

void MapsWindowImpl::onMouseMove( QPointF point ) {

    //qDebug() << "onMouseMove";

        qreal mx, my, px, py, ix, iy;

            mx = map->mapFromParent( point ).x();
            my = map->mapFromParent( point ).y();

            px = (mx / mWidth) * mdx + mmx1;
    //           |         |	 |
    //	       0...1	   |     |
    //                   0...2   |
    //                        -1...1
            py = (my / mHeight) * mdy + mmy1;


            //qDebug() << "px=" << (px) ;
            //qDebug() << "py=" << (py) ;

    // =============================
        //обратное преобразование из интервалов -1..1, -1..1 в координаты карты (см. zoom in/out)
            //ty = py / sdy + shy;
            //tx = px / sdx + shx;
            //из (-1..1 to 0..1) в координаты карты
            //ox = (((( (tx) -shx)*sdx)-mmx1)/mdx)*mWidth;
            //oy = (((( (ty) -shy)*sdy)-mmy1)/mdy)*mHeight;

            ix = px / sdx + shx;
            iy = py /sdy + shy;

            if (((mx>=0)&&(mx<=mWidth))&&((my>=0)&&(my<=mHeight))){

                //qDebug() << "mx=" << mx << ", my=" << my << ", px=" << px << ", py=" << py;

                posEdit->clear();
                posEdit->insertHtml( "<p align=center><b>" + real2lat( iy ) + "<br>" + real2long( ix ) + "</b></p>" );

                qthEdit->clear();
                qthEdit->insertHtml( "<p align=center><b>" + real2qthloc( iy, ix ) + "</b></p>" );
            }

}


MvScene::MvScene(QObject *parent) : QGraphicsScene(parent)
{

}

void MvScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    emit onMouseMove( event->scenePos() );

}


