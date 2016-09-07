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
// TODO 如果你的任务定义在别的文件里，请在这里引入你的任务所在的文件
#include "initTask.h"

char TaskInitFlag; // 任务初始化标志，在任务切换时被自动清零，用户可以用此标志
                   // 位完成任务的启动初始化工作。初始化完毕后由用户自己置1
                   
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
		
	// TODO	这里用户添加自己创建任务工作
	CreatTask(&InitTaskID, InitTask); // 创建初始化任务
	
	// TODO	这里用户添加自己的任务初始化工作
	// TODO	这里用户启动自己的任务
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
		// 调度用户任务
		if (curTaskID < TASK_NUM+1)
		{
			TASKFunc[curTaskID]();
		}
		else
			;	// 处理非法的调度值
			
		// TODO	这里用户把IdelTask放在这里执行
		IdelTask();
	}
}

/*******************************************************************************
Function: 		CreatTask()
Description: 	创建任务
Calls: 			
Called By: 		
Input: 			TFunc -- 任务地址
Output: 		*TaskID -- 存储用户任务ID值
Return: 		返回0 -- 创建任务不成功,返回1 -- 创建任务成功
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
Description: 	删除任务
Calls: 			
Called By: 		
Input: 			*TaskID -- 用户要删除的任务ID地址
Output: 		*TaskID -- 将用户ID置为0值
Return: 		返回0 -- 删除任务不成功,返回1 -- 删除任务成功
Others: 		no
********************************************************************************/
char DeleteTask(unsigned char *TaskID)
{
	if (*TaskID < TASK_NUM+1)
	{
		if (curTaskID == *TaskID)	// 禁止任务删除自己
			return 0;
		else
		{
			if (0 == TASKFunc[*TaskID]) // 任务已经不存在也不需要删除
				return 0;
			else
			{
				TASKFunc[*TaskID] = 0;	// 回收任务资源
				*TaskID = 255;			// 确定系统不再调用任务
				return 1;
			}
		}
	}
	else
		return 0;
}

/*******************************************************************************
Function: 		ChangeTask()
Description: 	切换任务
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
Description: 	获取当前运行任务ID 
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
Description: 	系统未定义任务，防止系统非法运行
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
