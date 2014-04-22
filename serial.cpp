#include "serial.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

Serial::Serial()
{

}

void Serial::SerialPortInit()
{
//    serialPort = new QSerialPort(this);

    // Example use QSerialPortInfo
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << portInfo.portName();
        qDebug() << "Description : " << portInfo.description();
        qDebug() << "Manufacturer: " << portInfo.manufacturer();
    }
}

void Serial::openSerialPort()
{

}

void Serial::closeSerialPort()
{

}

void Serial::writeData(const QByteArray &data)
{

}

void Serial::readData()
{

}
