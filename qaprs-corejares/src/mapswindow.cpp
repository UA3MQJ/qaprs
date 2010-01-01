#include "mapswindow.h"
#include "loadmap.h"

////////////////////////////////////////
aprsStation::aprsStation( QGraphicsItem * parent, QGraphicsScene * scene )
    : QGraphicsPixmapItem(parent,scene)
{

}

void aprsStation::setParams( QString ncall, QString nlat, QString nlng, QString npix, QGraphicsScene * nscene )
{
        this->call = ncall;
        this->rlat = lat2real(nlat);
        this->rlng = long2real(nlng);
        this->lat = nlat;
        this->lng = nlng;
        this->setPixmap( QPixmap( npix ) );

        this->callText = nscene->addText( ncall );
        this->callText->setZValue(3);

        QPen mypen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QBrush mybrush(Qt::yellow, Qt::SolidPattern);
        //this->callRect = nscene->addRect(this->callText->boundingRect(), mypen, mybrush);

        this->callRect = nscene->addRect(QRectF(this->callText->boundingRect().x(), this->callText->boundingRect().y()+4,
                                                this->callText->boundingRect().width(), this->callText->boundingRect().height()-8 ), mypen, mybrush);

        this->callRect->setZValue(2);
        nscene->addItem( this );
        setZValue( 1 );

        qDebug() << ncall;

}

aprsStation::~aprsStation()
{

}
/////////////////////////////////////////
aprsMap::aprsMap( QGraphicsItem * parent, QGraphicsScene * scene )
    : QGraphicsPixmapItem(parent,scene)
{


}


//

aprsMap::~aprsMap()
{

}
/////////////////////////


MvScene::MvScene(QObject *parent) : QGraphicsScene(parent)
{
    map = NULL;
}

void MvScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
        qreal mx, my, px, py, ix, iy;



        if (!(map==NULL)) {
            mx = map->mapFromParent( event->scenePos() ).x();
            my = map->mapFromParent( event->scenePos() ).y();

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

            if (((mx>=0)and(mx<=mWidth))and((my>=0)and(my<=mHeight))){
                posEdit->clear();
                posEdit->insertHtml( "<p align=center><b>" + real2lat( iy ) + "<br>" + real2long( ix ) + "</b></p>" );

                qthEdit->clear();
                qthEdit->insertHtml( "<p align=center><b>" + real2qthloc( iy, ix ) + "</b></p>" );
            }
        }

}

int MvScene::loadMap(QString mapName) {

        QString tname, p1, p2, p3, p4;

        tname = mapName;

        qDebug() << "Maps loadmap " << mapName;

        if (map==NULL) {
            map = new aprsMap();
        } else {
            delete map;
            map = new aprsMap();
        }

        map->setPixmap( QPixmap( tname ) );

        map->setTransformationMode ( Qt::SmoothTransformation );

        addItem( map );

        setSceneRect( 0, 0,
                        map->mapToParent( map->pixmap().width(), map->pixmap().height() ).x(),
                        map->mapToParent( map->pixmap().width(), map->pixmap().height() ).y()
                        );

        QFile file( tname.left( tname.length()-4 ) + ".inf" );
        qDebug() << tname.left( tname.length()-4 ) + ".inf";

        file.open( QIODevice::ReadOnly );
        QTextStream in( &file );
        QString str1, str2, str3;

        str1 = in.readLine();
        str2 = in.readLine();
        str3 = in.readLine();

        p1 = str1.section( ",", 0, 0 );
        p2 = str1.section( ",", 1, 1 );
        if (p2[0]==' ') p2.remove(0,1);

        p3 = str2.section( ",", 0, 0 );
        p4 = str2.section( ",", 1, 1 );
        if (p4[0]==' ') p4.remove(0,1);

        // p1=x1 p2=y1 p3=x2 p4=y2 если буржуи перепутали координаты, то меняем местами x&y
        if ( p1[ p1.length()-1 ]=='N' or p1[ p1.length()-1 ]=='S' ) {
                str1 = p1; p1 = p2; p2 = str1;
                str2 = p3; p3 = p4; p4 = str2;
        };
        //qDebug() << "p1=" << p1;
        //qDebug() << "p2=" << p2;
        //qDebug() << "p3=" << p3;
        //qDebug() << "p4=" << p4;

        map->mapName = str3;

        mrx1 = long2real( p1 );

        mry1 = lat2real( p2 );

        mrx2 = long2real( p3 );

        mry2 = lat2real( p4 );

        //qDebug() << "mrx1=" << mrx1;
        //qDebug() << "mry1=" << mry1;
        //qDebug() << "mrx2=" << mrx2;
        //qDebug() << "mry2=" << mry2;

    // размеры текущей карты
        mWidth  = map->pixmap().width();   // 0..map.sizeX
        mHeight = map->pixmap().height();  // 0..map.sizeY

        //qDebug() << "mWidth=" << mWidth;
        //qDebug() << "mHeight=" << mHeight;

    //коэффициенты
    //mdx = mmx2 - mmx1;
    //mdy = mmy2 - mmy1;
        mdx = mWidth;
        mdy = mHeight;

        //qDebug() << "mdx=" << mdx;
        //qDebug() << "mdy=" << mdy;

        mrWidth  = mrx2 - mrx1;
        mrHeight = mry2 - mry1;

        //qDebug() << "mrWidth=" << mrWidth;
        //qDebug() << "mrHeight=" << mrHeight;

        sdx = mdx / mrWidth;
        sdy = mdy / mrHeight;

        //qDebug() << "sdx=" << sdx;
        //qDebug() << "sdy=" << sdy;

        shx = mrx1 - (mmx1 / sdx);
        shy = mry1 - (mmy1 / sdy);

        //qDebug() << "shx=" << shx;
        //qDebug() << "shy=" << shy;

        return 0;
}

