#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->startBroadcastButton, &QPushButton::clicked, [this]() {
        broadcaster.startBroadcaster();
    });
    connect(ui->stopBroadcastButton, &QPushButton::clicked, [this]() {
        broadcaster.stopBroadcaster();
    });

    connect(&broadcastReceiver, QOverload<QString>::of(&BroadcastReceiver::broadcastMessageReceived),
            [this](QString message) {
        ui->receivedBroadcastMessage->appendPlainText(message);
    });

    connect(ui->fileSelectButton, &QPushButton::clicked, [this]() {
        fileName = QFileDialog::getOpenFileName(this,
                                                tr("Open file"),
                                                "C:\\");
        qDebug() << fileName;
        ui->fileNameLineEdit->setText(fileName);
    });

    connect(ui->fileSendButton, &QPushButton::clicked, [this]() {
        FileSender *fileSender = new FileSender(fileName, QHostAddress(ui->IpLineEdit->text()));
        connect(fileSender, SIGNAL(statusUpdate(int)), ui->senderProgressBar, SLOT(setValue(int)));
    });

    connect(&fileReceiveServer, QOverload<FileReceiver*>::of(&FileReceiveServer::receiverInitialized),
            [this](FileReceiver *receiver) {
        connect(receiver, SIGNAL(statusUpdate(int)), ui->receiverProgressBar, SLOT(setValue(int)));
    });
}

void MainWindow::newConnectionReceived(FileReceiver *receiver)
{
    connect(receiver, SIGNAL(statusUpdate(int)), ui->receiverProgressBar, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
