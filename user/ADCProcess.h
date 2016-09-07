/*******************************************************************************
*
*   sw 
*   Soft version:   V1.00
*   File Name:      ADCOrocess.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#ifndef ADCProcess_h
#define ADCProcess_h

#include "GLOBAL.h"



extern void ADCProcInit(void);
extern word GetADCValue(void);
extern void ADCProcess(void);
extern void ConventADToReal(void);
#endif
