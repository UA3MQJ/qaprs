#include "../../core/include/qsysvars.h"

QSysVars::QSysVars( ) {

    first = NULL;
    last = NULL;

}

QString QSysVars::getVal( QString varName ) {

    if ( first == NULL ) {

        return "";

    } else {

        QSysVar *tmp;

        tmp = first;

        while ( tmp != NULL ) {

            if ( tmp->name == varName ) return tmp->value;
            tmp = tmp->next;

        }

        return "";

    }

}


void QSysVars::setVal( QString varName, QString varValue ) {

    QSysVar *tmp;

    //qDebug() << "QSysDBase::setVal varName=" << varName << " varValue=" << varValue;

    //if ( baseType == 0 ) { //������� ������ � sql
    //    DatabaseWriteValThread *DBThread;
    //    DBThread           = new DatabaseWriteValThread::DatabaseWriteValThread();
    //    DBThread->varName  = varName;
    //    DBThread->varVal   = varValue;
    //    DBThread->thn      = this->thn++;

    //    DBThread->start( QThread::HighestPriority );
    //}

    //���� ������ ����, �� ����, � ���� �������, �� ������
    if ( first != NULL ){

        tmp = first;

        while ( tmp != NULL ) {

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
    //���� ���, �� ��������� ������ ������
    if ( first == NULL ) {

        tmp = new QSysVar::QSysVar();
        tmp->name = varName;
        tmp->value = varValue;
        tmp->stored = false;

        last = tmp;
        first = tmp;

    } else {
    //��� ������ � ����� ������
        tmp = new QSysVar::QSysVar(); //������� ��������� ������ ����� ���������

        tmp->name = varName;
        tmp->value = varValue;
        tmp->stored = false;

        last->next = tmp;
        tmp->prev  = last; //!!! ������ ���� �����!!! ����� ���������!!

        last  = tmp;

    }

}

void QSysVars::delVal( QString varName ) {

    //��������� ������� �����, ���� ������ ����������� �������� ��������� ����������
    qDebug() << varName+"=null";
    this->setVal( varName, "" );

}
