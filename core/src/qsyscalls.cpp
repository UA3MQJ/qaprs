#include "../../core/include/qsyscalls.h"

QSysCalls::QSysCalls( ) {

    first = NULL;
    last = NULL;

}

//QString QSysCalls::getVal( QString varName ) {
QString QSysCalls::getCall( QString callName ) {

    if ( first == NULL ) {

        return "";

    } else {

        QSysCall *tmp;

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->name == callName ) return tmp->name;
            tmp = tmp->next;

        }

        return "";

    }

}


void QSysCalls::addCall( QString callName ) {

    QSysCall *tmp;

    //если записи есть, то ищем, и если находим, то мен€ем
    if ( first != NULL ){

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->name == callName ) {
                return;
            }
            tmp = tmp->next;

        }

    }
    //если нет, то добавл€ем первую запись
    if ( first == NULL ) {

        tmp = new QSysCall::QSysCall();
        tmp->name = callName;
        tmp->stored = false;

        last = tmp;
        first = tmp;

    } else {
    //или запись в конец списка
        tmp = new QSysCall::QSysCall(); //создаем следующую запись после последней

        tmp->name = callName;
        tmp->stored = false;

        last->next = tmp;
        tmp->prev  = last; //!!! совсем ведь забыл!!! ¬≈«ƒ≈ ѕ–ќ¬≈–»“№!!

        last  = tmp;

    }

}

void QSysCalls::delCall( QString callName ) {

    if ( first != NULL ) {

        if ( first == last) {
            if ( first->name == callName ) {
                delete first;
                first = NULL;
                last  = NULL;

                return;
            }
        }

        QSysCall *tmp;

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->name == callName ) {

                if ( tmp == first) {
                    first = tmp->next;
                    delete tmp;
                    return;
                }

                if ( tmp == last ) {
                    last = tmp->prev;
                    delete tmp;
                    return;
                }

                if (( tmp != last )&&(tmp != first)) {
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                    delete tmp;
                    return;
                }

            }
            tmp = tmp->next;

        }

    }

}
