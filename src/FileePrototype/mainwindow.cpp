#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Start the file dialog at the user's directory
    QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
    dir.setPath(env.value("USERPROFILE"));

    // Start broadcasting
    connect(ui->startBroadcastButton, &QPushButton::clicked, [this]() {
        broadcaster.startBroadcaster();
    });

    // Stop broadcasting
    connect(ui->stopBroadcastButton, &QPushButton::clicked, [this]() {
        broadcaster.stopBroadcaster();
    });

    // Define actions when receiving broadcast message
    connect(&broadcastReceiver, QOverload<QString>::of(&BroadcastReceiver::broadcastMessageReceived),
            [this](QString message) {
        ui->receivedBroadcastMessage->appendPlainText(message);
    });

    // Define actions when the "Select file" button was clicked
    connect(ui->fileSelectButton, &QPushButton::clicked, [this]() {
        fileName = QFileDialog::getOpenFileName(this, tr("Open file"), dir.absolutePath());
        dir.setPath(fileName);
        qDebug() << fileName;
        ui->fileNameLineEdit->setText(fileName);
    });

    // Define actions when instantiating a file sender
    connect(ui->fileSendButton, &QPushButton::clicked, [this]() {
        FileSender *fileSender = new FileSender(fileName, QHostAddress(ui->IpLineEdit->text()));
        connect(fileSender, SIGNAL(statusUpdate(int)), ui->senderProgressBar, SLOT(setValue(int)));
        senders.push_back(fileSender);
    });

    // Define actions when new file receiver is created
    connect(&fileReceiveServer, QOverload<FileReceiver*>::of(&FileReceiveServer::receiverInitialized),
            [this](FileReceiver *receiver) {
        connect(receiver, SIGNAL(statusUpdate(int)), ui->receiverProgressBar, SLOT(setValue(int)));
        receivers.push_back(receiver);
    });

    // Retrive all loop back interfaces
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for (int i = 0; i < list.count(); i++)
    {
        if(!list[i].isLoopback())
            if (list[i].protocol() == QAbstractSocket::IPv4Protocol)
                qDebug() << list[i].toString();
    }

    // Actions of the sender's pause/resume button
    connect(ui->senderPauseButton, &QPushButton::clicked, [&]() {
        if (ui->senderPauseButton->text() == "Pause") {
            for (int i = 0; i < senders.size(); i++) {
                senders.at(i)->pause();
            }
            ui->senderPauseButton->setText("Resume");
        } else {
            for (int i = 0; i < senders.size(); i++) {
                senders.at(i)->resume();
            }
            ui->senderPauseButton->setText("Pause");
        }
    });

    // Actions of the sender's cancel button
    connect(ui->senderCancelButton, &QPushButton::clicked, [&]() {
        for (int i = 0; i < senders.size(); i++) {
            senders.at(i)->cancel();
        }
    });

    // Actions of the receiver's pause/resume button
    connect(ui->receiverPauseButton, &QPushButton::clicked, [&]() {
        if (ui->receiverPauseButton->text() == "Pause") {
            for (int i = 0; i < receivers.size(); i++) {
                receivers.at(i)->pause();
            }
            ui->receiverPauseButton->setText("Resume");
        } else {
            for (int i = 0; i < receivers.size(); i++) {
                receivers.at(i)->resume();
            }
            ui->receiverPauseButton->setText("Pause");
        }
    });

    // Actions of the receiver's cancel button
    connect(ui->receiverCancelButton, &QPushButton::clicked, [&]() {
        for (int i = 0; i < receivers.size(); i++) {
            receivers.at(i)->cancel();
        }
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
