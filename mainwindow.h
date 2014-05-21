#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <inttypes.h>

#define PING        0x01
#define READ_DATA   0x02
#define WRITE_DATA  0x03
#define OUTPUT_PULSE  0x04
#define SET_DATETIME  0x05

#define INS_SIGN 1
#define AA_SIGN 2
#define BB_SIGN 3
#define CC_SIGN 4
#define NNN_SIGN 5
#define PPP_SIGN 6
#define SHARP_SIGN 7
#define SEC_SIGN 8
#define MIN_SIGN 9
#define HOUR_SIGN 10
#define DAY_SIGN 11
#define DATE_SIGN 12
#define MONTH_SIGN 13
#define YEAR_SIGN 14

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
    void receiveDataComplete();
    void dataProcessComplete();
    void writeTag();
    void readTag();
    void eraseTag();
    void syncDateTime();

private slots:
    void onActionConnectTriggered();
    void onActionDisconnectTriggered();
    void onReceiveData();
    void onReceiveDataProcess();
    void onWriteDataButtonClicked();
    void onReadDataButtonClicked();
    void onEraseDataButtonClicked();
    void onSetDateTimeButtonClicked();
    void WriteRFIDData();
    void ReadRFIDData();
    void EraseRFIDData();
    void updateTime();
    void SyncRTCDateTime();

public slots:
    /** @brief Load a specific style */
    void loadAppStyle(G_MAINWINDOW_STYLE style);
    /** @brief Reload the CSS style sheet */
    void reloadAppStylesheet();

    void rfidReceivedDataUpdate();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    QString sendDataStr;
    QString receiveDataStr;

    QString aaValueStr;
    QString bbValueStr;
    QString ccValueStr;
    QString nnnValueStr;
};

#endif // MAINWINDOW_H
