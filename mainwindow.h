#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "serial.h"

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
    Serial serialport;
public slots:
    /** @brief Load a specific style */
    void loadAppStyle(G_MAINWINDOW_STYLE style);
    /** @brief Reload the CSS style sheet */
    void reloadAppStylesheet();

private:
    Ui::MainWindow *ui;
    Console *console;
    SettingsDialog *settings;
};

#endif // MAINWINDOW_H
