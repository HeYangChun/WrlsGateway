/*
 * Delay.c
 *
 *  Created on: 2012-2-3
 *      Author: guximiao
 */

#include "Delay.h"

unsigned long tick_10ms = 0;
unsigned long tick_100ms = 0;

//****************************************************************************
//函数名称：Delay10MS
//函数说明：10ms为单位的非阻塞延时
//入口参数：dly			延时结构体
//		start		启动延时标志，上升沿：开始延时    0：关闭延时
//		tout		延时时间，单位为10ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
unsigned char Delay10MS(tDelayType *dly, unsigned char start, unsigned long tout)
{
	if((dly->start_old == 0) && (start != 0))
	{
		dly->tick_old = tick_10ms;
	}
	if(start != 0)
	{
		if((tick_10ms - dly->tick_old) >= tout)
		{dly->timeout = 1;}
		else
		{dly->timeout = 0;}
	}
	else
	{dly->timeout = 0;}

	dly->start_old = start;
        
	return dly->timeout;
}

//****************************************************************************
//函数名称：DelayQuery10MS
//函数说明：查询延时时间是否已到
//入口参数：dly			延时结构体
//		tout		延时时间，单位为10ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
unsigned char DelayQuery10MS(tDelayType *dly, unsigned long tout)
{
	unsigned char timeout;
	
	if(dly->start_old != 0)
	{
		if((tick_10ms - dly->tick_old) >= tout)
		{timeout = 1;}
		else
		{timeout = 0;}
	}
	else
	{timeout = 0;}
        
	return timeout;
}

//****************************************************************************
//函数名称：Delay100MS
//函数说明：100ms为单位的非阻塞延时
//入口参数：dly			延时结构体
//		start		启动延时标志，上升沿：开始延时    0：关闭延时
//		tout		延时时间，单位为100ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
unsigned char Delay100MS(tDelayType *dly, unsigned char start, unsigned long tout)
{
	if((dly->start_old == 0) && (start != 0))
	{
		dly->tick_old = tick_100ms;
	}
	if(start != 0)
	{
		if((tick_100ms - dly->tick_old) >= tout)
		{dly->timeout = 1;}
		else
		{dly->timeout = 0;}
	}
	else
	{dly->timeout = 0;}

	dly->start_old = start;
        
	return dly->timeout;
}

//****************************************************************************
//函数名称：DelayQuery100MS
//函数说明：查询延时时间是否已到
//入口参数：dly			延时结构体
//		tout		延时时间，单位为10ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
unsigned char DelayQuery100MS(tDelayType *dly, unsigned long tout)
{
	unsigned char timeout;
	
	if(dly->start_old != 0)
	{
		if((tick_10ms - dly->tick_old) >= tout)
		{timeout = 1;}
		else
		{timeout = 0;}
	}
	else
	{timeout = 0;}
        
	return timeout;
}
