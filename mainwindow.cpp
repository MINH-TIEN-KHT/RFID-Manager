#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTimer>

QDate curDate;
QTime curTime;

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

    curDate = QDate::currentDate();
    ui->bblineEditWrite->setText(QString::number(curDate.day()+curDate.dayOfWeek()));

    ui->dateTimeEdit->setDisplayFormat("ddd, dd/MM/yyyy hh:mm:ss");
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

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

    connect(serial, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    connect(this, SIGNAL(receiveDataComplete()), this, SLOT(onReceiveDataProcess()));
    connect(this, SIGNAL(dataProcessComplete()), this, SLOT(rfidReceivedDataUpdate()));

    connect(ui->writeDataButton, SIGNAL(clicked()), this, SLOT(onWriteDataButtonClicked()));
    connect(ui->readDataButton, SIGNAL(clicked()), this, SLOT(onReadDataButtonClicked()));
    connect(ui->eraseDataButton, SIGNAL(clicked()), this, SLOT(onEraseDataButtonClicked()));
    connect(ui->setDateTimeButton, SIGNAL(clicked()), this, SLOT(onSetDateTimeButtonClicked()));

    connect(this, SIGNAL(writeTag()), this, SLOT(WriteRFIDData()));
    connect(this, SIGNAL(readTag()), this, SLOT(ReadRFIDData()));
    connect(this, SIGNAL(eraseTag()), this, SLOT(EraseRFIDData()));
    connect(this, SIGNAL(syncDateTime()), this, SLOT(SyncRTCDateTime()));

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
    QMessageBox msgBox;
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

            ui->statusBar->showMessage("Comport is connected.");
            serial->setRequestToSend(0);
            serial->setDataTerminalReady(1);
            serial->setDataTerminalReady(0);
        }
        else
        {
            qDebug()<<"Serial Port Open Error.";
            msgBox.setText("Can not connect to comport, please try again!");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
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

        ui->statusBar->showMessage("Comport is disconnected.");
    }
}

