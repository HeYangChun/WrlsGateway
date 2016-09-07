/*******************************************************************************
*
*   
*   Soft version:   
*   File Name:      AT24C32A.h
*   Author   :      
*   creation date:  20011-08-15
*   module description:EEPROM¿ØÖÆº¯Êý
*   Copyright (C) 
*
********************************************************************************/
#include "GLOBAL.h"
#if !defined(AT24C32A_H)
#define AT24C32A_H



void AT24_init(void);
void AT24_I2C_START(void);
void AT24_I2C_STOP(void);
void AT24_I2C_RecvACK(void);
void AT24_I2C_SendACK(u08 ack);
void AT24_I2C_Wdata(u08 data);
u08 AT24_I2C_Rdata(void);
void AT24_Write_byte(u16 addr,u08 data);
u08 AT24_Read_byte(u16 addr);
void AT24_Write_page(u16 addr,u08 *buf,u08 len);
void AT24__Read_Page(u16 addr,u08 *buf,u08 len);
#endif //

/* EOF */
