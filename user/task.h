/*******************************************************************************
*
*   scheduler 
*   Soft version:   scheduler V1.00
*   File Name:      task.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: task creat and initialize and scheduler declare
*   Copyright (C) reserve
*
********************************************************************************/
#if !defined(__TASK_H__)
#define __TASK_H__

typedef void			TASK;
typedef unsigned char* 	bp;
typedef void 			(*voidFuncPtr)(void);
typedef void			(*voidProcPtr)(bp bpv);
#define TASK_NUM	5	// 定义系统任务个数，最大254

#define __KERNEL_VERSION		"V1.02"

extern char TaskInitFlag; // 任务初始化标志，在任务切换时被自动清零，用户可以用此标志
                          // 位完成任务的启动初始化工作。初始化完毕后由用户自己置1
#ifndef NULL
#define NULL 0
#endif

#define TASKCFUNC(A) unsigned char A##TaskID;\
    TASK A##Task(void)

#define TASKDFUNC(A) extern unsigned char A##TaskID;\
    void A##Task(void);

void sch_scheduler_init(void);
void sch_scheduler(void);
TASK undefTask(void);
char CreatTask(unsigned char *TaskID, voidFuncPtr TFunc);
char ChangeTask(unsigned char TaskID);
char DeleteTask(unsigned char *TaskID);
unsigned char GetCurTaskID(void);

#endif
/* EOF */