void MainWindow::WriteRFIDData()
{
    QMessageBox msgBox;
    bool aaOK, bbOK, ccOK, nnnOK;
    qDebug("Write data button clicked");
    // format data frame:  iINSTRUCTIONaAAbBBcCCnNNN#
    sendDataStr.clear();

    if(ui->aalineEditWrite->text().isEmpty() || \
            ui->bblineEditWrite->text().isEmpty() || \
            ui->cclineEditWrite->text().isEmpty() || \
            ui->nnnlineEditWrite->text().isEmpty())
    {
        msgBox.setText("Data field is empty, please try again!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else if(ui->aalineEditWrite->text().toInt(&aaOK)<0 ||
            ui->aalineEditWrite->text().toInt()>99 ||
            ui->bblineEditWrite->text().toInt(&bbOK)<0 ||
            ui->bblineEditWrite->text().toInt()>38 ||
            ui->cclineEditWrite->text().toInt(&ccOK)<0 ||
            ui->cclineEditWrite->text().toInt()>99 ||
            ui->nnnlineEditWrite->text().toInt(&nnnOK)<0 ||
            ui->nnnlineEditWrite->text().toInt()>999
            )
    {
        msgBox.setText("Data range is not correct, please try again!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else if(aaOK==0 || bbOK==0 || ccOK==0 || nnnOK==0)
    {
        msgBox.setText("Data type is not correct, please try again!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else
    {
        sendDataStr.append("i" + QString::number(WRITE_DATA)); // add instruction
        sendDataStr.append("a" + ui->aalineEditWrite->text());
        sendDataStr.append("b" + ui->bblineEditWrite->text());
        sendDataStr.append("c" + ui->cclineEditWrite->text());
        sendDataStr.append("n" + ui->nnnlineEditWrite->text());

        sendDataStr.append("#");

        serial->write(sendDataStr.toLatin1());
        serial->flush();

        curDate = QDate::currentDate();
        ui->bblineEditWrite->setText(QString::number(curDate.day()+curDate.dayOfWeek()));
//        msgBox.setText("Card writing is successful.");
//        msgBox.setIcon(QMessageBox::Information);
//        msgBox.exec();
    }
}

void MainWindow::ReadRFIDData()
{
    qDebug("Read data button clicked");

    // format data frame:  iINSTRUCTION#
    sendDataStr.clear();

    sendDataStr.append("i" + QString::number(READ_DATA)); // add instruction

    sendDataStr.append("#");

    serial->write(sendDataStr.toLatin1());
    serial->flush();  
}

void MainWindow::EraseRFIDData()
{
    QMessageBox msgBox;
    qDebug("Erase data button clicked");
    // format data frame:  iINSTRUCTIONaAAbBBcCCnNNN#
    sendDataStr.clear();

    sendDataStr.append("i" + QString::number(WRITE_DATA)); // add instruction
    sendDataStr.append("a0");
    sendDataStr.append("b0");
    sendDataStr.append("c0");
    sendDataStr.append("n0");

    sendDataStr.append("#");

    serial->write(sendDataStr.toLatin1());
    serial->flush();

    ui->aalineEditRead->clear();
    ui->bblineEditRead->clear();
    ui->cclineEditRead->clear();
    ui->nnnlineEditRead->clear();

//    msgBox.setText("Card erasing is successful.");
//    msgBox.setIcon(QMessageBox::Information);
//    msgBox.exec();
}

void MainWindow::updateTime()
{
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::SyncRTCDateTime()
{
    qDebug("Sync button clicked");
    sendDataStr.clear();

    curDate = QDate::currentDate();
    curTime = QTime::currentTime();

    sendDataStr.append("i" + QString::number(SET_DATETIME)); // add instruction
    sendDataStr.append("s" + QString::number(curTime.second())); // seconds
    sendDataStr.append("m" + QString::number(curTime.minute())); // minutes
    sendDataStr.append("h" + QString::number(curTime.hour())); // hours
    sendDataStr.append("d" + QString::number(curDate.day())); // days of month (1-31)
    sendDataStr.append("D" + QString::number(curDate.dayOfWeek())); // days of week (1-7)
    sendDataStr.append("M" + QString::number(curDate.month())); // months
    sendDataStr.append("y" + QString::number(curDate.year()-2000)); // years

    sendDataStr.append("#");

    serial->write(sendDataStr.toLatin1());
    serial->flush();
}

void MainWindow::onReceiveData()
{
    QString str;
    uint8_t size;
    if(serial->bytesAvailable())
    {
       str = QString::fromLatin1(serial->readAll());
       receiveDataStr.append(str);
       size = receiveDataStr.size();

       if(receiveDataStr.at(size-1)== '#')
       {
           serial->flush();
           emit receiveDataComplete();
       }
    }
}

void MainWindow::onReceiveDataProcess()
{
    uint8_t i, flag=0;
    uint8_t size = receiveDataStr.size();

    aaValueStr.clear();
    bbValueStr.clear();
    ccValueStr.clear();
    nnnValueStr.clear();

    for(i=0; i<size; i++)
    {
        if(receiveDataStr.at(i) == 'a')
        {
            qDebug("match a");
            flag = AA_SIGN;
        }
        else if(receiveDataStr.at(i) == 'b')
        {
            qDebug("match b");
            flag = BB_SIGN;
        }
        else if(receiveDataStr.at(i) == 'c')
        {
            qDebug("match c");
            flag = CC_SIGN;
        }
        else if(receiveDataStr.at(i) == 'n')
        {
            qDebug("match n");
            flag = INS_SIGN;
        }
        else if(receiveDataStr.at(i) == '#')
        {
            qDebug("match #");
            break;
        }
        else
        {
            if(flag == AA_SIGN)
            {
                aaValueStr.append(receiveDataStr.at(i));
            }
            else if(flag == BB_SIGN)
            {
                bbValueStr.append(receiveDataStr.at(i));
            }
            else if(flag == CC_SIGN)
            {
                ccValueStr.append(receiveDataStr.at(i));
            }
            else if(flag == INS_SIGN)
            {
                nnnValueStr.append(receiveDataStr.at(i));
            }
        }
    }
    receiveDataStr.clear();
    emit dataProcessComplete();
}

void MainWindow::onWriteDataButtonClicked()
{
    QMessageBox msgBox;
    if(serial->isOpen())
        emit writeTag();
    else
    {
        msgBox.setText("System is not connected, please try to connect again.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void MainWindow::onReadDataButtonClicked()
{
    QMessageBox msgBox;
    if(serial->isOpen())
        emit readTag();
    else
    {
        msgBox.setText("System is not connected, please try to connect again.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void MainWindow::onEraseDataButtonClicked()
{
    QMessageBox msgBox;
    if(serial->isOpen())
        emit eraseTag();
    else
    {
        msgBox.setText("System is not connected, please try to connect again.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void MainWindow::onSetDateTimeButtonClicked()
{
    QMessageBox msgBox;
    if(serial->isOpen())
        emit syncDateTime();
    else
    {
        msgBox.setText("System is not connected, please try to connect again.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
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

void MainWindow::rfidReceivedDataUpdate()
{

    int16_t aaValue=0, bbValue=0, ccValue=0, nnnValue=0;

    ui->aalineEditRead->setText(aaValueStr);
    ui->bblineEditRead->setText(bbValueStr);
    ui->cclineEditRead->setText(ccValueStr);
    ui->nnnlineEditRead->setText(nnnValueStr);

    aaValue = aaValueStr.toInt();
    bbValue = bbValueStr.toInt();
    ccValue = ccValueStr.toInt();
    nnnValue = nnnValueStr.toInt();

    curDate = QDate::currentDate();

    if(aaValue>=0 && aaValue<=99 && (bbValue == (curDate.day() + curDate.dayOfWeek())))   // Tag Valid
    {
        qDebug("Read data button clicked");

        QMessageBox msgBox;
        QPixmap pixmap(":/images/files/images/ok-icon.png");

        msgBox.setIconPixmap(pixmap);
        msgBox.setText("Card is valid.");
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.exec();
        qDebug("exit Message box.");

//        emit eraseTag();
    }
    else if(aaValue==0 && bbValue==0 && ccValue==0 && nnnValue==0 )
    {
        QMessageBox msgBox;
        msgBox.setText("Card is empty.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else // Tag Invalid
    {
        QMessageBox msgBox;
        QPixmap pixmap(":/images/files/images/Alarm-Error-icon.png");

        msgBox.setIconPixmap(pixmap);
        msgBox.setText("Card is invalid.");
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.exec();
    }
}
