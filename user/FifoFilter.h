/*******************************************************************************
*
*   sw
*   Soft version:   
*   File Name:      FifoFilter.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:DTS 具有先进先出功能的数字滤波器 
*   Copyright (C) 
*
********************************************************************************/
#if !defined(FIFO_FILTER_H)
#define FIFO_FILTER_H_INCLUDED
#include "GLOBAL.h"

#define FIFO_BUF_LEN32	100
#define FIFO_BUF_LEN	10

#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))

// 构造 FIFO 队列
typedef struct
{
	byte fi;
	byte fo;
	byte cnt;
	word buf[FIFO_BUF_LEN];	
} dFIFO, *pFIFO;

typedef struct
{
	byte fi;
	byte fo;
	byte cnt;
	u32 buf[FIFO_BUF_LEN32];	
} dFIFO32, *pFIFO32;


extern dFIFO32 IAFilterFifo[6];
extern dFIFO32  ADFilterFifo32[2];
void FlushFifoFilter(pFIFO pQ);
void PushFifoFilter(word val, pFIFO pQ);
word PopFifoFilter(pFIFO pQ);


void FlushFifoFilter32(pFIFO32 pQ);
void PushFifoFilter32(u32 val, pFIFO32 pQ);
u32 PopFifoFilter32(pFIFO32 pQ);
#endif // !defined(FIFO_FILTER_H)

/* EOF */
