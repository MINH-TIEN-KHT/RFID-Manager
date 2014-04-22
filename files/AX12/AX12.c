#include "AX12.h"
#include <mega8.h>

unsigned char ID=0;        //Dia chi cua module hien tai
unsigned char gbpRxBuffer[gbpRxBuffSize];   //Bo dem nhan-Chua toan bo frame nhan duoc
enum ERR_CODE
{ 
OK,
RxD_Timeout,
Wrong_Header,
Wrong_ID,
Wrong_Length,
Wrong_CheckSum,
} error_code;

#if (AX12MODE==BothRxTx || AX12MODE==RXonly)
extern char rx_rd_index,rx_wr_index;  //con tro doc, ghi- duoc khai bao trong chuong trinh ngat nhan USART
extern char rx_buffer[];
unsigned char RxPacket(unsigned char bRxPacketLength)
{
#define RX_TIMEOUT_COUNT2  50L
#define RX_TIMEOUT_COUNT1 (RX_TIMEOUT_COUNT2*10L)
#define CLEAR_BUFFER  rx_rd_index = rx_wr_index
unsigned long ulCounter;
unsigned char bCount, bLength;
unsigned int bChecksum;
unsigned char bTimeout;
bTimeout = 0;
//CLEAR_BUFFER;
for(bCount = 0; bCount < bRxPacketLength; bCount++)
{
        ulCounter = 0;
        while(rx_rd_index == rx_wr_index)
        {
                if(ulCounter++ > RX_TIMEOUT_COUNT1)
                {
                        bTimeout = 1;
                        break;
                }
        }
        if(bTimeout) break;   
        gbpRxBuffer[bCount]=rx_buffer[rx_rd_index++];
        if(rx_rd_index==RX_BUFFER_SIZE) rx_rd_index=0;     
        //neu doc xong bo dem thi quay ve ba dau
}
//CLEAR_BUFFER;
bLength = bCount;
if(bCount<3) 
{                       
        error_code=RxD_Timeout;
        return 0;
}
bChecksum = 0;

//Structure of each data frame:
//[0xff][0xff][len][ID1][data byte1][data byte2][data byte3]..[data byte n][chk sum][chk sum]

  if(bTimeout && (bRxPacketLength != 255))
  {
          //Error:RxD Timeout;
          error_code=RxD_Timeout;
          CLEAR_BUFFER;
  }

  if(bLength > 3) //checking is available.
  {
          if(gbpRxBuffer[0] != 0xff || gbpRxBuffer[1] != 0xff)
          {
                  //Error:Wrong Header    
                  error_code=Wrong_Header;
                  CLEAR_BUFFER;
                  return 0xff;
          }
                
           if(gbpRxBuffer[3] != ID && gbpRxBuffer[3]!=gbpID) //3 bytes: [0xff] [0xff] [len]
          {
                  //Error:Wrong ID
                  error_code=Wrong_ID;
                  CLEAR_BUFFER;
                  return 0xff;
          }
                   
          if(gbpRxBuffer[2] != bLength-3) //3 bytes: [0xff] [0xff] [len]
          {
                  //Error:Wrong Length
                  error_code=Wrong_Length;
                  CLEAR_BUFFER;
                  return 0xff;
          }
          for(bCount = 2; bCount <= bLength-3; bCount++) bChecksum +=gbpRxBuffer[bCount];  //4 bytes: [0xff][0xff][chksum][chksum]               
          bChecksum+=make16(gbpRxBuffer[bCount],gbpRxBuffer[bCount+1]);  
          if(bChecksum != 0xffff)
          {
                  //Error:Wrong CheckSum
                  error_code=Wrong_CheckSum;
                  CLEAR_BUFFER;
                  return 0xff;
          }
  }
  error_code=OK;
return bLength;
}
#endif

#if (AX12MODE==BothRxTx || AX12MODE==TXonly)
extern unsigned char tx_wr_index,tx_rd_index,tx_counter;   //con tro doc, ghi- duoc khai bao trong chuong trinh ngat truyen USART
extern char tx_buffer[];
unsigned char TxPacket(unsigned char *data,unsigned char length)
{
    #define  CHECK_TXD_FINISH   tx_counter<=0
    int i;
    unsigned int checksum;
    tx_buffer[0]=0xFF;
    tx_buffer[1]=0xFF;
    tx_buffer[2]=length+3;  //1 byte ID, 2 byte chksum  
    checksum=tx_buffer[2];
    for(i=0;i<length;i++)
    {
        tx_buffer[i+3]=*(data+i);
        checksum+=*(data+i);
    } 
    //checksum+=1;                    
    checksum=0xffff-checksum;
    tx_buffer[i+3]=highByte(checksum);
    tx_buffer[i+4]=lowByte(checksum);
    //tx_buffer hoan chinh, chuan bi truyen di    
    tx_rd_index=1;
    tx_counter=length+4;
    UDR=tx_buffer[0]; 
    while(!CHECK_TXD_FINISH); 
                                        
    if (RxPacket(255)==0xff || error_code!=OK) return 0;
    else return 1;
}
#endif

unsigned int make16(unsigned char highByte,unsigned char lowByte)
{
  return lowByte|((unsigned int)highByte<<8);
}

unsigned char highByte(unsigned int num)
{
    return (unsigned char)((num>>8)&0x00ff);
} 
unsigned char lowByte(unsigned int num)
{
    return (unsigned char)(num&0x00ff);
}
