/*******************************************************************************
*
*   scheduler 
*   Soft version:   scheduler V1.00
*   File Name:      task.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: task creat and initialize and scheduler
*   Copyright (C) reserve
*
********************************************************************************/
#include "task.h"
#include "AT24C32A.h"
// TODO �������������ڱ���ļ������������������������ڵ��ļ�
#include "initTask.h"

char TaskInitFlag; // �����ʼ����־���������л�ʱ���Զ����㣬�û������ô˱�־
                   // λ��������������ʼ����������ʼ����Ϻ����û��Լ���1
                   
volatile static voidFuncPtr TASKFunc[TASK_NUM+1];
volatile static unsigned char curTaskID;
/*******************************************************************************
Function: 		sch_scheduler_init()
Description: 	task scheduler initilization,creat task
Calls: 			no
Called By: 		main()
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void sch_scheduler_init(void)
{
	unsigned char chNum;
	
	// make sure task func array was initilized
	for (chNum = 0; chNum < TASK_NUM+1; chNum++)
		TASKFunc[chNum] = 0;
	
	TASKFunc[0] = undefTask;
	curTaskID = 0;
		
	// TODO	�����û�����Լ�����������
	CreatTask(&InitTaskID, InitTask); // ������ʼ������
	
	// TODO	�����û�����Լ��������ʼ������
	// TODO	�����û������Լ�������
	ChangeTask(InitTaskID);
    
    
}

/*******************************************************************************
Function: 		sch_scheduler()
Description: 	task scheduler
Calls: 			
Called By: 		main()
Input: 			no							    
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void sch_scheduler(void)
{
	for (; ;)
	{
		// �����û�����
		if (curTaskID < TASK_NUM+1)
		{
			TASKFunc[curTaskID]();
		}
		else
			;	// ����Ƿ��ĵ���ֵ
			
		// TODO	�����û���IdelTask��������ִ��
		IdelTask();
	}
}

/*******************************************************************************
Function: 		CreatTask()
Description: 	��������
Calls: 			
Called By: 		
Input: 			TFunc -- �����ַ
Output: 		*TaskID -- �洢�û�����IDֵ
Return: 		����0 -- �������񲻳ɹ�,����1 -- ��������ɹ�
Others: 		no
********************************************************************************/
char CreatTask(unsigned char *TaskID, voidFuncPtr TFunc)
{
	unsigned char chNum;
	
	if (0 == TFunc)
		return 0;
		
	for (chNum = 1; chNum < TASK_NUM+1; chNum++)
	{
		if (0 == TASKFunc[chNum])
		{
			TASKFunc[chNum] = TFunc;
			*TaskID = chNum;
			return 1;
		}
	}
	
	return 0;
}

/*******************************************************************************
Function: 		DeleteTask()
Description: 	ɾ������
Calls: 			
Called By: 		
Input: 			*TaskID -- �û�Ҫɾ��������ID��ַ
Output: 		*TaskID -- ���û�ID��Ϊ0ֵ
Return: 		����0 -- ɾ�����񲻳ɹ�,����1 -- ɾ������ɹ�
Others: 		no
********************************************************************************/
char DeleteTask(unsigned char *TaskID)
{
	if (*TaskID < TASK_NUM+1)
	{
		if (curTaskID == *TaskID)	// ��ֹ����ɾ���Լ�
			return 0;
		else
		{
			if (0 == TASKFunc[*TaskID]) // �����Ѿ�������Ҳ����Ҫɾ��
				return 0;
			else
			{
				TASKFunc[*TaskID] = 0;	// ����������Դ
				*TaskID = 255;			// ȷ��ϵͳ���ٵ�������
				return 1;
			}
		}
	}
	else
		return 0;
}

/*******************************************************************************
Function: 		ChangeTask()
Description: 	�л�����
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
char ChangeTask(unsigned char TaskID)
{
	if (TaskID < TASK_NUM+1)
	{
		curTaskID = TaskID;
		TaskInitFlag = 0;
		return 1;
	}
	else
		return 0;
}

/*******************************************************************************
Function: 		GetCurTaskID()
Description: 	��ȡ��ǰ��������ID 
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
unsigned char GetCurTaskID(void)
{	
	return (curTaskID);
}

/*******************************************************************************
Function: 		undefTask()
Description: 	ϵͳδ�������񣬷�ֹϵͳ�Ƿ�����
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
TASK undefTask(void)
{
	
}

/* EOF */
