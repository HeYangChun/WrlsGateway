/*******************************************************************************
*
*    
*   Soft version:   
*   File Name:      UART3.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-25
*   module description:DTS 串口通信应用层
*   Copyright (C) 
*
********************************************************************************/
#include "COMM.h"
#include "sTimeout.h"
#include "initTask.h"
#include "GLOBAL.h"
#include "buffer.h"
#include "uart.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"
#include "wifi.h"

//cBuffer _transBuffer[2];
ascomm com;
ascomm com_;
//static unsigned char trans0Data[TRANS_BUFFER_SIZE];
//static unsigned char trans1Data[TRANS_BUFFER_SIZE];

//static u08 _recvBuf;
/*******************************************************************************
Function: 		CommInit()
Description: 	串口及串口通信协议层初始化
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void CommInit(void)
{
	//ClcpInit(); // 通信链路层初始化
	//NPL_init(); // 网络层初始化
}
/*******************************************************************************
Function: 		commTask()
Description: 	串口任务
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void commTask(void)
{
	//byte len;

//	u08 can_box_num;
	
     //  if(_cmdIsSend)//′??ú????
	//{
		// ZIGBEE receive
	 if (uartHasNewLineLF(COM_WIFI))
	 {
		 wifiReadResponse();
                 // _cmdIsSend=0;
		 uartwifiTimer = TIMING_MONITOR_WIFI;
	 }
	//}
	/*else//
	{// wifi receive
	 if(uartHasNewLine(COM_WIFI))
	 {
		wifiReadResponse();
		uartwifiTimer = TIMING_MONITOR_WIFI;
	 }

        }*/

	if (uartwifiTimer == 0)
	{
		uartFlushReceiveBuffer(COM_WIFI); // 
		uartwifiTimer = TIMING_MONITOR_WIFI;
		
	}
}




/* EOF */
