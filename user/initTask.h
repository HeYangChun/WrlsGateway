/*******************************************************************************
*
*   sw 
*   Soft version:   sw
*   File Name:      initTask.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/

#if !defined(__INIT_TASK_H__)
#define __INIT_TASK_H__
#include "GLOBAL.h"
#include "task.h"

enum  _MenuS
{
 MENU_PRE=0,
 MENU_TITLE,
 MENU_CONTROL,
 MENU_DEFAULT
};
enum  _FUNS
{
 FUNS_PRE=0,
 FUNS_TEST,
 FUNS_TEMP
};

extern byte  keyledtemp;
TASKDFUNC(Init)
TASK IdelTask(void);
extern void GETCONFIG(void);//上电初始化，从flash取得配置信息
TASKDFUNC(wifiMonitor);	                      //
TASKDFUNC(wifiReset);
char  Char_convert_ASCII(char data);
char  ASCII_convert_Char(char ASdata);
void GetSWSTATES(void);
//By HeYC 0822 setssidtask(void);
//By HeYC 0822	u08 setstassidtask(void);
u08 STAClienttask(void);
void get_flag(int module);
void AirControltask(void);
u08 AirControlCmd(u08 cmd,u08 state);
u08 XOR(unsigned char* puchMsg, unsigned short usDataLen);
void  IRProcess(void);
void  CheckTimeOutAfterAPPCfg(void);
void  alarmRFTask(void);
#endif	


/* EOF */

