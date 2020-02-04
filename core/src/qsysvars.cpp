#include "../../core/include/qsysvars.h"

QSysVars::QSysVars( ) {

    first = nullptr;
    last = nullptr;

}

QString QSysVars::getVal( QString varName ) {

    if ( first == nullptr ) {

        return "";

    } else {

        QSysVar *tmp;

        tmp = first;

        while ( tmp != nullptr ) {

            if ( tmp->name == varName ) return tmp->value;
            tmp = tmp->next;

        }

        return "";

    }

}


void QSysVars::setVal( QString varName, QString varValue ) {

    QSysVar *tmp;

    //qDebug() << "QSysDBase::setVal varName=" << varName << " varValue=" << varValue;

    //if ( baseType == 0 ) { //значить данные с sql
    //    DatabaseWriteValThread *DBThread;
    //    DBThread           = new DatabaseWriteValThread::DatabaseWriteValThread();
    //    DBThread->varName  = varName;
    //    DBThread->varVal   = varValue;
    //    DBThread->thn      = this->thn++;

    //    DBThread->start( QThread::HighestPriority );
    //}

    //если записи есть, то ищем, и если находим, то меняем
    if ( first != nullptr ){

        tmp = first;

        while ( tmp != nullptr ) {

            if ( tmp->name == varName ) {
                if ( tmp->value != varValue ) {
                    tmp->value = varValue;
                    tmp->stored = false;
                }
                return;
            }
            tmp = tmp->next;

        }

    }
    //если нет, то добавляем первую запись
    if ( first == nullptr ) {

        tmp = new QSysVar();
        tmp->name = varName;
        tmp->value = varValue;
        tmp->stored = false;

        last = tmp;
        first = tmp;

    } else {
    //или запись в конец списка
        tmp = new QSysVar(); //создаем следующую запись после последней

        tmp->name = varName;
        tmp->value = varValue;
        tmp->stored = false;

        last->next = tmp;
        tmp->prev  = last; //!!! совсем ведь забыл!!! ВЕЗДЕ ПРОВЕРИТЬ!!

        last  = tmp;

    }

}

void QSysVars::delVal( QString varName ) {

    //реализуем немного позже, хотя врядли потребуется удаление системных переменных
    qDebug() << varName+"=null";
    this->setVal( varName, "" );

}
