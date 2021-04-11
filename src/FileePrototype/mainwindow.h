#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QFileDialog>
#include <QHostAddress>
#include <QStandardPaths>

#include "./broadcast/broadcaster.h"
#include "./broadcast/broadcastreceiver.h"
#include "./transfer/sender/filesender.h"
#include "./transfer/receiver/filereceiveserver.h"
#include "./transfer/receiver/filereceiver.h"
#include "./models/onlinedevicesmodel.h"
#include "./transfer/sender/transfersession.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newConnectionReceived(FileReceiver*);
    void senderPauseButtonPressed();

private:
    Ui::MainWindow *ui;
    Broadcaster broadcaster;
    BroadcastReceiver broadcastReceiver;
    QString fileName;
    FileReceiveServer fileReceiveServer;
    TransferSession *session = nullptr;

    QVector<FileReceiver*> receivers;
    OnlineDevicesModel *onlineDevicesModel = nullptr;

    QDir dir;
};
#endif // MAINWINDOW_H
