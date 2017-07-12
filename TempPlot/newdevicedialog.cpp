#include "newdevicedialog.h"
#include "ui_newdevicedialog.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

NewDeviceDialog::NewDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDeviceDialog)
{
    ui->setupUi(this);
    getDevList();
    connect(ui->btnApply, SIGNAL(clicked()), SLOT(createNewDev()));
    connect(ui->btnCancel, SIGNAL(clicked()), SLOT(cancel()));
}

NewDeviceDialog::~NewDeviceDialog()
{
    delete ui;
}

void NewDeviceDialog::createNewDev()
{
    nam = new QNetworkAccessManager();
/* For POST Methods */
    QUrl url = QUrl("http://localhost:3000/api/data/" + ui->letDev->text());
    connect(nam, SIGNAL(finished(QNetworkReply*)), SLOT(onNewDevReply(QNetworkReply*)));
    QNetworkRequest networkRequest(url);
    QByteArray postData;
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    nam->post(networkRequest, postData);
}

void NewDeviceDialog::getDevList()
{
    nam = new QNetworkAccessManager();
    QUrl url = QUrl("http://localhost:3000/api/data/");
    connect(nam, SIGNAL(finished(QNetworkReply*)), SLOT(onGetDevReply(QNetworkReply*)));
    QNetworkRequest networkRequest(url);
    nam->get(networkRequest);
}

void NewDeviceDialog::onNewDevReply(QNetworkReply* reply)
{
    // check for error
    if ( reply->error() )
    {
        qDebug() << reply->errorString();
        ui->lblReply->setText(reply->errorString());
    } else {
    // get http response code
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QVariant contentLengthV = reply->header(QNetworkRequest::ContentLengthHeader);

        qDebug() << statusCodeV.toString();
        qDebug() << contentLengthV.toString();
        QByteArray status(reply->readAll());
        qDebug() << status.data();
        ui->lblReply->setText(status.data());
    }

    reply->deleteLater();
    reply = NULL;
}

void NewDeviceDialog::onGetDevReply(QNetworkReply* reply)
{
    // check for error
    if ( reply->error() )
    {
        qDebug() << reply->errorString();
        ui->lblReply->setText(reply->errorString());
    } else {
    // get http response code
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QVariant contentLengthV = reply->header(QNetworkRequest::ContentLengthHeader);

        qDebug() << statusCodeV.toString();
        qDebug() << contentLengthV.toString();
        QByteArray status(reply->readAll());
        qDebug() << status.data();
        QString dataBody = status.data();
        QJsonDocument d = QJsonDocument::fromJson(dataBody.toUtf8());
        qDebug() << "GET:" << d;
        QJsonObject obj = d.object();
        QJsonArray objArr = obj.value("Devlist").toArray();
        //qDebug() << "GET:" << objArr.at(0).toObject().value("name").toString();
        //qDebug() << "GET:" << objArr.at(1).toObject().value("name").toString();

        foreach (const QJsonValue & value, objArr) {
            QJsonObject o = value.toObject();
            ui->listWidget->addItem(QString(o.value("name").toString()));
            qDebug() << "GET:" << o.value("name").toString();
        }
    }

    reply->deleteLater();
    reply = NULL;
}

void NewDeviceDialog::cancel()
{
    close();
}
