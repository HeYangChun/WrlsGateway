/*
 * Delay.h
 *
 *  Created on: 2012-2-3
 *      Author: guximiao
 */

#ifndef DELAY_H_
#define DELAY_H_

typedef struct _delaytype
{
	unsigned char start_old;
	unsigned char timeout;
	unsigned long tick_old;
}tDelayType;

#define DELAY_DEFAULT	{0, 0, 0}

extern unsigned long tick_10ms;
extern unsigned long tick_100ms;

//****************************************************************************
//函数名称：Delay10MS
//函数说明：10ms为单位的非阻塞延时
//入口参数：dly			延时结构体
//		start		启动延时标志，上升沿：开始延时    0：关闭延时
//		tout		延时时间，单位为10ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
extern unsigned char Delay10MS(tDelayType *dly, unsigned char start, unsigned long tout);


//****************************************************************************
//函数名称：DelayQuery10MS
//函数说明：查询延时时间是否已到
//入口参数：dly			延时结构体
//		tout		延时时间，单位为10ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
extern unsigned char DelayQuery10MS(tDelayType *dly, unsigned long tout);


//****************************************************************************
//函数名称：Delay100MS
//函数说明：100ms为单位的非阻塞延时
//入口参数：dly			延时结构体
//		start		启动延时标志，上升沿：开始延时    0：关闭延时
//		tout		延时时间，单位为100ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
extern unsigned char Delay100MS(tDelayType *dly, unsigned char start, unsigned long tout);


//****************************************************************************
//函数名称：DelayQuery100MS
//函数说明：查询延时时间是否已到
//入口参数：dly			延时结构体
//		tout		延时时间，单位为10ms
//出口参数：无
//返回值：  0:定时时间未到    1:定时时间到
//****************************************************************************
extern unsigned char DelayQuery100MS(tDelayType *dly, unsigned long tout);


//****************************************************************************
//函数名称：DelayReset
//函数说明：复位延时结构体
//入口参数：dly			延时结构体的指针
//出口参数：无
//返回值：  无
//****************************************************************************
#define DelayReset(dly)	Delay10MS((dly), 0, 0)

#endif /* DELAY_H_ */
