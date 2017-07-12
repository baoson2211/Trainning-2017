#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "qcustomplot.h"

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
    void on_btnConnect_clicked();
    void update();
    void dataReady();
    void readSerialPortInfos();
//    void horzScrollBarChanged(int value);
//    void vertScrollBarChanged(int value);
//    void xAxisChanged(QCPRange range);
//    void yAxisChanged(QCPRange range);

private:
    Ui::MainWindow *ui;

    QSerialPort *serial = new QSerialPort();
    bool isConnect = false;
//    bool isScroll = false;
    bool _dataReady = false;
};

#endif // MAINWINDOW_H
