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

    //���� ������ ����, �� ����, � ���� �������, �� ������
    if ( first != NULL ){

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->name == callName ) {
                return;
            }
            tmp = tmp->next;

        }

    }
    //���� ���, �� ��������� ������ ������
    if ( first == NULL ) {

        tmp = new QSysCall::QSysCall();
        tmp->name = callName;
        tmp->stored = false;

        last = tmp;
        first = tmp;

    } else {
    //��� ������ � ����� ������
        tmp = new QSysCall::QSysCall(); //������� ��������� ������ ����� ���������

        tmp->name = callName;
        tmp->stored = false;

        last->next = tmp;
        tmp->prev  = last; //!!! ������ ���� �����!!! ����� ���������!!

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
