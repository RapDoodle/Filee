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
                                                "/home/jana");
//                                                tr("Image Files (*.png *.jpg *.bmp)"));
        qDebug() << fileName;
        ui->fileNameLineEdit->setText(fileName);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
