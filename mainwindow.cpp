#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webSocketClient.setParent(this);

    connect(&webSocketClient, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(&webSocketClient, SIGNAL(disconnected()), this, SLOT(on_disconnected()));

    connect(&webSocketClient, SIGNAL(textMessageReceived(QString)), this, SLOT(on_textMessageReceived(QString)));
    connect(&webSocketClient, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(on_binaryMessageReceived(QByteArray)));

    connect(&webSocketClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonConectar_clicked()
{
    QString link("ws://" + ui->lineServer->text() + ":" + ui->spinBoxPort->text());

    webSocketClient.open(QUrl(link));

    qDebug() << link;
}

void MainWindow::on_connected()
{
    ui->pushButtonEnviar->setEnabled(true);
    ui->pushButtonDesconectar->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->progressBar->setEnabled(true);

    ui->pushButtonConectar->setEnabled(false);
    ui->lineServer->setEnabled(false);
    ui->spinBoxPort->setEnabled(false);
}

void MainWindow::on_pushButtonDesconectar_clicked()
{
    webSocketClient.close();
}

void MainWindow::on_disconnected()
{
    ui->pushButtonEnviar->setEnabled(false);
    ui->pushButtonDesconectar->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->progressBar->setEnabled(false);

    ui->pushButtonConectar->setEnabled(true);
    ui->lineServer->setEnabled(true);
    ui->spinBoxPort->setEnabled(true);
}

void MainWindow::on_textMessageReceived(const QString &message)
{
    qDebug() << "Received: " + message;

    ui->plainTextLog->appendPlainText("Received: " + message);
}

void MainWindow::on_pushButtonEnviar_clicked()
{
    sendTextMessage(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_returnPressed()
{
    sendTextMessage(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::sendTextMessage(QString str)
{
    if (!str.isEmpty())
    {
        webSocketClient.sendTextMessage(str);

        ui->plainTextLog->appendPlainText("Sent: " + str);

        qDebug() << "Sent: " + str;
    }
}

void MainWindow::on_error(QAbstractSocket::SocketError error)
{
    QMessageBox::warning(this, "WebSocket", webSocketClient.errorString());

    this->on_disconnected();

    qDebug() << "Error:" << error;
    qDebug() << webSocketClient.errorString();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QByteArray message = QByteArray::number(value, 16).toUpper();

    qDebug() << "Sent:" << value << "by HEX message" << message;

    webSocketClient.sendBinaryMessage(message);
}

void MainWindow::on_binaryMessageReceived(const QByteArray &message)
{
    bool ok;
    int value = message.toInt(&ok, 16);

    qDebug() << "Received:" << value << "by HEX message" << message;

    ui->progressBar->setValue(value);
}
