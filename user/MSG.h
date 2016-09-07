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
// ��Ϣ������ID����
//


/////////////////////////////////////////////////////////
//
// ϵͳ��Ϣ����
//
extern volatile byte 	SYSTEM_MSG;	// ϵͳ��Ϣ����

#define SYSTEM_MSG_NONE				(0 << 0) // ����Ϣ
#define SYSTEM_MSG_READY			(1 << 0) // ϵͳ׼������
#define SYSTEM_MSG_ADCEND			(1 << 1) // AD Convent completed
#define SYSTEM_MSG_TURNOFF		    (1 << 2) // ȷ���ػ��ź�
#define SYSTEM_MSG_SENDOFF			(1 << 3) // ���͹ػ��ź�
#define SYSTEM_MSG_CTUS				(1 << 4) // δ������������

/////////////////////////////////////////////////////////
//
// RS485��Ϣ����
//
extern volatile byte 	RS_485_MSG;	// RS485 ��Ϣ����

#define RS_485_MSG_SEND_COMPLETE	(1 << 0)	// RS485������� 
#define RS_485_MSG_WAIT_SENDEND		(1 << 1)	// RS485�ȴ�������� 
#define RS_485_MSG_START_SEND       (1 << 2)    // RS485��������
#define RS_485_DELAY                (1 << 4)	// RS485�ȴ�
#endif // 
/* EOF */
