/*******************************************************************************
*
*   Application 
*   Soft version:   V1.00
*   File Name:      APP.c
*   Author   :      zzw
*   creation date:  2012-02-19
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#include "APP.h"
#include "initTask.h"
ledTimer_t ledTimer;

/*******************************************************************************
Function: 		ledSetParam()
Description: 	设置led控制参数，onCycle和ledCycle相等，且不为0时，led常亮；
				onCycle和ledCycle相等，且为0时，led常灭；
Calls: 			
Called By: 		
Input: 			onCycle -- LED亮时间，单位10ms，最大值255
				ledCycle -- LED控制周期，单位10ms，最大255
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void ledSetParam(u08 onCycle, u08 ledCycle)
{
	if (onCycle <= ledCycle)
	{
		ledTimer.ticket = 0;
		ledTimer.period = ledCycle;
		ledTimer.ledOnTime = onCycle;
		if (onCycle == ledCycle)
		{
			if (onCycle == 0)
			{
				RunLedOff();
				//BeepOff();
			}
			else
			{
				RunLedOn();
				//BeepOn();
			}
		}
	}
}

/*******************************************************************************
Function: 		ledCtrlProc()
Description: 	led控制例程,10ms运行一次
Calls: 			
Called By: 		
Input: 			
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void ledCtrlProc(void)
{
	if (ledTimer.ledOnTime < ledTimer.period)
	{
		ledTimer.ticket++;
		if (ledTimer.ticket == ledTimer.ledOnTime)
		{
			// led off
			RunLedOff();
		   // BeepOff();
		}
		else if (ledTimer.ticket >= ledTimer.period)
		{
			// led on
			RunLedOn();
		 //   BeepOn();
			ledTimer.ticket = 0;
		}
	}
}


/* EOF */
