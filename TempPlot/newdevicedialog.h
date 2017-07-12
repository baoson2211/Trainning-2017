#ifndef NEWDEVICEDIALOG_H
#define NEWDEVICEDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class NewDeviceDialog;
}

class NewDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDeviceDialog(QWidget *parent = 0);
    ~NewDeviceDialog();

public slots:
    void createNewDev();
    void getDevList();
    void onNewDevReply(QNetworkReply* rep);
    void onGetDevReply(QNetworkReply* rep);
    void cancel();

private:
    Ui::NewDeviceDialog *ui;
    QNetworkAccessManager *nam;
};

#endif // NEWDEVICEDIALOG_H
