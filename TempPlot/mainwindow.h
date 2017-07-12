#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDataStream>
#include "qcustomplot.h"
#include "newdevicedialog.h"

namespace Ui {
class MainWindow;
}

class NewDeviceDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateData2Server(float data);
    void onServerReply(QNetworkReply* rep);

private slots:
    void on_btnConnect_clicked();
    void update();
    void readSerialPortInfos();
//    void dataReady();
//    void horzScrollBarChanged(int value);
//    void vertScrollBarChanged(int value);
//    void xAxisChanged(QCPRange range);
//    void yAxisChanged(QCPRange range);

    void on_actionNew_Device_triggered();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager* networkManager;
    QSerialPort *serial = new QSerialPort();
    QByteArray data;
    NewDeviceDialog * newDeviceDiag;
//    bool isConnect = false;
//    bool isScroll = false;
//    bool _dataReady = false;
};

#endif // MAINWINDOW_H
