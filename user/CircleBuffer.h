#ifndef _CIRCLEBUFFER_H_
#define _CIRCLEBUFFER_H_

typedef struct _cyclebuffer
{
	unsigned int size;
	unsigned int readIndex;
	unsigned int writeIndex;
	unsigned char * pbuf;
        
        unsigned char (*Read)(struct _cyclebuffer *p);
        void (*Write)(struct _cyclebuffer *p, unsigned char data);
        unsigned int (*Size)(struct _cyclebuffer *p);
        unsigned int (*Used)(struct _cyclebuffer *p);
        unsigned int (*Free)(struct _cyclebuffer *p);
        unsigned int (*IsEmpty)(struct _cyclebuffer *p);
        unsigned int (*IsFull)(struct _cyclebuffer *p);
        void (*Flush)(struct _cyclebuffer *p);
}tCircleBuffer;

extern void BufferInit(tCircleBuffer *p, unsigned char buf[], unsigned int size);

#endif
