#define gbpRxBuffSize 128    //Chieu dai toi da cua frame du lieu nhan

#define RXonly 0
#define TXonly 1
#define BothRxTx 2

#define AX12MODE BothRxTx 
#ifndef RX_BUFFER_SIZE 
#define RX_BUFFER_SIZE 8     //Bat buoc phai khai bao, theo gia tri da khai bao cua bo USART
#endif
#define gbpID 0xFE      //Dia chi toan cuc. Khi nhan duoc dia chi nay, toan bo slave se thuc hien lenh

enum ERR_CODE
{
OK,
RxD_Timeout,
Wrong_Header,
Wrong_ID,
Wrong_Length,
Wrong_CheckSum,
} error_code;

//Chuong trinh xu ly frame nhan duoc
//Tham so bRxPacketLength:chieu dai toi da frame co the nhan duoc (<255)
//Gia tri tra ve: (=0xff)-Loi, frame sai cu phap; tra cuu bien error_code
//                (!=0xff)-chieu dai frame nhan duoc
unsigned char RxPacket(unsigned char bRxPacketLength);

//Chuong trinh ta frame de gui di
//Tham so data:con tro mang du lieu can gui di
//        length: kich thuoc frame can gui di 
//Tra ve (0-low level) neu nhan sai hoac ko nhan duoc frame ACK
//       (1-high level) nhan duoc frame hoi dap hop le
unsigned char TxPacket(unsigned char *data,unsigned char length);
unsigned int make16(unsigned char highByte,unsigned char lowByte);  
unsigned char highByte(unsigned int num);
unsigned char lowByte(unsigned int num);

/* Huong dan su dung thu vien:
--B1: Khoi tao chuong trinh bang CodeWizardAVR gom
------USART: cho phep ngat nhan(Bat buoc)
------USART: cho phep ngat truyen(Bat buoc neu co tra ve frame)
--B2: Thiet lap gia tri cho AX12MODE
--B3: Khai bao lai gia tri cho RX_BUFFER_SIZE
--B4: Khai bao loai chip trong file AX12.c
*/ 
