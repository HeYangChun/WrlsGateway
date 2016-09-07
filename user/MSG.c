/*******************************************************************************
*
*   MSG 
*   Soft version:   V1.00
*   File Name:      MSG.c
*   Author   :      zzw
*   creation date:  2008-02-19
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#include "MSG.h"

volatile byte 	RS_485_MSG;	// RS485 消息队列
volatile byte 	SYSTEM_MSG;	// 系统消息队列

void MSGInit(void)
{
	RS_485_MSG = 0;
	SYSTEM_MSG = 0;
}

/* EOF */
