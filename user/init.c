/*******************************************************************************
*
*   scheduler 
*   Soft version:   scheduler V1.00
*   File Name:      init.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: system initialize entry
*   Copyright (C) reserve
*
********************************************************************************/
#include "init.h"
#include "stm8s_lib.h"
#include "gpio.h"
#include "uart.h"
#include "Delay.h"
#include "ADCProcess.h"
#include "AT24C32A.h"
#include "IR.h"

u32 counter10ms=0;
u08 randtime=0;
/*******************************************************************************
Function: 		main()
Description: 	system initialize entry
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void sysInit(void)
{
  CLK_Configuration();	/* �ⲿʱ������ */
  TIM3_Configuration();   /* TIM3 ���� */
  
  GPIO_Configuration();
  UART1_Configuration();
  UART3_Configuration();
  AT24_init();
 
  IWDG_Configuration();
  IRInit();//���߲ɼ���ʼ��
  enableInterrupts();     /* ���ж� */

}

/*******************************************************************************
* Function Name  : CLK_Configuration
* Description    : Configures the different system clocks. 16MHz
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CLK_Configuration(void)
{
  CLK_DeInit();
  CLK->CKDIVR=0x00;//�ڲ����񲻷�Ƶ
  CLK_HSECmd(ENABLE);
  while(!(CLK->ECKR&0X2));  //�ȴ��ⲿʱ��rdy /*!< External Clocks Control Register */
  CLK->CKDIVR &= 0XF8;      //CPU�޷�Ƶ /*!< Clock Divider Register */
  CLK->SWR = 0XB4;          //ѡ���ⲿʱ�� /*!< ��ʱ���л��Ĵ���Clock Master Switch Register */
  while(!(CLK->SWCR&0X8));  //����Ҫ�� /*!< �л����ƼĴ���Switch Control Register */
  CLK->SWCR |=0X2;          //ʹ���ⲿʱ��  /*!< Switch Control Register */		
}

/*******************************************************************************
* Function Name  : TIMER_Configuration
* Description    : Configures the TIMER.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_Configuration(void)
{  
  TIM3_DeInit();
  TIM3_TimeBaseInit(TIM3_PRESCALER_32,500);  //0.01s-5000,500-1mS
  TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);       
  TIM3_Cmd(ENABLE);//����TIM3
}

/*******************************************************************************
* Function Name  : TIMER_Configuration
* Description    : Configures the TIMER.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Configuration(void)
{  
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_16,60000);  //1-1uS,60000--60ms
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);       
  TIM2_Cmd(ENABLE);//����TIM2
}

/*void IWDG_Configuration(void)
{ 	
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //�����Ĵ�����д
  IWDG_SetPrescaler(IWDG_Prescaler_32);	   //128Kʱ��256��Ƶ
  IWDG_SetReload(0xFF);//��������ֵ
  IWDG_Enable();	//�������Ź�
}*/

void IWDG_Configuration(void)
{ 	
  IWDG_Enable();	//�������Ź�
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //�����Ĵ�����д
  IWDG_SetPrescaler(IWDG_Prescaler_256);	   //128Kʱ��256��Ƶ
  IWDG_SetReload(0xFF);//��������ֵ
  IWDG_ReloadCounter();	
  
}

void TIME2_set(u16 time)
{
 TIM2->ARRH = (u8)(time >> 8);//1-1uS,time us
 TIM2->ARRL = (u8)(time);
 TIM2->CNTRH=0;
 TIM2->CNTRL=0;
}
/* EOF */



