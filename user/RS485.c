#include "stm8s_lib.h"
#include "uart.h"
#include "RS485.h"

#define RS485_TX_EN()   GPIO_WriteHigh(RS485_PORT, RS485_TS_PIN)
#define RS485_TX_DIS()  GPIO_WriteLow(RS485_PORT, RS485_TS_PIN)

void RS485_Init(void)
{
  GPIO_Init(RS485_PORT, RS485_TS_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  RS485_TX_DIS();
  
  UART_Configuration();
}

u32 RS485_ReadByte(u8 *data)
{
  return UART_ReadByte(data);
}

void RS485_Write(u8 *buf, u32 len)
{
  RS485_TX_EN();
  UART_Write(buf, len);
  RS485_TX_DIS();
}
