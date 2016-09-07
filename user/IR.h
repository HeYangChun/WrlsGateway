/*******************************************************************************
*
*   
*   Soft version:   
*   File Name:      IR.h
*   Author   :      zzw
*   creation date:  20012-02-15
*   module description: ºìÍâ¿ØÖÆº¯Êý
*   Copyright (C) 
*   Others:  
********************************************************************************/
#if !defined(IR_H)
#define IR_H
#include "GLOBAL.h"

void IR_Decoding(u08 type,u08 n);
void IR_Send(u08 n);
void Record_Delete_One(u08 type,u08 n);
void Record_Delete_ALL(void);
void LongKey_Ccontrol(u08 n,u08 num);
void Save_IR(void);
//Commecnted by HeYC 0826void GetIRCode(void);
void   IRInit(void);

extern u08 readbuf[2];	
#endif //

/* EOF */
