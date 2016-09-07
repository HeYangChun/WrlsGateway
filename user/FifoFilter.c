/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      FifoFilter.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:DTS 具有先进先出功能的数字滤波器 
*   Copyright (C) 
*
********************************************************************************/
#include "FifoFilter.h"

dFIFO32  ADFilterFifo32[2];
/*******************************************************************************
Function: 		FlushFifoFilter()
Description:    清空FIFO队列
Calls: 			
Called By: 		
Input: 			pQ 要清空的队列
Output: 		
Return: 		  
Others: 		no
********************************************************************************/
void FlushFifoFilter(pFIFO pQ)
{
	byte i;
	
	pQ->fi  = 0;//数据存入
	pQ->cnt = 0;//队列中数据个数
	pQ->fo  = 0;//数据取出
	for (i = 0; i < FIFO_BUF_LEN; i++)
		pQ->buf[i] = 0;
}

/*******************************************************************************
Function: 		PushFifoFilter()
Description:    将一个值放入FIFO队列中,队列满时自动丢弃最早的一个数值
Calls: 			
Called By: 		
Input: 			val 要存入队列的AD值,pQ操作的队列
Output: 		
Return: 		  
Others: 		no
********************************************************************************/
void PushFifoFilter(word val, pFIFO pQ)
{	
	pQ->buf[pQ->fi] = val;
	pQ->fi = (pQ->fi + 1)%FIFO_BUF_LEN;
	
	if (++pQ->cnt > FIFO_BUF_LEN)
	{
		pQ->cnt = FIFO_BUF_LEN;
		pQ->fo = (pQ->fo + 1)%FIFO_BUF_LEN;
	}
}

/*******************************************************************************
Function: 		PopFifoFilter()
Description:    从FIFO队列中获取一个值
Calls: 			
Called By: 		
Input: 			pQ操作的队列
Output: 		
Return: 		获取的AD 值  
Others: 		no
********************************************************************************/
word PopFifoFilter(pFIFO pQ)
{
	word tmpBuf[FIFO_BUF_LEN];
	long tmp;
	byte i;
	word min, max;
	
	if (pQ->cnt < 3 || FIFO_BUF_LEN < 3)
		return (pQ->buf[(pQ->fi + FIFO_BUF_LEN - 1)%FIFO_BUF_LEN]);
	
	min = 0xffff;
	max = 0x0;
	tmp = 0;
	for (i = 0; i < pQ->cnt; i++)
	{
		tmpBuf[i] = pQ->buf[(pQ->fo+i)%FIFO_BUF_LEN];
		min = MIN(min,tmpBuf[i]);
		max = MAX(max,tmpBuf[i]);
		tmp += tmpBuf[i];
	}
	
	return (word)((tmp-max-min)/(pQ->cnt-2));		
}


/*******************************************************************************
Function: 		FlushFifoFilter32()
Description:    清空FIFO队列,32位数据
Calls: 			
Called By: 		
Input: 			pQ 要清空的队列
Output: 		
Return: 		  
Others: 		no
********************************************************************************/

void FlushFifoFilter32(pFIFO32 pQ)
{
	byte i;
	
	pQ->fi  = 0;//数据存入
	pQ->cnt = 0;//队列中数据个数
	pQ->fo  = 0;//数据取出
	for (i = 0; i < FIFO_BUF_LEN32; i++)
		pQ->buf[i] = 0;
}
/*******************************************************************************
Function: 		PushFifoFilter()
Description:    将一个值放入FIFO队列中,队列满时自动丢弃最早的一个数值
Calls: 			
Called By: 		
Input: 			val 要存入队列的AD值,pQ操作的队列
Output: 		
Return: 		  
Others: 		no
********************************************************************************/
void PushFifoFilter32(u32 val, pFIFO32 pQ)
{	
	pQ->buf[pQ->fi] = val;
	pQ->fi = (pQ->fi + 1)%FIFO_BUF_LEN32;
	
	if (++pQ->cnt > FIFO_BUF_LEN32)
	{
		pQ->cnt = FIFO_BUF_LEN32;
		pQ->fo = (pQ->fo + 1)%FIFO_BUF_LEN32;
	}
}

/*******************************************************************************
Function: 		PopFifoFilter()
Description:    从FIFO队列中获取一个值
Calls: 			
Called By: 		
Input: 			pQ操作的队列
Output: 		
Return: 		获取的AD 值  
Others: 		no
********************************************************************************/
u32 PopFifoFilter32(pFIFO32 pQ)
{
	u32 tmpBuf[FIFO_BUF_LEN32];
	u32 tmp;
	byte i;
	u32 min, max;
	
	if (pQ->cnt < 3 || FIFO_BUF_LEN32 < 3)
		return (pQ->buf[(pQ->fi + FIFO_BUF_LEN32 - 1)%FIFO_BUF_LEN32]);
	
	min = 0xffffffff;
	max = 0x0;
	tmp = 0;
	for (i = 0; i < pQ->cnt; i++)
	{
		tmpBuf[i] = pQ->buf[(pQ->fo+i)%FIFO_BUF_LEN32];
		min = MIN(min,tmpBuf[i]);
		max = MAX(max,tmpBuf[i]);
		tmp += tmpBuf[i];
	}
	
	return (u32)((tmp-max-min)/(pQ->cnt-2));		
}
/* EOF */
