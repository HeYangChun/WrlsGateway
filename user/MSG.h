/*******************************************************************************
*
*   MSG 
*   Soft version:   V1.00
*   File Name:      DTS_MSG.h
*   Author   :      zzw
*   creation date:  2008-02-19
*	  module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#if !defined(MSG_H_INCLUDED)
#define MSG_H_INCLUDED
#include "GLOBAL.h"
#include "MSG.h"

#define SendMsg(MSG_C, msg)	((MSG_C) |= (msg))
#define GetMsg(MSG_C, msg)	((MSG_C) & (msg))
#define ClsMsg(MSG_C, msg)	((MSG_C) &= (~(msg)))
void MSGInit(void);

////////////////////////////////////////////////////////
// 
// 消息及外设ID定义
//


/////////////////////////////////////////////////////////
//
// 系统消息定义
//
extern volatile byte 	SYSTEM_MSG;	// 系统消息队列

#define SYSTEM_MSG_NONE				(0 << 0) // 无消息
#define SYSTEM_MSG_READY			(1 << 0) // 系统准备就绪
#define SYSTEM_MSG_ADCEND			(1 << 1) // AD Convent completed
#define SYSTEM_MSG_TURNOFF		    (1 << 2) // 确定关机信号
#define SYSTEM_MSG_SENDOFF			(1 << 3) // 发送关机信号
#define SYSTEM_MSG_CTUS				(1 << 4) // 未启动控制任务

/////////////////////////////////////////////////////////
//
// RS485消息定义
//
extern volatile byte 	RS_485_MSG;	// RS485 消息队列

#define RS_485_MSG_SEND_COMPLETE	(1 << 0)	// RS485发送完成 
#define RS_485_MSG_WAIT_SENDEND		(1 << 1)	// RS485等待发送完成 
#define RS_485_MSG_START_SEND       (1 << 2)    // RS485启动发送
#define RS_485_DELAY                (1 << 4)	// RS485等待
#endif // 
/* EOF */
