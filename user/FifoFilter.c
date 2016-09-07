/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      FifoFilter.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:DTS �����Ƚ��ȳ����ܵ������˲��� 
*   Copyright (C) 
*
********************************************************************************/
#include "FifoFilter.h"

dFIFO32  ADFilterFifo32[2];
/*******************************************************************************
Function: 		FlushFifoFilter()
Description:    ���FIFO����
Calls: 			
Called By: 		
Input: 			pQ Ҫ��յĶ���
Output: 		
Return: 		  
Others: 		no
********************************************************************************/
void FlushFifoFilter(pFIFO pQ)
{
	byte i;
	
	pQ->fi  = 0;//���ݴ���
	pQ->cnt = 0;//���������ݸ���
	pQ->fo  = 0;//����ȡ��
	for (i = 0; i < FIFO_BUF_LEN; i++)
		pQ->buf[i] = 0;
}

/*******************************************************************************
Function: 		PushFifoFilter()
Description:    ��һ��ֵ����FIFO������,������ʱ�Զ����������һ����ֵ
Calls: 			
Called By: 		
Input: 			val Ҫ������е�ADֵ,pQ�����Ķ���
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
Description:    ��FIFO�����л�ȡһ��ֵ
Calls: 			
Called By: 		
Input: 			pQ�����Ķ���
Output: 		
Return: 		��ȡ��AD ֵ  
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
Description:    ���FIFO����,32λ����
Calls: 			
Called By: 		
Input: 			pQ Ҫ��յĶ���
Output: 		
Return: 		  
Others: 		no
********************************************************************************/

void FlushFifoFilter32(pFIFO32 pQ)
{
	byte i;
	
	pQ->fi  = 0;//���ݴ���
	pQ->cnt = 0;//���������ݸ���
	pQ->fo  = 0;//����ȡ��
	for (i = 0; i < FIFO_BUF_LEN32; i++)
		pQ->buf[i] = 0;
}
/*******************************************************************************
Function: 		PushFifoFilter()
Description:    ��һ��ֵ����FIFO������,������ʱ�Զ����������һ����ֵ
Calls: 			
Called By: 		
Input: 			val Ҫ������е�ADֵ,pQ�����Ķ���
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
Description:    ��FIFO�����л�ȡһ��ֵ
Calls: 			
Called By: 		
Input: 			pQ�����Ķ���
Output: 		
Return: 		��ȡ��AD ֵ  
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
