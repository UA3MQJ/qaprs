#include "../../core/include/qaprssymbols.h"

QString QAPRSSymbol::code() {

    return Scode;


};

QString QAPRSSymbol::name() {

    if ( ( digisym != "" )&&
         ( not( ( digisym.mid( 0, 1 ) == "\\" ) || ( digisym.mid( 0, 1 ) == "/" ) ) ) ){

        return "(" + digisym.mid( 0, 1 ) + ") " + Sname;

    }


    return Sname;

};

QString QAPRSSymbol::img() {

/*
    if ( ( Scode.mid( 1, 1 ) == "#" ) &&
         ( ( Scode.mid( 0, 1 ) != "/" ) || ( Scode.mid( 0, 1 ) != "\\" ) )  ) {

        //это особый такой DIGI с символом в звездочке
        if ( parent != NULL ) {

            //return parent->getSym( "\\#" ).img();
            return Simg;

        }

        return Simg;

    }

    return Simg; */
/*
    if ( ( digisym != "" )&&
         ( not( ( digisym.mid( 0, 1 ) == "\\" ) || ( digisym.mid( 0, 1 ) == "/" ) ) ) ){

        QString timg;
        timg = parent->getSym( "/#" ).img();

        return "";

    } */


    return Simg;


};

QAPRSSymbols::QAPRSSymbols(){

    first = NULL;
    last = NULL;

    //грузим символы в массивы
    QDomDocument doc("symbols");
    QFile fileConfig("etc/symbols.xml");
    if (!fileConfig.open(QIODevice::ReadOnly)) {
        //return 1;
        qDebug() << "symbols.xml open error";
    }
    if (!doc.setContent(&fileConfig)) {
        //return 1;
        qDebug() << "symbols.xml set content error";
    }
    fileConfig.close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {

            //symNames->insert( e.attributes().namedItem("code").nodeValue(), e.attributes().namedItem("name").nodeValue() );
            //symImgs->insert( e.attributes().namedItem("code").nodeValue(), e.attributes().namedItem("img").nodeValue() );
            //qDebug() << e.attributes().namedItem("code").nodeValue() << e.attributes().namedItem("name").nodeValue() << e.attributes().namedItem("img").nodeValue();

            this->setSym( e.attributes().namedItem("code").nodeValue(), e.attributes().namedItem("name").nodeValue(), e.attributes().namedItem("img").nodeValue() );
        }
        n = n.nextSibling();
    }

    //this->sort();

}

QAPRSSymbol QAPRSSymbols::getSym( QString varCode ) {

    QAPRSSymbol *nullres;
    QString digSym;
    QString tVarCode = varCode;

    nullres = NULL;

    digSym = "";

    //qDebug() << "getsym << " << tVarCode;

    if ( ( tVarCode.mid( 1, 1 ) == "#" ) &&
         ( ( tVarCode.mid( 0, 1 ) != "/" ) || ( tVarCode.mid( 0, 1 ) != "\\" ) )  ) {

        digSym = tVarCode;
        tVarCode = "/#";

    }

    if ( first == NULL ) {

        qDebug() << "getsym " << varCode << "is NULL";
        return *nullres;

    } else {

        QAPRSSymbol *tmp;

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->code() == tVarCode ) {

                tmp->digisym = digSym;
                return *tmp;
            }
            tmp = tmp->next;

        }

        qDebug() << "getsym " << varCode << "is NULL";
        return *nullres;

    }

}

QString QAPRSSymbols::getSymName( QString varCode ) {

    if ( ( varCode.mid( 1, 1 ) == "#" ) &&
         !( ( varCode.mid( 0, 1 ) == "/" ) || ( varCode.mid( 0, 1 ) == "\\" ) )  ) {

        return "(" + varCode.mid( 0, 1 ) + ") Digi";

    } else {

        if ( first == NULL ) {

            return "";

        } else {

            QAPRSSymbol *tmp;

            tmp = first;

            while ( tmp != NULL ) {

                if ( tmp->code() == varCode ) {

                    return tmp->name();
                }
                tmp = tmp->next;

            }

            return "";

        }

    }

    return "";

}