void MvScene::drawMap() {

        qreal ox, oy, rz;
        aprsStation tstation;

        map->setPos(0,0);

        //рисуем объекты (доделывать надо)

        rz = 2;

        for (int i=0; i<stationList.size();i++) {

                obj = stationList.at(i);

                ox = (((( (obj->rlng) -shx)*sdx)-mmx1)/mdx)*mWidth;
                oy = (((( (obj->rlat) -shy)*sdy)-mmy1)/mdy)*mHeight;
                obj->setPos( map->mapToParent(ox,oy).x()-(obj->pixmap().width()/2) ,map->mapToParent(ox,oy).y()-(obj->pixmap().width()/2) );
                obj->setZValue(rz++);
                obj->callText->setPos( map->mapToParent(ox,oy).x()+(obj->pixmap().width()/2)+2, map->mapToParent(ox,oy).y()-(obj->pixmap().width()/2)+2 );
                obj->callRect->setZValue(rz++);
                obj->callRect->setPos( obj->callText->pos() );
                obj->callText->setZValue(rz++);

        };

        setSceneRect( 0, 0,
                        map->mapToParent( map->pixmap().width(), map->pixmap().height() ).x(),
                        map->mapToParent( map->pixmap().width(), map->pixmap().height() ).y()
                        );

}

//
MapsWindowImpl::MapsWindowImpl( QWidget * parent, Qt::WFlags f)
        : QMainWindow(parent, f)
{

    setupUi(this);

    scene = new MvScene;

    graphicsView->setScene( scene );

    scene->qthEdit = qthEdit;
    scene->msgEdit = msgEdit;
    scene->posEdit = posEdit;

    //scene->map = new aprsMap();

    //scene->loadMap( getDBVal("map") );


	//scene->map->setTransformationMode ( Qt::SmoothTransformation );

    //scene->addItem( scene->map );

    //scene->drawMap();

        QObject::connect( actionZoomIn, SIGNAL( activated() ),
                                  this, SLOT( zoomInClick() ) );

        QObject::connect( actionZoomOut, SIGNAL( activated() ),
                                   this, SLOT( zoomOutClick() ) );

        QObject::connect( actionLoad_Map, SIGNAL( activated() ),
                                    this, SLOT( loadMapClick() ) );

}

