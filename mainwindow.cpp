#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QFile>
#include "files/AX12/AX12.h"
#include "serial.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentStyle(G_MAINWINDOW_STYLE_DARKORANGE3),
    styleFileName(QCoreApplication::applicationDirPath() + ":styles/files/styles/style-darkorange3.css")
{
    ui->setupUi(this);
    // load Stylesheet
    loadAppStyle(currentStyle);
    // serialport
    Serial serialport;
    serialport.SerialPortInit();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
//    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAppStyle(MainWindow::G_MAINWINDOW_STYLE style)
{
    switch (style)
    {
        case G_MAINWINDOW_STYLE_NATIVE:
            // Native mode means setting no style
            // so if we were already in native mode
            // take no action
            // Only if a style was set, remove it.
            if (style != currentStyle)
            {
                qApp->setStyleSheet("");
            }
            break;
        case G_MAINWINDOW_STYLE_INDOOR:
            qApp->setStyle("fusion");
            styleFileName = ":styles/files/styles/style-indoor.css";
            reloadAppStylesheet();
            break;
        case G_MAINWINDOW_STYLE_OUTDOOR:
            qApp->setStyle("fusion");
            styleFileName = ":styles/files/styles/style-outdoor.css";
            reloadAppStylesheet();
            break;
        case G_MAINWINDOW_STYLE_DARKORANGE:
            qApp->setStyle("fusion");
            styleFileName = ":styles/files/styles/style-darkorange.css";
            reloadAppStylesheet();
            break;
        case G_MAINWINDOW_STYLE_DARKORANGE2:
            qApp->setStyle("fusion");
            styleFileName = ":styles/files/styles/style-darkorange2.css";
            reloadAppStylesheet();
            break;
        case G_MAINWINDOW_STYLE_DARKORANGE3:
            qApp->setStyle("fusion");
            styleFileName = ":styles/files/styles/style-darkorange3.css";
            reloadAppStylesheet();
            break;
        default:
            break;
        currentStyle = style;
    }
}

void MainWindow::reloadAppStylesheet()
{
    // Load style sheet
    QFile* styleSheet = new QFile(styleFileName);
    if (!styleSheet->exists())
    {
        styleSheet = new QFile(":general/files/styles/style-darkorange3.css");
    }
    if (styleSheet->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString style = QString(styleSheet->readAll());
        style.replace("ICONDIR", QCoreApplication::applicationDirPath()+ "styles/files/styles/");
        qApp->setStyleSheet(style);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Application did not load a new style"));
        msgBox.setInformativeText(tr("Stylesheet file %1 was not readable").arg(styleFileName));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    delete styleSheet;
}

void MainWindow::openSerialPort()
{
//    SettingsDialog::Settings portSettings = settings->settings();
//    serialport->setPortName(portSettings.name);
//    if (serialport->open(QIODevice::ReadWrite)) {
//        if (serialport->setBaudRate(portSettings.baudRate)
//                && serialport->setDataBits(portSettings.dataBits)
//                && serialport->setParity(portSettings.parity)
//                && serialport->setStopBits(portSettings.stopBits)
//                && serialport->setFlowControl(portSettings.flowControl)) {

//            ui->actionConnect->setEnabled(false);
//            ui->actionDisconnect->setEnabled(true);
//            ui->actionConfigure->setEnabled(false);
//            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
//                                       .arg(portSettings.name).arg(portSettings.stringBaudRate).arg(portSettings.stringDataBits)
//                                       .arg(portSettings.stringParity).arg(portSettings.stringStopBits).arg(portSettings.stringFlowControl));

//        } else {
//            serialport->close();
//            QMessageBox::critical(this, tr("Error"), serialport->errorString());

//            ui->statusBar->showMessage(tr("Open error"));
//        }
//    } else {
//        QMessageBox::critical(this, tr("Error"), serialport->errorString());

//        ui->statusBar->showMessage(tr("Configure error"));
//    }
}

void MainWindow::closeSerialPort()
{
//    serialport->close();
//    console->setEnabled(false);
//    ui->actionConnect->setEnabled(true);
//    ui->actionDisconnect->setEnabled(false);
//    ui->actionConfigure->setEnabled(true);
//    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::writeData(const QByteArray &data)
{
    serialport->write(data);
}

void MainWindow::readData()
{
    QByteArray data = serialport->readAll();
//    console->putData(data);
}


