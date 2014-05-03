#ifndef SERIAL_H
#define SERIAL_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <inttypes.h>

#define gbpRxBuffSize 128    //Chieu dai toi da cua frame du lieu nhan

#define RXonly 0
#define TXonly 1
#define BothRxTx 2

#define AX12MODE BothRxTx
#ifndef RX_BUFFER_SIZE
#define RX_BUFFER_SIZE 8     //Bat buoc phai khai bao, theo gia tri da khai bao cua bo USART
#endif
#define gbpID 0xFE      //Dia chi toan cuc. Khi nhan duoc dia chi nay, toan bo slave se thuc hien lenh


class Serial : public QSerialPort
{
    Q_OBJECT

public:
    explicit Serial(QSerialPort *parent = 0);
    void SerialPortInit();

signals:
    void ax12MsgReceived(QByteArray msg);

public slots:
    void openSerialPort();
    void closeSerialPort();
    void getSerialDataFromComport();
    void sendAX12MsgToComport(const char *buf, uint8_t len);

private:
    QSerialPort *serialPort;
};

#endif // SERIAL_H
