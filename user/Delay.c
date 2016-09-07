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
//�������ƣ�Delay10MS
//����˵����10msΪ��λ�ķ�������ʱ
//��ڲ�����dly			��ʱ�ṹ��
//		start		������ʱ��־�������أ���ʼ��ʱ    0���ر���ʱ
//		tout		��ʱʱ�䣬��λΪ10ms
//���ڲ�������
//����ֵ��  0:��ʱʱ��δ��    1:��ʱʱ�䵽
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
//�������ƣ�DelayQuery10MS
//����˵������ѯ��ʱʱ���Ƿ��ѵ�
//��ڲ�����dly			��ʱ�ṹ��
//		tout		��ʱʱ�䣬��λΪ10ms
//���ڲ�������
//����ֵ��  0:��ʱʱ��δ��    1:��ʱʱ�䵽
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
//�������ƣ�Delay100MS
//����˵����100msΪ��λ�ķ�������ʱ
//��ڲ�����dly			��ʱ�ṹ��
//		start		������ʱ��־�������أ���ʼ��ʱ    0���ر���ʱ
//		tout		��ʱʱ�䣬��λΪ100ms
//���ڲ�������
//����ֵ��  0:��ʱʱ��δ��    1:��ʱʱ�䵽
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
//�������ƣ�DelayQuery100MS
//����˵������ѯ��ʱʱ���Ƿ��ѵ�
//��ڲ�����dly			��ʱ�ṹ��
//		tout		��ʱʱ�䣬��λΪ10ms
//���ڲ�������
//����ֵ��  0:��ʱʱ��δ��    1:��ʱʱ�䵽
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
