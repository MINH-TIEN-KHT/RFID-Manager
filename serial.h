#ifndef SERIAL_H
#define SERIAL_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Serial
{
public:
    Serial();
    QSerialPort serialPort;
    void SerialPortInit();
public slots:
    void openSerialPort();
    void closeSerialPort();
};

#endif // SERIAL_H
