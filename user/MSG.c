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

volatile byte 	RS_485_MSG;	// RS485 ��Ϣ����
volatile byte 	SYSTEM_MSG;	// ϵͳ��Ϣ����

void MSGInit(void)
{
	RS_485_MSG = 0;
	SYSTEM_MSG = 0;
}

/* EOF */
