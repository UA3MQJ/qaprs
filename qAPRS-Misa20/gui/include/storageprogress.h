#ifndef __STORAGEPROGRESS_H__
#define __STORAGEPROGRESS_H__

#include "ui_storageprogress.h"
#include <QObject>
#include <QDialog>
#include <QDebug>
#include <QSplashScreen>


class StorageProgressDialog : public QDialog, public Ui::DialogStorageProgress
{

    Q_OBJECT

public:

                StorageProgressDialog();
                ~StorageProgressDialog();

signals:

private slots:
                void storeProgress( QString msg );

};



#endif // __STORAGEPROGRESS_H__
