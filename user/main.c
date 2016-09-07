/******************** (C) COPYRIGHT 2010 SHBST ********************
* File Name          : main.c
* Author             : 
* Version            : V1.0
* Date               : 2012/04/01
* Description        : Main program body
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s_lib.h"
#include "init.h"
#include "task.h"

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
******************************************************READ_KEY*************************/
int main(void)
{
#ifdef DEBUG
    debug();
#endif
    
	/* just refer to cpu,target board,memory,extern device ect initialization */
	sysInit();
	
	/* complete user task initialization   */
	/* start the first task */
	sch_scheduler_init();
	
	/* user task running & task scheduler  */
	sch_scheduler();
}



#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  int test=0;
/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	/* Infinite loop */
	while (1)
	{
          test++;
	}
}
#endif
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
