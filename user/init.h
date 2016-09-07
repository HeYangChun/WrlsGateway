/*******************************************************************************
*
*   scheduler 
*   Soft version:   scheduler V1.00
*   File Name:      init.h
*   Author   :      zhaofei ( zhaozhiyuan@163.net )
*   creation date:  2006-05-17
*	module description: system initilize entry declare
*   Copyright (C) reserve
*
********************************************************************************/
#if !defined(__INIT_H__)
#define __INIT_H__
#include "GLOBAL.h"
extern u08 randtime;
void sysInit(void);
void CLK_Configuration(void);
void TIM3_Configuration(void);
void TIM2_Configuration(void);
void IWDG_Configuration(void);
void sysInit(void);
void TIME2_set(u16 time);
#endif
/* EOF */
