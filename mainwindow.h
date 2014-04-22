#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class Console;
class SettingsDialog;

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

public slots:
    /** @brief Load a specific style */
    void loadAppStyle(G_MAINWINDOW_STYLE style);
    /** @brief Reload the CSS style sheet */
    void reloadAppStylesheet();

    void openSerialPort();
    void closeSerialPort();
//    void about();
    void writeData(const QByteArray &data);
    void readData();

//    void handleError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;
    Console *console;
    SettingsDialog *settings;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
