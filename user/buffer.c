/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      buffer.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-25
*   module description:环形缓冲区。
*   Copyright (C) 
*
********************************************************************************/

#include "buffer.h"

/*#ifndef CRITICAL_SECTION_START
#define CRITICAL_SECTION_START	unsigned char _sreg = SREG; cli()  //关中断，保护期间的程序不被打断
#define CRITICAL_SECTION_END	SREG = _sreg                       //开中断
#endif */

//#ifndef CRITICAL_SECTION_START
//#define CRITICAL_SECTION_START	asm("sim")                 //关中断，保护期间的程序不被打断
//#define CRITICAL_SECTION_END	asm("rim")               //开中断
//#endif 



// global variables

// initialization

void bufferInit(cBuffer* buffer, unsigned char *start, unsigned short size)
{
	// begin critical section
	//CRITICAL_SECTION_START;
	// set start pointer of the buffer
	buffer->dataptr = start;
	buffer->size = size;
	// initialize index and length
	buffer->dataindex = 0;
	buffer->datalength = 0;
	// end critical section
	//CRITICAL_SECTION_END;
}

// access routines
unsigned char  bufferGetFromFront(cBuffer* buffer)
{
	unsigned char data = 0;
	// begin critical section
	//CRITICAL_SECTION_START;
	// check to see if there's data in the buffer
	if(buffer->datalength)
	{
		// get the first character from buffer
		data = buffer->dataptr[buffer->dataindex];
		// move index down and decrement length
		buffer->dataindex++;
		if(buffer->dataindex >= buffer->size)
		{
			buffer->dataindex -= buffer->size;
		}
		buffer->datalength--;
	}
	// end critical section
	//CRITICAL_SECTION_END;
	// return
	return data;
}

void bufferDumpFromFront(cBuffer* buffer, unsigned short numbytes)
{
	// begin critical section
	//CRITICAL_SECTION_START;
	// dump numbytes from the front of the buffer
	// are we dumping less than the entire buffer?
	if(numbytes < buffer->datalength)
	{
		// move index down by numbytes and decrement length by numbytes
		buffer->dataindex += numbytes;
		if(buffer->dataindex >= buffer->size)
		{
			buffer->dataindex -= buffer->size;
		}
		buffer->datalength -= numbytes;
	}
	else
	{
		// flush the whole buffer
		buffer->datalength = 0;
	}
	// end critical section
	//CRITICAL_SECTION_END;
}

unsigned char bufferGetAtIndex(cBuffer* buffer, unsigned short index)
{
	// begin critical section
	unsigned char data;
	//CRITICAL_SECTION_START;
	// return character at index in buffer
	data = buffer->dataptr[(buffer->dataindex+index)%(buffer->size)];
	// end critical section
	//CRITICAL_SECTION_END;
	return data;
}

unsigned char bufferAddToEnd(cBuffer* buffer, unsigned char data)
{
	// begin critical section
	//CRITICAL_SECTION_START;
	// make sure the buffer has room
	if(buffer->datalength < buffer->size)
	{
		// save data byte at end of buffer
		buffer->dataptr[(buffer->dataindex + buffer->datalength) % buffer->size] = data;
		// increment the length
		buffer->datalength++;
		// end critical section
//		CRITICAL_SECTION_END;
		// return success
		return 1;
	}
	// end critical section
	//CRITICAL_SECTION_END;
	// return failure
	return 0;
}

unsigned short bufferIsNotFull(cBuffer* buffer)
{
	unsigned short bytesleft;
	// begin critical section
	//CRITICAL_SECTION_START;
	// check to see if the buffer has room
	// return true if there is room
	bytesleft = (buffer->size - buffer->datalength);
	// end critical section
	//CRITICAL_SECTION_END;
	return bytesleft;
}

void bufferFlush(cBuffer* buffer)
{
	// begin critical section
	//CRITICAL_SECTION_START;
	// flush contents of the buffer
	buffer->datalength = 0;
	// end critical section
	//CRITICAL_SECTION_END;
}

/* EOF */ 
