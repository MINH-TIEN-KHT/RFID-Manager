#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentStyle(G_MAINWINDOW_STYLE_DARKORANGE3),
    styleFileName(QCoreApplication::applicationDirPath() + ":styles/files/styles/style-darkorange3.css")
{
    ui->setupUi(this);
    loadAppStyle(currentStyle);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);

    serial = new QSerialPort(this);

    QString portname;
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << portInfo.portName();
        qDebug() << "Description : " << portInfo.description();
        qDebug() << "Manufacturer: " << portInfo.manufacturer();
        portname = portInfo.portName();
        if(portInfo.manufacturer() == "Prolific")
            break;
    }
    qDebug() << "Target Port Name: " << portname;
    serial->setPortName(portname);

    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(onActionConnectTriggered()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(onActionDisconnectTriggered()));
    connect(ui->writeDataButton, SIGNAL(clicked()),this, SLOT(onWriteButtonMainwindowClicked()));
    connect(ui->readDataButton, SIGNAL(clicked()),this, SLOT(onReadButtonMainwindowClicked()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    serial->flush();
    serial->close();
    delete ui;
}

void MainWindow::onActionConnectTriggered()
{
    if(!serial->isOpen())
    {
        if(serial->open(QIODevice::ReadWrite)) // successful
        {
            qDebug()<<"Serial Port Opened Ok.";
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);

            serial->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
            serial->setParity(QSerialPort::NoParity);
            serial->setDataBits(QSerialPort::Data8);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            serial->setStopBits(QSerialPort::OneStop);
            qDebug() << "Serialport Init complete.";
        }
        else
        {
            qDebug()<<"Serial Port Open Error.";
        }
    }
}

void MainWindow::onActionDisconnectTriggered()
{
    if(serial->isOpen())
    {
        serial->close();
        qDebug()<<"Serial Port Closed.";
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::onWriteButtonMainwindowClicked()
{
    qDebug("Write data button clicked");
    // format data frame:  iINSTRUCTIONaAAbBBcCCnNNN#
    sendDataStr.clear();

    sendDataStr.append("i" + QString::number(WRITE_DATA)); // add instruction
    sendDataStr.append("a" + ui->aalineEditWrite->text());
    sendDataStr.append("b" + ui->bblineEditWrite->text());
    sendDataStr.append("c" + ui->cclineEditWrite->text());
    sendDataStr.append("n" + ui->nnnlineEditWrite->text());

    sendDataStr.append("#");

    serial->write(sendDataStr.toLatin1());
    serial->flush();
}

void MainWindow::onReadButtonMainwindowClicked()
{
    qDebug("Read data button clicked");

    // format data frame:  iINSTRUCTION#
    sendDataStr.clear();

    sendDataStr.append("i" + QString::number(READ_DATA)); // add instruction

    sendDataStr.append("#");

    serial->write(sendDataStr.toLatin1());
    serial->flush();
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

void MainWindow::rfidReceivedDataUpdate(QByteArray rfidData)
{
//    ui->aalineEditRead->setText(QString::number(rfidData[0]));
//    ui->bblineEditRead->setText(QString::number(rfidData[1]));
//    ui->cclineEditRead->setText(QString::number(rfidData[2]));
//    ui->nnnlineEditRead->setText(QString::number(rfidData[3] + rfidData[4]*256));
    qDebug()<< "show receive data";
    ui->plainTextEdit->insertPlainText(QString::fromLatin1(rfidData));
}

