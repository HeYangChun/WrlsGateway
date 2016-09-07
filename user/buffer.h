/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      buffer.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-25
*   module description:RS232м╗педё©И 
*   Copyright (C) 
*
********************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#ifndef cli
	#define cli()			__asm__ __volatile__ ("cli" ::)
#endif
#ifndef sei
	#define sei()			__asm__ __volatile__ ("sei" ::)
#endif
// structure/typdefs

//! cBuffer structure
typedef struct struct_cBuffer
{
	unsigned char *dataptr;			///< the physical memory address where the buffer is stored
	unsigned short size;			///< the allocated size of the buffer
	unsigned short datalength;		///< the length of the data currently in the buffer
	unsigned short dataindex;		///< the index into the buffer where the data starts
} cBuffer;

// function prototypes

//! initialize a buffer to start at a given address and have given size
void			bufferInit(cBuffer* buffer, unsigned char *start, unsigned short size);

//! get the first byte from the front of the buffer
unsigned char	bufferGetFromFront(cBuffer* buffer);

//! dump (discard) the first numbytes from the front of the buffer
void bufferDumpFromFront(cBuffer* buffer, unsigned short numbytes);

//! get a byte at the specified index in the buffer (kind of like array access)
// ** note: this does not remove the byte that was read from the buffer
unsigned char	bufferGetAtIndex(cBuffer* buffer, unsigned short index);

//! add a byte to the end of the buffer
unsigned char	bufferAddToEnd(cBuffer* buffer, unsigned char data);

//! check if the buffer is full/not full (returns zero value if full)
unsigned short	bufferIsNotFull(cBuffer* buffer);

//! flush (clear) the contents of the buffer
void			bufferFlush(cBuffer* buffer);

#endif
//@}
