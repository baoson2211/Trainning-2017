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
    serial->setParity(QSerialPort::Parity::NoParity);
    serial->setStopBits(QSerialPort::StopBits::OneStop);
    serial->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    connect(serial, SIGNAL(readyRead()),this,SLOT(update()));

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

    //QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    //timer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if(serial->isOpen()) {
        data += serial->readAll();
        if (data[0] == '*') {
            if (data[data.length()-1] == '#') {
                qDebug() << data << data.length() << data[0] << data[data.length()-1];
                data = data.remove(0, 1); data = data.remove(data.length()-1, 1);

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
                this->updateData2Server((float) data.toLong() * 0.0625f);
                data.clear();
            }
        }
        else data.clear();
    }
}

void MainWindow::on_btnConnect_clicked()
{
    if(!serial->isOpen()) {
        serial->setPortName(QString(ui->cbPort->currentText()));

        serial->open(QIODevice::ReadWrite);
        ui->btnConnect->setText("Disconnect");
        //isConnect = true;
    } else {
        serial->close();
        ui->btnConnect->setText("Connect");
        //isConnect = false;
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

void MainWindow::updateData2Server(float data) {
    networkManager = new QNetworkAccessManager();
    QString temp = QString::number(data);
/* For POST & PUT Methods */
    QByteArray putData;
    putData.append("data=");
    putData.append(temp);
    // putData.append("&param2=string2");
    QUrl url = QUrl("http://localhost:3000/api/data/" + ui->letDevName->text());
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), SLOT(onServerReply(QNetworkReply*)));
    QNetworkRequest networkRequest(url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    networkManager->put(networkRequest, putData);
}

void MainWindow::onServerReply(QNetworkReply* reply) {
    // check for error
    if ( reply->error() )
    {
    QString form("Download of %1 failed: %2\n");
        ui->statusBar->showMessage( form.arg(reply->url().toEncoded().constData()).arg(qPrintable(reply->errorString())) );
    } else {
    // get http response code
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant contentLengthV = reply->header(QNetworkRequest::ContentLengthHeader);
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        if ( statusCodeV.toInt() == 302 )
        {
            reply->deleteLater();
            reply = nullptr;

            QNetworkRequest request( redirectionTargetUrl.toUrl() );
            reply = networkManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
            return;
        }

        qDebug() << statusCodeV.toString();
        qDebug() << contentLengthV.toString();
        qDebug() << reply->readAll();
    }

    reply->deleteLater();
    reply = NULL;
}

void MainWindow::on_actionNew_Device_triggered()
{    
    // newDeviceDiag = new NewDeviceDialog();
    newDeviceDiag = new NewDeviceDialog(this); // must be closed this dialog before wanna close mainwindow
    newDeviceDiag->open();
}
