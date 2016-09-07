#ifndef UART_H_
#define UART_H_

#include "buffer.h"
#include "GLOBAL.h"

#define UART1_PORT    GPIOA
#define UART1_RX_PIN  GPIO_PIN_4
#define UART1_TX_PIN  GPIO_PIN_5

#define UART3_PORT    GPIOD
#define UART3_RX_PIN  GPIO_PIN_6
#define UART3_TX_PIN  GPIO_PIN_5

extern void UART1_Configuration(void);
extern void UART3_Configuration(void);
extern void UART_WriteByte(u08 nUart,char data);
extern void UART_Write(u08 nUart,char *buf, u32 len);
extern u32 UART_ReadByte(u8 *data);
extern void UART_RxHandler(u8 data);
extern void UART3_RxHandler(u8 data);
u08 uartReceiveByte(u08 nUart, u08* rxData);
void uart0InitBuffers(void);
void uart2InitBuffers(void);
int uartHasNewLine(u08 nUart);
int uartReadLine(u08 nUart, char* buf, u16 maxlen);
void uartFlushReceiveBuffer(u08 nUart);
u08 uartSendBuffer(u08 nUart, char *buffer, u16 nBytes);
void uartWriteLine(u08 nUart, char* buf);
void uartPrintln(u08 nUart, char* buf);
int uartHasNewLineLF(u08 nUart);
int uartReadLineWifi(u08 nUart, char* buf, u16 maxlen);

#endif
