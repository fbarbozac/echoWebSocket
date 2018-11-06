#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonConectar_clicked();
    void on_connected();

    void on_pushButtonDesconectar_clicked();
    void on_disconnected();

    void on_textMessageReceived(const QString &);

    void on_pushButtonEnviar_clicked();
    void on_lineEdit_returnPressed();

    void on_error(QAbstractSocket::SocketError error);

    void on_horizontalSlider_valueChanged(int value);
    void on_binaryMessageReceived(const QByteArray &);

private:
    Ui::MainWindow *ui;
    QWebSocket webSocketClient;

    void sendTextMessage(QString str);
};

#endif // MAINWINDOW_H
