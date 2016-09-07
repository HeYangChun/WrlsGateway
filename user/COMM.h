/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      UART3.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-25
*   module description:DTS 串口通信应用层
*   Copyright (C) 
*
********************************************************************************/
#if !defined(DTS_COMM_H)
#define DTS_COMM_H
#include "GLOBAL.h"
#include "buffer.h"
#define TRANS_BUFFER_SIZE		100
#define AS_SEND_BUF_MAX	        100
typedef struct _ASS_COMM
{
	byte cmd;
	byte len;
	byte buf[AS_SEND_BUF_MAX+1];
} _ascomm;

typedef union _ASU_COMM
{
	byte sbuf[AS_SEND_BUF_MAX+3];
	_ascomm comm;
} ascomm, *pAscomm;


void CommInit(void);
void commTask(void);

#endif // !defined(DTS_COMM_H)

/* EOF */
