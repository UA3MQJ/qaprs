#include "../../core/include/aprs.h"

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

	if (( tlat <= 1 )&&( tlng <= 1 )&&( tlat >= -1 )&&( tlng >= -1 )) {

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

bool isDigi( QString call ) {

    if ( call.right( 1 ) == "*" ) return true;

    if ( ( call.left( 4 ).toUpper() == "WIDE" ) && ( call.length() >= 7 ) ) {
        //значит это какой-то из WIDE
        //wide14-14
        //wide14-1
        //wide1-1
        int n1 = 0, n2 = 0;
        n1 = call.split( '-' ).at( 0 ).mid( 4 ).toInt();
        n2 = call.split( '-' ).at( 1 ).toInt();

        //qDebug() << "wide" << n1 << "-" << n2;
        if ( n1 != n2 ) return true;

    }

    if ( ( call.left( 5 ).toUpper() == "TRACE" ) && ( call.length() >= 8 ) ) {
        //значит это какой-то из WIDE
        //TRACE14-14
        //TRACE14-1
        //TRACE1-1
        int n1 = 0, n2 = 0;
        n1 = call.split( '-' ).at( 0 ).mid( 4 ).toInt();
        n2 = call.split( '-' ).at( 1 ).toInt();

        //qDebug() << "TRACE" << n1 << "-" << n2;
        if ( n1 != n2 ) return true;

    }


    return false;
}

int  portTypeIdFromName( QString portType ) {

    if ( portType.toUpper()=="INET" ) {
        return 0;
    } else
    if ( portType.toUpper()=="KISS" ) {
        return 1;
    } else
    if ( portType.toUpper()=="AGW" ) {
        return 2;
    } else
    if ( portType.toUpper()=="AXIP" ) {
        return 5;
    } else
    if ( portType.toUpper()=="FLDIGI" ) {
        return 6;
    } else
    if ( portType.toUpper()=="VASKA" ) {
        return 7;
    }

    return -1;

}

int  parIdFromName( int portTypeId, QString paramName ) {

    if ( portTypeId==0 ) { //inet
        if ( paramName=="Server1" ) {
            return 1;
        } else
        if ( paramName=="Server2" ) {
            return 2;
        } else
        if ( paramName=="Server3" ) {
            return 3;
        } else
        if ( paramName=="User" ) {
            return 4;
        } else
        if ( paramName=="Pass" ) {
            return 5;
        } else
        if ( paramName=="Filter" ) {
            return 6;
        } else
        if ( paramName=="Proxy" ) {
            return 7;
        } else
        if ( paramName=="PAuth" ) {
            return 8;
        } else
        if ( paramName=="PName" ) {
            return 9;
        } else
        if ( paramName=="PPort" ) {
            return 10;
        } else
        if ( paramName=="PUser" ) {
            return 11;
        } else
        if ( paramName=="PPass" ) {
            return 12;
        }
    } else
    if ( portTypeId==1 ) { //kiss
        if ( paramName=="COM" ) {
            return 1;
        } else
        if ( paramName=="Speed" ) {
            return 2;
        } else
        if ( paramName=="Command1" ) {
            return 3;
        } else
        if ( paramName=="Command2" ) {
            return 4;
        } else
        if ( paramName=="Command3" ) {
            return 5;
        } else
        if ( paramName=="Command4" ) {
            return 6;
        } else
        if ( paramName=="TXDelay" ) {
            return 7;
        } else
        if ( paramName=="TXTail" ) {
            return 8;
        } else
        if ( paramName=="SlotTime" ) {
            return 9;
        } else
        if ( paramName=="Persist" ) {
            return 10;
        } else
        if ( paramName=="FullDuplex" ) {
            return 11;
        } else
        if ( paramName=="ExitFromKISS" ) {
            return 12;
        }
    } else
    if ( portTypeId==2 ) { //agw
        if ( paramName=="Host" ) {
            return 1;
        } else
        if ( paramName=="AGWPort" ) {
            return 2;
        }
    } else
    if ( portTypeId==5 ) { //axip
        if ( paramName=="RXPort" ) {
            return 1;
        } else
        if ( paramName=="TXPort" ) {
            return 2;
        } else
        if ( paramName=="IPAddress" ) {
            return 3;
        }
    } else
    if ( portTypeId==6 ) { //fldigi
        if ( paramName=="Host" ) {
            return 1;
        } else
        if ( paramName=="Port" ) {
            return 2;
        }
    } else
    if ( portTypeId==7 ) { //vaska
        if ( paramName=="Host" ) {
            return 1;
        } else
        if ( paramName=="Port" ) {
            return 2;
        } else
        if ( paramName=="Mode" ) {
            return 3;
        }
    }

    return -1;

}

int  parIdFromName( QString portType, QString paramName ) {

    return parIdFromName( portTypeIdFromName( portType ), paramName );

}

QString parNameFromId( int portTypeId, int parId ) {

    if ( portTypeId==0 ) { //inet
        if ( parId==1 ) {
            return "Server1";
        } else
        if ( parId==2 ) {
            return "Server2";
        } else
        if ( parId==3 ) {
            return "Server3";
        } else
        if ( parId==4 ) {
            return "User";
        } else
        if ( parId==5 ) {
            return "Pass";
        } else
        if ( parId==6 ) {
            return "Filter";
        } else
        if ( parId==7 ) {
            return "Proxy";
        } else
        if ( parId==8 ) {
            return "PAuth";
        } else
        if ( parId==9 ) {
            return "PName";
        } else
        if ( parId==10 ) {
            return "PPort";
        } else
        if ( parId==11 ) {
            return "PUser";
        } else
        if ( parId==12 ) {
            return "PPass";
        }
    } else
    if ( portTypeId==1 ) { //kiss
        if ( parId==1 ) {
            return "COM";
        } else
        if ( parId==2 ) {
            return "Speed";
        } else
        if ( parId==3 ) {
            return "Command1";
        } else
        if ( parId==4 ) {
            return "Command2";
        } else
        if ( parId==5 ) {
            return "Command3";
        } else
        if ( parId==6 ) {
            return "Command4";
        } else
        if ( parId==7 ) {
            return "TXDelay";
        } else
        if ( parId==8 ) {
            return "TXTail";
        } else
        if ( parId==9 ) {
            return "SlotTime";
        } else
        if ( parId==10 ) {
            return "Persist";
        } else
        if ( parId==11 ) {
            return "FullDuplex";
        } else
        if ( parId==12 ) {
            return "ExitFromKISS";
        }
    } else
    if ( portTypeId==2 ) { //agw
        if ( parId==1 ) {
            return "Host";
        } else
        if ( parId==2 ) {
            return "AGWPort";
        }
    } else
    if ( portTypeId==5 ) { //axip
        if ( parId==1 ) {
            return "RXPort";
        } else
        if ( parId==2 ) {
            return "TXPort";
        } else
        if ( parId==3 ) {
            return "IPAddress";
        }
    } else
    if ( portTypeId==6 ) { //fldigi
        if ( parId==1 ) {
            return "Host";
        } else
        if ( parId==2 ) {
            return "Port";
        }
    } else
    if ( portTypeId==7 ) { //vaska
        if ( parId==1 ) {
            return "Host";
        } else
        if ( parId==2 ) {
            return "Port";
        } else
        if ( parId==3 ) {
            return "Mode";
        }
    }

    return "";

}

QString trPar( QString parVal ) {
//функция вставляет в значении параметра \перед "
    QString ts = parVal;

    ts = ts.replace( "\\", "\\\\" );
    ts = ts.replace( "\"", "\\\"" );

    return ts;
}
