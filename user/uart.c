#include "stm8s_lib.h"
#include "CircleBuffer.h"
#include "uart.h"
#include "GLOBAL.h"
#include "buffer.h"
#include "string.h"
#include "wifi.h"


#define BAUD_115200  115200
#define BAUD_9600  9600

//#define RX_BUFF_SIZE  256


cBuffer uartRxBuffer[3];
static unsigned char uart0RxData[UART0_RX_BUFFER_SIZE];
static unsigned char uart3RxData[UART3_RX_BUFFER_SIZE];


void UART1_Configuration(void)
{
  //BufferInit(&rxBuffer, rxBuf, RX_BUFF_SIZE);
  
  // UART1的IO口初始化
  GPIO_Init(UART1_PORT, UART1_RX_PIN, GPIO_MODE_IN_FL_NO_IT);//
  GPIO_Init(UART1_PORT, UART1_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  
  // 打开UART1时钟
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
  
  UART1_DeInit();
  
  UART1_Init((u32)BAUD_115200,               // 115200bps
             UART1_WORDLENGTH_8D,           // 8位数据位
             UART1_STOPBITS_1,              // 1位停止位
             UART1_PARITY_NO,               // 无校验位
             UART1_SYNCMODE_CLOCK_DISABLE,  // 禁止同步时钟
             UART1_MODE_TXRX_ENABLE);       // 接收和发送使能
  
  // 禁止发送寄存器空中断
  UART1_ITConfig(UART1_IT_TXE, DISABLE);
  // 禁止发送完成中断
  UART1_ITConfig(UART1_IT_TC, DISABLE);
  // 使能接收寄存器非空中断
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  UART1_Cmd(ENABLE);
  uart0InitBuffers();
}


void UART3_Configuration(void)
{
  //BufferInit(&rxBuffer, rxBuf, RX_BUFF_SIZE);
  
  // UART1的IO口初始化
  GPIO_Init(UART3_PORT, UART3_RX_PIN, GPIO_MODE_IN_FL_NO_IT);//
  GPIO_Init(UART3_PORT, UART3_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  
  // 打开UART1时钟
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART3, ENABLE);
  
  UART3_DeInit();
  
  UART3_Init((u32)BAUD_9600,                 //9600bps
             UART3_WORDLENGTH_8D,           // 8位数据位
             UART3_STOPBITS_1,              // 1位停止位
             UART3_PARITY_NO,               // 无校验位
             UART3_MODE_TXRX_ENABLE);       // 接收和发送使能
  
  // 禁止发送寄存器空中断
  UART3_ITConfig(UART3_IT_TXE, DISABLE);
  // 禁止发送完成中断
  UART3_ITConfig(UART3_IT_TC, DISABLE);
  // 使能接收寄存器非空中断
  UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
  
  UART3_Cmd(ENABLE);
  uart2InitBuffers();
}

void uart0InitBuffers(void)
{
		// initialize the UART0 buffers
    bufferInit(&uartRxBuffer[0], uart0RxData, UART0_RX_BUFFER_SIZE);
}
void uart2InitBuffers(void)
{
		// initialize the UART0 buffers
    bufferInit(&uartRxBuffer[2], uart3RxData, UART3_RX_BUFFER_SIZE);
}


void uartFlushReceiveBuffer(u08 nUart)
{
	// flush all data from receive buffer
	bufferFlush(&uartRxBuffer[nUart]);
}

cBuffer* uartGetRxBuffer(u08 nUart)
{
	// return rx buffer pointer
	return &uartRxBuffer[nUart];
}


int uartGetByte(u08 nUart)
{
	// get single byte from receive buffer (if available)
	u08 c;
	if(uartReceiveByte(nUart, &c))
		return c;
	else
		return -1;
}


u08 uartSendBuffer(u08 nUart, char *buffer, u16 nBytes)
{
  if(nUart==COM1)
  {
    UART_Write(nUart,buffer, nBytes);
  
  }
  else  if(nUart==COM3)
  {
    UART_Write(nUart,buffer, nBytes);
  }
  else
  {}
  return 1;
}
void UART_WriteByte(u08 nUart,char data)
{
  if(nUart==COM1)
  {
    UART1_SendData8(data);
    while(!UART1_GetFlagStatus(UART1_FLAG_TC));
  
  }
  else  if(nUart==COM3)
  {
    UART3_SendData8(data);
    while(!UART3_GetFlagStatus(UART3_FLAG_TC));
  }
  else
  {}
  
}

void UART_Write(u08 nUart,char *buf, u32 len)
{
  if(nUart==COM1)
  {
     for(; len>0; len--)
     {
      UART_WriteByte(nUart,*buf++);
      IWDG_ReloadCounter();	// 踢看门狗
     }
  }
  else  if(nUart==COM3)
  {
    for(; len>0; len--)
     {
      UART_WriteByte(nUart,*buf++);
      IWDG_ReloadCounter();	// 踢看门狗
     }
  }
  else
  {}
 
}

u08 uartReceiveByte(u08 nUart, u08* rxData)
{
	// make sure we have a receive buffer
	if(uartRxBuffer[nUart].size)
	{
		// make sure we have data
		if(uartRxBuffer[nUart].datalength)
		{
			// get byte from beginning of buffer
			//NVIC_SETPRIMASK();
			*rxData = bufferGetFromFront(&uartRxBuffer[nUart]);
			//NVIC_RESETPRIMASK();
			return TRUE;
		}
		else
			return FALSE;			// no data
	}
	else
		return FALSE;				// no buffer
}

/*u32 UART_ReadByte(u8 *data)
{
  if(rxBuffer.IsEmpty(&rxBuffer))
  {return 0;}
  else
  {
    *data = rxBuffer.Read(&rxBuffer);
    return 1;
  }
}*/

void UART_RxHandler(u8 data)
{
  //rxBuffer.Write(&rxBuffer, data);
 /* if(waitsmssend==1)	//
  {
     if(data== '>' ) 
     {
       smssenddata=1;
       waitsmssend=0;
     }
     else
     {
      bufferAddToEnd(&uartRxBuffer[0], data);
     }
   }
  else
  {*/
   if(data== '>' ) 
   {
    waitsmssend=1;
   }
   else
   {
    bufferAddToEnd(&uartRxBuffer[0], data);
   }
   
 // }
  
}
void UART3_RxHandler(u8 data)
{

   if(data== 0x89 ) 
   {
    	air.ack=IR_OK;
   }
   else if(data== 0xE0)
   {
     	air.ack=IR_ERR; 
     	IR.Learnflag=0xff;
   	// bufferAddToEnd(&uartRxBuffer[2], data);
   }
   else if(data!= 0xE0)
   {
   	//He:This is a fucking logic!!!!
     	IR.Learnflag=2;  //红外学习成功标志
   }
   else
   {
   //He: never run to here!,fucking !
     	air.ack=IR_NONE;
   }
   
 // }
  
}


int uartHasNewLine(u08 nUart)
{
	int ret = 0;
	cBuffer* buf = uartGetRxBuffer(nUart);
	char last = 0x00;
	unsigned short index = 0;
	char dat;
	
	while (index < buf->datalength) 
	{
		dat = bufferGetAtIndex(buf, index);
		//if ( dat == '\0' )	//	might be in the binary data
		//	break;
		if ( dat == '*' && last == '*' ) 
		{
			ret = 1;
			break;
		}
		last = dat;
		index++;
	}
	return ret;
}

int uartReadLine(u08 nUart, char* buf, u16 maxlen)
{
	u16 i = 0;
	int ret;
	
	while (i < maxlen - 1) 
	{
		if ( (ret = uartGetByte(nUart)) != -1 ) 
		{
			buf[i] = (char)ret;
			//if ( buf[i] == 0x00 )	//	may be in binary data
			//	break;
			if ( buf[i] == '*' && i>0 && buf[i-1] == '*' ) 
			{
				i--;
				break;
			}
			i++;
		}
	}
	buf[i] = 0x00;

	return i;
}
int uartHasNewLineLF(u08 nUart)
{
	int ret = 0;
	cBuffer* buf = uartGetRxBuffer(nUart);
	char last[512] = {0x00};
	unsigned short index = 0;
	char dat;
	
	while (index < buf->datalength) 
	{
		dat = bufferGetAtIndex(buf, index);
		//if ( dat == '\0' )	//	might be in the binary data
			//break;
		//if ( dat == '\n' && last == '\r' ) 
        if ( ((dat == '\n') && (last[index-1] == '\r'))||((dat == 0x43)&&(last[index-1] == 0xC2) && (last[10] == 0x48)) ) 
		{
			ret = 1;
			break;
		}
        if(strstr(last, "4,CLOSED"))
        {
          ret = 1;
			break;
        }
		last[index] = dat;
		index++;
	}
	return ret;
}
int uartReadLineWifi(u08 nUart, char* buf, u16 maxlen)
{
	u16 i = 0;
	int ret;
	cBuffer* buftemp = uartGetRxBuffer(nUart);
	//char c;
	while (i < (maxlen - 1)) 
	{
		if ( (ret = uartGetByte(nUart)) != -1 ) 
		{
			buf[i] = (char)ret;
			//if ( buf[i] == 0x00 )	//	may be in binary data
				//break;
           // if ( ((buf[i] == 0x0A) && (i>0) && (buf[i-1] == 0x0D)) ) //0x0A-换行键，0x0xD-回车键
            if(((buf[i] == 0x0A)&&(i>0)&&(buf[i-1] == 0x0D))||((buf[i] == 0x43)&&(i>0)&&(buf[i-1] == 0xC2)&&(buf[10] == 0x48)))
			{
				i--;
				
				break;
			}
            if(strstr(buf, "4,CLOSED"))
        {
          i--;
			break;
        }
			i++;
		}
	}
	
	/*if(waitsmssend==1)	//
	{
         if((bufferGetAtIndex(buftemp, 0))== '>' ) 
          {
              ret = uartGetByte(nUart);//
	      i++;
	      buf[i-1]=(char)ret;  
	      buf[i] = 0x00;

          }
	  else
	  {
	   buf[i] = 0x00;
	  }
	    
	}
	else
	{
	 buf[i] = 0x00;
	}*/
	return i;
}

void uartWriteLine(u08 nUart, char* buf)
{
	uartSendBuffer(nUart, buf, strlen(buf));
}
void uartPrintln(u08 nUart, char* buf)
{
	char edlBuf[2] = { '\r','\n'};//DEBUG   0x0d,0x0a
//	char edlBuf[2] = { '\n', '\r'};
	uartWriteLine(nUart, buf);
	uartSendBuffer(nUart, edlBuf, 2);
}


