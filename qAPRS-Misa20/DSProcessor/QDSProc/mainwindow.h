#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

// ����������� ���� WAVE_HEADER
// ��� ���������� ���������, ������� ����� ���� ����� �������, �� �� �����
// ��� ��� ������ � �������� ���������

int const WAVE_FORMAT_PCM = 1;

typedef struct
{
    /*  ������������ RIFF  */

    //byte quint8
    //word quint16
    //dword quint32
    //SHORT quint16

    char     riff[4];         //BYTE       // ��� ������ ���� ����� RIFF
    quint32    riffsize;         //DWORD      // ������ ����� - 8

    /* ������������ FORMAT, ��������� ������ WAV �����  */
    char       wavefmt[8];        //char       // ��� ������ ���� ����� "WAVEfmt "
    quint32    fmtsize;          //DWORD      // ������ ������������ FORMAT - ����� ���� ������ 16 ��� 18
                               // ��� ������������ ��������� WAV ���������� �������� 16
    quint16    wFormatTag;     //WORD       // ������ ����� , ��� ���������� ������ WAVE_FORMAT_PCM ( == 1 )
    quint16    nChannels;      //WORD       // ����� ������� ( 1 ��� 2 )
    quint32    nSamplesPerSec;   //DWORD      // ����� �������� � ������� ( 8000, 11025, 22050 ��� 44100 )
    quint32    nAvgBytesPerSec;  //DWORD      // ����� ���� � �������.
    quint16    nBlockAlign;       //WORD       // ������������ ����� ������
    quint16      wBitsPerSample;   //       // ����� ��� �� ������ ( 8 ��� 16 )
    /*  DATA chunk  ( chunk == ����� ) */
    quint8     data[4];           //BYTE       // ��� ������ ���� ����� "data"
    quint32    datasize;         //DWORD      // ������ ������ � ������
                                /* ����� � ����� ���� ������, ������ ������� == datasize  */
    /*  � ���������, ����� ����������� � ������ �����, �� ������ data
        �� ������ ������ ����� data ������ ���� fact ����, ������� �������� ���������� � ������ � ����
        �������� �����. ����� ����� ��� ���� ����� ���������� */
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