QString QAPRSSymbols::getSymImg( QString varCode ) {

    if ( first == NULL ) {

        return "";

    } else {

        QAPRSSymbol *tmp;

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->code() == varCode ) {

                return tmp->img();
            }
            tmp = tmp->next;

        }

        return "";

    }

    return "";

}

void QAPRSSymbols::setSym( QString varCode, QString varName, QString varImg ) {

    QAPRSSymbol *tmp;

    //если записи есть, то ищем, и если находим, то мен€ем
    if ( first != NULL ){

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->code() == varCode ) {

                tmp->setName( varName );

                tmp->setImg( varImg );

                return;
            }
            tmp = tmp->next;

        }

    }
    //если нет, то добавл€ем первую запись
    if ( first == NULL ) {

        tmp = new QAPRSSymbol::QAPRSSymbol();
        tmp->setCode( varCode );
        tmp->setName( varName );
        tmp->setImg( varImg );
        tmp->parent = this;

        last = tmp;
        first = tmp;

    } else {
    //или запись в конец списка
        tmp = new QAPRSSymbol::QAPRSSymbol(); //создаем следующую запись после последней

        tmp->setCode( varCode );
        tmp->setName( varName );
        tmp->setImg( varImg );
        tmp->parent = this;

        last->next = tmp;
        tmp->prev  = last; //!!! совсем ведь забыл!!! ¬≈«ƒ≈ ѕ–ќ¬≈–»“№!!!

        last  = tmp;

    }


}

void QAPRSSymbols::exch( QAPRSSymbol *s1 ) {
//помен€ть два символа в цепочке

    QAPRSSymbol *t1; //первый
    QAPRSSymbol *t2; //второй


    QAPRSSymbol *prev_t1; //предыдущий дл€ t1
    QAPRSSymbol *next_t2; //последующий после t2


    //не буду выпендриватьс€
    t1 = s1;
    t2 = s1->next;

    if ((t1->code()=="\\~")&&(t2->code()=="/~")) {

        qDebug() << "t1=" << t1->code();
        qDebug() << "t2=" << t2->code();

    }

    prev_t1 = t1->prev;
    next_t2 = t2->next;

    // теперь, когда все 4 звена известны
    // перестраиваю пр€мую и обратную цепочку


    if ( prev_t1!=NULL ) {
        prev_t1->next = t2;
        //t2->prev = prev_t1;
    } else {
        //если предыдущее NULL значит t1 был первым в цепочке
        //следовательно надо подправить first
        this->first = t2;
    }

    if ( next_t2!=NULL ) {
        next_t2->prev = t1;
        //t1->next = next_t2;
    } else {
        //если последующее значение NULL значит t2 был последним в цепочке
        //следовательно надо подправить last
        this->first = t2;
    }

    t2->next = t1;
    t2->prev = prev_t1;

    t1->next = next_t2;
    t1->prev = t2;


}

void QAPRSSymbols::sort() {
//сортировка пузырьком. ну а что такого?

    QAPRSSymbol *tmp; //временный

    bool flag;

    int  count=0;

    flag = true;

    while ( flag == true ) {

        flag = false;

        tmp = this->first;

        while ( tmp != NULL ) {

            if ( tmp != NULL ) {
                if ( tmp->next != NULL ) {
                    if ( tmp->code().right(1) > tmp->next->code().right(1) ) {

                        //qDebug() << "sort exchange" << tmp->code() << tmp->next->code() << "?";
                        //qDebug() << "YES!!! need!!!";
                        this->exch( tmp );
                        count++;
                        flag = true; //произошел хот€бы один обмен

                    }
                }
            }

            if ( tmp != NULL ) tmp = tmp->next;
        }

    }

    qDebug() << "Sortirovka!!! " << count << "perestanovok!!!";

}
