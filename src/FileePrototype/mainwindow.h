#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QFileDialog>
#include <QHostAddress>
#include <QProcessEnvironment>
#include <QNetworkInterface>

#include "broadcaster.h"
#include "broadcastreceiver.h"
#include "filesender.h"
#include "filereceiveserver.h"
#include "filereceiver.h"

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

private:
    Ui::MainWindow *ui;
    Broadcaster broadcaster;
    BroadcastReceiver broadcastReceiver;
    QString fileName;
    FileReceiveServer fileReceiveServer;

    QVector<FileSender*> senders;
    QVector<FileReceiver*> receivers;

    QDir dir;
};
#endif // MAINWINDOW_H
