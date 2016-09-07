/*******************************************************************************
*
*   Timeout 
*   Soft version:   V1.00
*   File Name:      sTimeout.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#include "sTimeout.h"

/*******************************************************************************
Function: 		sTimeout()
Description:    Check for timeout using the given tick counter and timeout value
				Force timeout if timeout value is zero
Calls: 			
Called By: 		
Input: 			*varp -- the given tick counter, tout -- Force timeout
Output: 		BOOL
Return: 		return 1 when timeout  
Others: 		no
********************************************************************************/
BOOL sTimeout(u32 *varp, u32 tout)
{
    u32 diff;

    diff = tickcount - *varp;
    if (!tout || diff >= tout)
    {
        *varp = tickcount;
        return 1;
    }  
    return 0; 
//	
}
/*******************************************************************************
* Function Name  : NOP(void)
* Description    : —” ±µ»¥˝”√
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  NOP(void)
{
 u32 i;
 for(i=0;i<10;i++);
}
/* EOF */ 
