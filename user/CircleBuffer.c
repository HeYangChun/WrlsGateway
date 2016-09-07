#include "CircleBuffer.h"

static unsigned char BufferRead(tCircleBuffer *p);
static void BufferWrite(tCircleBuffer *p, unsigned char data);
static unsigned int BufferSize(tCircleBuffer *p);
static unsigned int BufferUsed(tCircleBuffer *p);
static unsigned int BufferFree(tCircleBuffer *p);
static unsigned int IsBufferEmpty(tCircleBuffer *p);
static unsigned int IsBufferFull(tCircleBuffer *p);
static void BufferFlush(tCircleBuffer *p);

void BufferInit(tCircleBuffer *p, unsigned char buf[], unsigned int size)
{
	p->size = size;
	p->pbuf = buf;
	p->readIndex = 0;
	p->writeIndex = 0;
        
        p->Read = BufferRead;
        p->Write = BufferWrite;
        p->Size = BufferSize;
        p->Used = BufferUsed;
        p->Free = BufferFree;
        p->IsEmpty = IsBufferEmpty;
        p->IsFull = IsBufferFull;
        p->Flush = BufferFlush;
}

static unsigned char BufferRead(tCircleBuffer *p)
{
	unsigned char data;
	
	data = p->pbuf[p->readIndex];
	p->readIndex++;
	if(p->readIndex == p->size)
	{p->readIndex = 0;}
	
	return data;
}

static void BufferWrite(tCircleBuffer *p, unsigned char data)
{
	p->pbuf[p->writeIndex] = data;
	p->writeIndex++;
	if(p->writeIndex == p->size)
	{p->writeIndex = 0;}
}

static unsigned int BufferSize(tCircleBuffer *p)
{
  return p->size;
}

static unsigned int BufferUsed(tCircleBuffer *p)
{
	if(p->writeIndex >= p->readIndex)
	{return (p->writeIndex - p->readIndex);}
	else
	{return (p->size - (p->readIndex - p->writeIndex));}
}

static unsigned int BufferFree(tCircleBuffer *p)
{
	return (p->size - BufferUsed(p));
}

static unsigned int IsBufferEmpty(tCircleBuffer *p)
{
	return((p->writeIndex  == p->readIndex) ? 1 : 0);
}

static unsigned int IsBufferFull(tCircleBuffer *p)
{
	unsigned int write;
	
	write = p->writeIndex + 1;
	if(write == p->size)
	{write = 0;}
	
	return ((write == p->readIndex) ? 1 : 0);
}

static void BufferFlush(tCircleBuffer *p)
{
	p->readIndex = 0;
	p->writeIndex = 0;
}
