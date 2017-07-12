#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->horizontalScrollBar->setRange(-500, 500);
    //ui->verticalScrollBar->setRange(-500, 500);

    readSerialPortInfos();
    connect(ui->btnRefresh, SIGNAL (clicked()), this, SLOT (readSerialPortInfos()));

    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::DataBits::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(serial, SIGNAL(readyRead()),this,SLOT(dataReady()));

    ui->customPlot->addGraph(); // blue line
    QPen bluePen;
    bluePen.setColor(QColor(40, 110, 255));
    bluePen.setStyle(Qt::SolidLine);
    bluePen.setWidthF(4);
    ui->customPlot->graph(0)->setPen(bluePen);
    ui->customPlot->xAxis->setLabel("Operating Time (hh:mm:ss)");
    ui->customPlot->yAxis->setLabel("Temp (°C)");
    ui->customPlot->graph(0)->setName("Sensor Temperature (°C)");
    //ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.5, 1.5);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis, SLOT(setRange(QCPRange)));
    //connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    //connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    //connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    //connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataReady() {
    _dataReady = true;
}

void MainWindow::update()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if(isConnect && _dataReady) {
        _dataReady = false;
        QByteArray data = serial->readAll();
        data = data.remove(8, data.length());

        if (data.length() == 8 && data.toFloat()) {
            ui->lblTemp->setText(QString("%1 °C").arg((float) data.toLong() * 0.0625f));

            if (key-lastPointKey > 0.002) // at most add point every 2 ms
            {
              // add data to lines:
              ui->customPlot->graph(0)->addData(key, (float) data.toLong() * 0.0625f);
              lastPointKey = key;
            }
            // make key axis range scroll with the data (at a constant range size of 8):
            if (ui->customPlot->graph(0)->data()->size() <= 300)
            {
                ui->customPlot->xAxis->rescale();
            } else if (ui->customPlot->graph(0)->data()->size() > 300) {
                ui->customPlot->xAxis->setRange(key, 30, Qt::AlignRight);
            }
            //ui->customPlot->yAxis->setRange(20.0, 40.0);
            ui->customPlot->yAxis->setRange((float) data.toLong() * 0.0625f, 10, Qt::AlignTop);
            //ui->customPlot->yAxis->rescale();
            //if (!isScroll)
                ui->customPlot->replot();

            // calculate frames per second:
            static double lastFpsKey;
            static int frameCount;
            ++frameCount;
            if (key-lastFpsKey > 2) // average fps over 2 seconds
            {
                ui->statusBar->showMessage(
                    QString("%1 FPS, Total Data points: %2")
                    .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                    .arg(ui->customPlot->graph(0)->data()->size())
                    , 0);
                lastFpsKey = key;
                frameCount = 0;
            }
        }
        //prevdata = data;
    }
}

void MainWindow::on_btnConnect_clicked()
{
    if(!isConnect) {
        serial->setPortName(QString(ui->cbPort->currentText()));

        serial->open(QIODevice::ReadWrite);
        ui->btnConnect->setText("Disconnect");
        isConnect = true;
    } else {
        serial->close();
        ui->btnConnect->setText("Connect");
        isConnect = false;
    }
}

void MainWindow::readSerialPortInfos() {
    QSerialPortInfo::availablePorts().clear();
    const auto infos = QSerialPortInfo::availablePorts();
    ui->cbPort->clear();
    for (const QSerialPortInfo &info : infos) {
        QString s = info.portName();
        ui->cbPort->addItem(s);
    }
}

//void MainWindow::horzScrollBarChanged(int value)
//{
//  if (qAbs(ui->customPlot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//  {
//    isScroll = true;
//    ui->customPlot->xAxis->setRange(value/100.0, ui->customPlot->xAxis->range().size(), Qt::AlignCenter);
//    ui->customPlot->replot();
//  } else ui->customPlot->replot();
//}

//void MainWindow::vertScrollBarChanged(int value)
//{
//  if (qAbs(ui->customPlot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//  {
//    isScroll = true;
//    ui->customPlot->yAxis->setRange(-value/100.0, ui->customPlot->yAxis->range().size(), Qt::AlignCenter);
//    ui->customPlot->replot();
//  } else isScroll = false;
//}

//void MainWindow::xAxisChanged(QCPRange range)
//{
//  ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
//  ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
//}

//void MainWindow::yAxisChanged(QCPRange range)
//{
//  ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
//  ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
//}
