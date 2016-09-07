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
#define TASK_NUM	5	// ����ϵͳ������������254

#define __KERNEL_VERSION		"V1.02"

extern char TaskInitFlag; // �����ʼ����־���������л�ʱ���Զ����㣬�û������ô˱�־
                          // λ��������������ʼ����������ʼ����Ϻ����û��Լ���1
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
