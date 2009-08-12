#include "aprs.h"

#define EARTH_RADIUS_METERS     6378138.0
#define EARTH_RADIUS_KILOMETERS 6378.138
#define EARTH_RADIUS_MILES      3963.1836

QString real2lat(qreal lat) {
	qreal tlat;
	QString res, P;
	tlat = lat;

	if ( tlat >= 0 ) P = "S"; else {
		P = "N";
		tlat = -tlat;	}

	tlat = tlat * 90;
	res = QString::number( int( tlat ) );
	while ( res.length() < 2 ) res = "0" + res;
	tlat = tlat - int(tlat);

	tlat = tlat * 60;
	res += ( int( tlat ) < 10 )?( ".0" + QString::number( int( tlat ) ) ):( "." + QString::number( int( tlat ) ) );
	tlat = tlat - int( tlat );

	tlat = tlat * 100;
	res += ( int( tlat ) < 10 )?( ".0" + QString::number( int( tlat ) ) ):( "." + QString::number( int( tlat ) ) );

	return res + P;
}

QString real2long(qreal lng) {
	qreal tlng;
	QString res, P;
	tlng = lng;

	if ( tlng >= 0 ) P = "E"; else {
		P = "W";
		tlng = -tlng;	}

	tlng = tlng * 180;
	res = QString::number( int( tlng ) );
	while ( res.length() < 3 ) res = "0" + res;
	tlng = tlng - int( tlng );

	tlng = tlng * 60;
	res += ( int( tlng ) < 10 )?( ".0" + QString::number( int( tlng ) ) ):( "." + QString::number( int( tlng ) ) );
	tlng = tlng - int( tlng );

	tlng = tlng * 100;
	res += ( int( tlng ) < 10 )?( ".0" + QString::number( int( tlng ) ) ):( "." + QString::number( int( tlng ) ) );

	return res + P;
}

qreal lat2real(QString lat) {

	QString ts = "", P;
	qreal a, b, c, res;

	ts.append( lat ) ;

	a = ( ts.section( ".", 0, 0 ) ).toInt() ;
	b = ( ts.section( ".", 1, 1 ) ).toInt() ;
	P = ts.right( 1 );
	ts.remove( ts.length() - 1, 1 );
	c = ( ts.section( ".", 2, 2 ) ).toInt() ;

	res = c / 100;
	res = ( res + b ) / 60;
	res = ( res + a ) / 90;

	if ( P=="N" ) res = -res;

	return res;
}

qreal long2real(QString lng) {

	QString ts = "", P;
	qreal a, b, c, res;

	ts.append( lng ) ;

	a = ( ts.section( ".", 0, 0 ) ).toInt() ;
	b = ( ts.section( ".", 1, 1 ) ).toInt() ;
	P = ts.right( 1 );
	ts.remove( ts.length() - 1, 1 );
	c = ( ts.section( ".", 2, 2 ) ).toInt() ;

	res = c / 100;
	res = ( res + b ) / 60;
	res = ( res + a ) / 180;

	if ( P=="W" ) res = -res;

	return res;
}


QString real2qthloc(qreal lat, qreal lng) {
	qreal tlat, tlng;
	QString res="", P;

	tlat = lat; tlng = lng;

	if (( tlat <= 1 )and( tlng <= 1 )and( tlat >= -1 )and( tlng >= -1 )) {

		tlat = (lat + 1); tlng = (lng + 1);

					tlng =  tlng * 9;
					res  =  res + char( 65 + int( tlng ) );
					tlng -= int( tlng );
		//               'A'

		tlat =  tlat * 9;
		res  =  res + char( 82 -int( tlat ) );
		tlat -= int( tlat );
		//               'R'


					tlng =  tlng * 10;
					res  =  res + char( 48 + int( tlng ) );
					tlng -= int( tlng );
		//               '0'

		tlat =  tlat * 10;
		res  =  res + char( 48 + 9 - int( tlat ) );
		tlat -= int( tlat );
		//               '9'

					tlng =  tlng * 24;
					res  =  res + char( 65 + int( tlng ) );

		tlat =  tlat * 24;
		res  =  res + char( 65 + 23 -int( tlat ) );

	}
	return res;
}

qreal calc_distance_law_of_cosines(qreal lat1, qreal lon1, qreal lat2, qreal lon2) {

    qreal d, r_lat1, r_lat2, r_lon1, r_lon2;

    r_lat1 = ( lat1 ) * ( M_PI / 2 );
    r_lat2 = ( lat2 ) * ( M_PI / 2 );

    r_lon1 = ( lon1 ) * M_PI;
    r_lon2 = ( lon2 ) * M_PI;

    d = acos(sin(r_lat1) * sin(r_lat2) + cos(r_lat1) * cos(r_lat2) * cos(r_lon2-r_lon1));

    return(d*180*60/M_PI*1852);

}

qreal calc_angle(qreal lat1, qreal lon1, qreal lat2, qreal lon2) {

    qreal r_lat1, r_lat2, r_lon1, r_lon2, r_c, r_m, r_d;

    r_lat1 = ( -lat1 ) * ( M_PI / 2 );
    r_lat2 = ( -lat2 ) * ( M_PI / 2 );

    r_lon1 = ( lon1 ) * M_PI;
    r_lon2 = ( lon2 ) * M_PI;

    r_m = acos(sin(r_lat1) * sin(r_lat2) + cos(r_lat1) * cos(r_lat2) * cos(r_lon2-r_lon1));
    r_d = r_m ;

    if ( r_lon1 == r_lon2 ) {
        if ((r_lat2 - r_lat1)>0.0)
            r_c=M_PI;
        else
            r_c=0.0;
    } else {
            if (sin((r_lon2-r_lon1))>0.0)
                r_c =            acos((sin(r_lat2)-sin(r_lat1)*cos(r_d))/(sin(r_d)*cos(r_lat1)));
            else
                r_c = (2*M_PI) - acos((sin(r_lat2)-sin(r_lat1)*cos(r_d))/(sin(r_d)*cos(r_lat1)));
    }

    //return(r_c);
    return(r_c/M_PI*180);

}
