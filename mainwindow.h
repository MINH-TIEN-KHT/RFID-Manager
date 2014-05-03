#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <inttypes.h>

#define RX_BUFFER_SIZE 11
#define TX_BUFFER_SIZE 11

#define DEVICE_ID   0x01
#define LENGTH      0x07   // 1 byte instruction + 5 bytes data + 1 byte checksum

#define PING        0x01
#define READ_DATA   0x02
#define WRITE_DATA  0x03

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // style for Application
    enum G_MAINWINDOW_STYLE
    {
        G_MAINWINDOW_STYLE_NATIVE,
        G_MAINWINDOW_STYLE_INDOOR,
        G_MAINWINDOW_STYLE_OUTDOOR,
        G_MAINWINDOW_STYLE_DARKORANGE,
        G_MAINWINDOW_STYLE_DARKORANGE2,
        G_MAINWINDOW_STYLE_DARKORANGE3
    };
    G_MAINWINDOW_STYLE currentStyle;
    QString styleFileName;

signals:
    void actionConnectTriggeredSignal();
    void actionDisconnectTriggeredSignal();

private slots:
    void onActionConnectTriggered();
    void onActionDisconnectTriggered();
    void onWriteButtonMainwindowClicked();
    void onReadButtonMainwindowClicked();


public slots:
    /** @brief Load a specific style */
    void loadAppStyle(G_MAINWINDOW_STYLE style);
    /** @brief Reload the CSS style sheet */
    void reloadAppStylesheet();

    void rfidReceivedDataUpdate(QByteArray rfidData);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    uint8_t rfid_rd_data[5];
    uint8_t rfid_wr_data[5];
};

#endif // MAINWINDOW_H
