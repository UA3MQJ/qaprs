#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    qDebug() << "on_startButton_clicked()";

    QDateTime time1, time2;

    time1 = QDateTime::currentDateTime();

    QFile file("E:/Qt/Projects/DSProcessor/Signals/IQ96 LSB 3,683700MHz 15.04.2006 0 58 55.wav");
    QFile file2("E:/Qt/Projects/DSProcessor/Signals/IQ96 LSB 3,683700MHz 15.04.2006 0 58 55.out.wav");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "!file.open";
        return;
    }

    if (!file2.open(QIODevice::WriteOnly)) {
        qDebug() << "!file2.open";
        return;
    }

    QByteArray data_in;
    QByteArray data_out;

    while (!file.atEnd()) {
        data_in = file.readAll();
        qDebug() << "line.length()=" << data_in.length();
    }

    qint64 findx;


    //for(findx=0;findx<data_in.length();findx++) {
    //    data_out.append(data_in.at(findx));
    //}

    //file2.write( data_out );

    qDebug() << "qint8 [0]" << (qint8)(data_in[0]);
    qDebug() << "qint8 [1]" << (qint8)(data_in[1]);
    qint16 *dat_ptr = (qint16*)data_in.data();
    qint16 dat_val;
    WAVE_HEADER *hdr_ptr;
    DATA_CHUNK  *data_ptr;

    dat_val = *dat_ptr;
    hdr_ptr = (WAVE_HEADER*)data_in.data();
    data_ptr = (DATA_CHUNK*)data_in.data();

    //qDebug() << "dat_val =" << dat_val;


    qDebug() << "riff ="             << hdr_ptr->riff ;
    qDebug() << "riffsize ="         << hdr_ptr->riffsize;
    qDebug() << "wavefmt ="          << hdr_ptr->wavefmt;
    qDebug() << "fmtsize ="          << hdr_ptr->fmtsize;
    qDebug() << "wFormatTag ="       << hdr_ptr->wFormatTag;
    qDebug() << "nChannels ="        << hdr_ptr->nChannels;
    qDebug() << "nSamplesPerSec ="   << hdr_ptr->nSamplesPerSec;
    qDebug() << "nAvgBytesPerSec ="  << hdr_ptr->nAvgBytesPerSec;
    qDebug() << "nBlockAlign ="      << hdr_ptr->nBlockAlign;
    qDebug() << "wBitsPerSample ="   << hdr_ptr->wBitsPerSample;
    qDebug() << "data ="             << (char*)hdr_ptr->data;
    qDebug() << "datasize ="         << hdr_ptr->datasize;

    if( strncmp( hdr_ptr->riff, "RIFF", 4) == 0 )
        qDebug() << "riff - OK";

    if( strncmp( hdr_ptr->wavefmt, "WAVEfmt ", 8) == 0 )
        qDebug() << "WAVEfmt - OK";

    if( hdr_ptr->wFormatTag == WAVE_FORMAT_PCM )
        qDebug() << "PCM - OK";

    if( strncmp( (char*)hdr_ptr->data, "data", 4) == 0 )
        qDebug() << "Start data stream after header!";




    file.close();
    file2.close();

    time2 = QDateTime::currentDateTime();

    qDebug() << "all!" << time1.secsTo( time2 );
    qDebug() << "seconds " << time1.secsTo( time2 );
    qDebug() << "start at " << time1.toString("yyyy.MM.dd hh:mm:ss.zzz");
    qDebug() << "end   at " << time2.toString("yyyy.MM.dd hh:mm:ss.zzz");




}
