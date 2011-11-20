#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

// определение типа WAVE_HEADER
// Это простейший заголовок, реально может быть более сложным, но мы учтем
// это при чтении и проверке заголовка

int const WAVE_FORMAT_PCM = 1;

typedef struct
{
    /*  подзаголовок RIFF  */

    //byte quint8
    //word quint16
    //dword quint32
    //SHORT quint16

    char     riff[4];         //BYTE       // тут должны бфть буквы RIFF
    quint32    riffsize;         //DWORD      // размер файла - 8

    /* подзаголовок FORMAT, описывает формат WAV файла  */
    char       wavefmt[8];        //char       // тут должны быть буквы "WAVEfmt "
    quint32    fmtsize;          //DWORD      // размер подзаголовка FORMAT - может быть только 16 или 18
                               // для минимального заголовка WAV достаточно значения 16
    quint16    wFormatTag;     //WORD       // формат файла , нас интересует только WAVE_FORMAT_PCM ( == 1 )
    quint16    nChannels;      //WORD       // число каналов ( 1 или 2 )
    quint32    nSamplesPerSec;   //DWORD      // число отсчетов в секунду ( 8000, 11025, 22050 или 44100 )
    quint32    nAvgBytesPerSec;  //DWORD      // число байт в секунду.
    quint16    nBlockAlign;       //WORD       // выравнивание блока данных
    quint16      wBitsPerSample;   //       // число бит на отсчет ( 8 или 16 )
    /*  DATA chunk  ( chunk == кусок ) */
    quint8     data[4];           //BYTE       // тут должны быть буквы "data"
    quint32    datasize;         //DWORD      // размер данных в байтах
                                /* далее в файле идут данные, размер которых == datasize  */
    /*  к сожалению, могут встречаться и другие чанки, не только data
        во многих файлах перед data чанком идет fact чанк, который содержит информацию о кодере и дате
        создания файла. Такие куски нам надо будет пропустить */
} WAVE_HEADER;

typedef struct
{
    char    data[4];
    quint32   datasize;
} DATA_CHUNK;


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
