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
Description: 	����led���Ʋ�����onCycle��ledCycle��ȣ��Ҳ�Ϊ0ʱ��led������
				onCycle��ledCycle��ȣ���Ϊ0ʱ��led����
Calls: 			
Called By: 		
Input: 			onCycle -- LED��ʱ�䣬��λ10ms�����ֵ255
				ledCycle -- LED�������ڣ���λ10ms�����255
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
Description: 	led��������,10ms����һ��
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
