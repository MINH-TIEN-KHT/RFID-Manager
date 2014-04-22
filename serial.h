#ifndef SERIAL_H
#define SERIAL_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Serial
{
public:
    Serial();

    QSerialPort *serialPort;
    void SerialPortInit();
public slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
};

#endif // SERIAL_H
