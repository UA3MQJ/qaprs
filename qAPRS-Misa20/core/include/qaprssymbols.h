#ifndef QAPRSSYMBOLS_H
#define QAPRSSYMBOLS_H

#include <QtCore>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>

class QAPRSSymbols;
class QAPRSSymbol;

class QAPRSSymbol {
public:
    QAPRSSymbol( ) { next = NULL; prev = NULL; parent = NULL; };
    QString code();
    QString name();
    QString img();
    QString digisym;
    void setCode( QString val ) { Scode = val; };
    void setName( QString val ) { Sname = val; };
    void setImg( QString val )  { Simg = val; };
    QAPRSSymbol *next;
    QAPRSSymbol *prev;
    QAPRSSymbols *parent;
private:
    QString Scode;
    QString Sname;
    QString Simg;
};


class QAPRSSymbols {
public:
    QAPRSSymbols( );
    QAPRSSymbol *first;
    QAPRSSymbol *last;
    QAPRSSymbol  getSym( QString varCode );
    QString      getSymName( QString varCode );
    QString      getSymImg( QString varCode );
    void         setSym( QString varCode, QString varName, QString varImg );
    void         exch( QAPRSSymbol *s1 );
    void         sort();
};

#endif // QAPRSSYMBOLS_H
