#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Start broadcaster
    broadcaster.startBroadcaster();

    // Start the file dialog at the user's directory
    dir.setPath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    // Create OnlineDevicesModel
    onlineDevicesModel = new OnlineDevicesModel(broadcastReceiver);
    ui->onlineDevicesTableView->setModel(onlineDevicesModel);
    connect(ui->onlineDevicesTableView, QOverload<const QModelIndex &>::of(&QTableView::clicked), [this](const QModelIndex &index) {
        ui->IpLineEdit->setText(onlineDevicesModel->getSelectedIp(index.row()));
    });

    // Start broadcasting
    connect(ui->startBroadcastButton, &QPushButton::clicked, [this]() {
        broadcaster.startBroadcaster();
    });

    // Stop broadcasting
    connect(ui->stopBroadcastButton, &QPushButton::clicked, [this]() {
        broadcaster.stopBroadcaster();
    });


    /* File sender */
    // Define actions when the "Select file" button was clicked
    connect(ui->fileSelectButton, &QPushButton::clicked, [this]() {
        fileName = QFileDialog::getOpenFileName(this, tr("Open file"), dir.absolutePath());
        dir.setPath(fileName);
        qDebug() << fileName;
        ui->fileNameLineEdit->setText(fileName);
    });

    // Define actions when instantiating a file sender
    connect(ui->fileSendButton, &QPushButton::clicked, [this]() {
        session = new TransferSession(fileName, QHostAddress(ui->IpLineEdit->text()));
        connect(session, SIGNAL(progressUpdate(int)), ui->senderProgressBar, SLOT(setValue(int)));

        // Actions of the sender's pause/resume button
        connect(ui->senderPauseButton, &QPushButton::clicked, this, &MainWindow::senderPauseButtonPressed);

        // Actions of the sender's cancel button
        connect(ui->senderCancelButton, &QPushButton::clicked, [&]() {
            session->cancel();
        });
    });


    /* For file receivers */
    // Define actions when new file receiver is created
    connect(&fileReceiveServer,
            QOverload<FileReceiver*>::of(&FileReceiveServer::receiverInitialized),
            this, QOverload<FileReceiver*>::of(&MainWindow::newConnectionReceived));

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
    qDebug() << "New connection";
    if (receivers.size() > 0) {
        disconnect(receivers[receivers.size() - 1], SIGNAL(statusUpdate(int)), ui->receiverProgressBar, SLOT(setValue(int)));
    }
    connect(receiver, SIGNAL(statusUpdate(int)), ui->receiverProgressBar, SLOT(setValue(int)));
    receivers.push_back(receiver);
}

void MainWindow::senderPauseButtonPressed()
{
    if (ui->senderPauseButton->text() == "Pause") {
        session->pause();
        ui->senderPauseButton->setText("Resume");
    } else {
        session->resume();
        ui->senderPauseButton->setText("Pause");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
