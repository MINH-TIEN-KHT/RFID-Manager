#include "serial.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

Serial::Serial()
{
//    serialPort = new QSerialPort(this);
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
    }

    serialPort.setPortName(portname);
    serialPort.setBaudRate(QSerialPort::Baud57600);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setStopBits(QSerialPort::OneStop);
}

void Serial::openSerialPort()
{
    if(!serialPort.isOpen())
    {
        if(serialPort.open(QIODevice::ReadWrite)) // successful
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
    if(serialPort.isOpen())
    {
        serialPort.close();
        qDebug()<<"Serial Port Closed.";
    }
}
