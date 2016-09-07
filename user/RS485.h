#ifndef _RS485_H_
#define _RS485_H_

#define RS485_PORT    GPIOA
#define RS485_TS_PIN  GPIO_PIN_6

extern void RS485_Init(void);
extern u32 RS485_ReadByte(u8 *data);
extern void RS485_Write(u8 *buf, u32 len);

#endif