int MapsWindowImpl::loadMap(QString mapName) {

    actionLoad_Map  ->setEnabled( TRUE );
    actionZoomIn    ->setEnabled( TRUE );
    actionZoomOut   ->setEnabled( TRUE );

    scene->loadMap( mapName );

    //scene->obj = new aprsStation();
    //scene->obj->setParams( "UA3MQJ", "58.01.83N", "038.51.13E", ":images/alternate/0C.png", scene);
    //scene->stationList << scene->obj;
    //scene->drawMap();

    //stationsQuery = "select t1.Call, t1.Sym, t2.Comment, t1.Lat, t1.Lng, t1.Grid, t1.Via, t1.StComment, null as Length, null as Deg, t1.LH, NULL as MinAgo "
    //                "from stations t1 left join symbols t2 on t1.sym=t2.sym";

    scene->stationList.clear();

    QSqlQuery query;

    query.prepare( "select t1.Call, t1.Sym, t2.Comment, t1.Lat, t1.Lng, t1.Grid, t1.Via, t1.StComment, null as Length, null as Deg, t1.LH, NULL as MinAgo "
                   "from stations t1 left join symbols t2 on t1.sym=t2.sym" );
    query.exec();
    query.first();
    while (query.next()) {
        //QString value = query.value( 0 ).toString();

        QString picName;
        QByteArray Sym;

             Sym.clear();
             Sym.append( query.value( 1 ).toString() );

             picName = ":images/";

             if (Sym.data()[0] == '/' )
                picName.append( "primary/" ); else
                picName.append( "alternate/" );

             int symindex = (uchar( Sym.data()[1] )-33) ;

             Sym.clear();
             Sym.append( symindex );

             picName.append( Sym.toHex().toUpper() );
             picName.append( ".png" );

        scene->obj = new aprsStation();
        scene->obj->setParams( query.value( 0 ).toString(), query.value( 3 ).toString(), query.value( 4 ).toString(), picName, scene);
        scene->stationList << scene->obj;

    }

    scene->drawMap();



    return 0;

}

void MapsWindowImpl::unloadMap() {

    actionLoad_Map  ->setDisabled( TRUE );
    actionZoomIn    ->setDisabled( TRUE );
    actionZoomOut   ->setDisabled( TRUE );

    delete scene->map;
    scene->map  = NULL;

}

void MapsWindowImpl::showEvent ( QShowEvent * event ) {

    qDebug() << "Maps show";

    if ( getDBVal("mmwin_X").length() > 0 ) {

        this->setGeometry( getDBVal("mmwin_X").toInt() + 4, getDBVal("mmwin_Y").toInt() + 23 /* !!! DURDOM */,
                           getDBVal("mmwin_W").toInt(), getDBVal("mmwin_H").toInt() );
    }

}

void MapsWindowImpl::closeEvent(QCloseEvent *event) {

    hide();

    QSqlQuery query;

    query.prepare( "delete from vars where varname like 'mmwin_%' " );
    query.exec();

    setDBVal( "mmwin_X", QString::number( this->x() ) );
    setDBVal( "mmwin_Y", QString::number( this->y() ) );
    setDBVal( "mmwin_W", QString::number( this->width() ) );
    setDBVal( "mmwin_H", QString::number( this->height() ) );

    event->ignore();

}

void MapsWindowImpl::zoomInClick(){

    scene->map->scale( 1.25, 1.25 );
    scene->drawMap();

}

void MapsWindowImpl::zoomOutClick(){

    scene->map->scale( 0.75, 0.75 );
    scene->drawMap();

}

void MapsWindowImpl::loadMapClick(){

    qDebug() << "Load MAP";

    LoadMapDialog win;

    win.setWindowModality(Qt::ApplicationModal);
    win.scene = scene;
    win.exec();

    setWindowTitle( "qAPRS - [" + scene->map->mapName + "]" );

    scene->drawMap();
}

QString MapsWindowImpl::getDBVal( QString varname ) {

    QSqlQuery query;

    query.prepare( "select varval from vars where varname='" + varname + "'" );
    query.exec();
    query.first();
    QString value = query.value( 0 ).toString();

    return value;

}

void MapsWindowImpl::setDBVal( QString varname, QString varval ) {

    QSqlQuery query;

    query.prepare( "insert into vars(varval, varname) values(:p1, :p2) " );
    query.bindValue(":p1", varval);
    query.bindValue(":p2", varname);
    query.exec();

}
