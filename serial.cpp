#include "serial.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

Serial::Serial(QSerialPort *parent):
    QSerialPort(parent)
{
    serialPort = new QSerialPort(this);

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(getSerialDataFromComport()));

}

void Serial::SerialPortInit()
{
    QString portname;

    // Example use QSerialPortInfo
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << portInfo.portName();
        qDebug() << "Description : " << portInfo.description();
        qDebug() << "Manufacturer: " << portInfo.manufacturer();        
        portname = portInfo.portName();
        if(portInfo.manufacturer() == "Prolific")
            break;
    }
    qDebug() << "connect to " << portname;
    serialPort->setPortName(portname);
    serialPort->setBaudRate(QSerialPort::Baud57600);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setStopBits(QSerialPort::OneStop);
    qDebug() << "Serialport Init complete.";
//    serialPort->open(QIODevice::ReadWrite);
//    serialPort->write("Hello123");
//    serialPort->close();
}

void Serial::openSerialPort()
{
    if(!serialPort->isOpen())
    {
        if(serialPort->open(QIODevice::ReadWrite)) // successful
        {           
            qDebug()<<"Serial Port Opened Ok.";
        }
        else
        {
            qDebug()<<"Serial Port Open Error.";
        }
    }
}

void Serial::closeSerialPort()
{
    if(serialPort->isOpen())
    {
        serialPort->close();
        qDebug()<<"Serial Port Closed.";
    }
}

void Serial::getSerialDataFromComport()
{
    QByteArray receiveData;
    receiveData = serialPort->readAll();
    qDebug()<< "get serial data from comport";
    emit ax12MsgReceived(receiveData);
}

void Serial::sendAX12MsgToComport(const char *buf, uint8_t len)
{
    qDebug()<<"write to comport";
    serialPort->write((const char *)buf, len);
}
