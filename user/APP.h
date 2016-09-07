/*******************************************************************************
*
*   Application 
*   Soft version:   V1.00
*   File Name:      APP.h
*   Author   :      zzw
*   creation date:  2012-02-19
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#if !defined(APP_H)
#define APP_H
#include "GLOBAL.h"
#include "gpio.h"
typedef struct __LED_TIMER
{
	u08 ticket;
	u08 period;
	u08 ledOnTime;
} ledTimer_t;


void ledSetParam(u08 onCycle, u08 ledCycle);
void ledCtrlProc(void);

#endif // 
/* EOF */
