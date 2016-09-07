/*******************************************************************************
*
*   
*   Soft version:   
*   File Name:      IR.c
*   Author   :      zzw
*   creation date:  20012-02-15
*   module description: ������ƺ���
*   Copyright (C) 
*   Others: ����ң���������֡��Ъ��Ⱦ�Ϊ10 ms ����,��ʼ��ĸߵ�ƽ��Ϊ5 ms ����,ͨ��Ϊ9 ms ���ҡ�����λ��10 ��s��5 ms֮��  
********************************************************************************/
#include "IR.h"
#include "stdio.h"
#include "COMM.h"
#include "buffer.h"
#include "string.h"
#include "GLOBAL.h"
#include <stdlib.h> 
#include "uart.h"
#include "sTimeout.h"
#include "gpio.h"
#include "APP.h"
#include "initTask.h"
#include "init.h"
#include "AT24C32A.h"

byte IRsendbuf[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u08 format[1]={0x00};
u08 readbuf[2];	//flash��ȡ��2�ֽ�����
//u08 cmd2[600];	//��ʱ��׼Ϊ1usʱ�������16λ�洢���������������
/*******************************************************************************
Function: 		IR_Decoding()
Description: 	       RF433���뺯��
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void   IR_Decoding(u08 type,u08 n)
{
  u32 i=0;
 
 // IRInit();	 //������ʼ��
  EXTI_UN();//�ر��ⲿ�ж�
  IR.Rcounter=0;
  IR.rfRcvTimer=0;
  if(rfs.type==1)
  {
    IR.REVFLAG=RF_TYPE_315;  //315
  }
  else if(rfs.type==2)
  {
    IR.REVFLAG=RF_TYPE_433;  //433
  }
  IR.DECODESTEP=IR_GUIDE;
  IR.guide=0;
  IR.PulseN=0;
  IR.Learnflag=0;
  IR.NO=n;//��ǰָ���
  memset(eeprombuf,0,270);
  for(i=0;i<256;i++)  //��״̬
  {
   IR.cmd[i]=0;
  }
  Record_Delete_One(type,n);	//��ʽ��flash
  rfs.number=n;
  EXTI_EN();//���ⲿ�ж�
}


/*******************************************************************************
Function: 		IR_Send()
Description: 	RF���ͺ���
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void   IR_Send(u08 n)
{
 //��flash��ȡ��������
  int i;
  int len;
  memset(eeprombuf,0,270);
  for(i=0;i<256;i++)  //��״̬
  {
   IR.cmd[i]=0;
  }
  switch (rfs.type)
  {
    case 1://rf315
       disableInterrupts();  //��֮ǰ�ȹ��ж�
       AT24__Read_Page((RF315_PRE_ADDR+OffsetAddr*n),readbuf,2);
       enableInterrupts();  //���������ж�
       break;
    case 2://rf433
       disableInterrupts();  //��֮ǰ�ȹ��ж�  
       AT24__Read_Page((RF433_PRE_ADDR+OffsetAddr*n),readbuf,2);
       if((readbuf[0]==0x55)&&(readbuf[1]==0xaa))
       {
         AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*n),readbuf,2);
       }
       enableInterrupts();  //���������ж�
       break;
    default:
       break;
  }
  IR.cmd[0]=readbuf[0]*256+readbuf[1];
  len=IR.cmd[0]+1;
  disableInterrupts();  //��֮ǰ�ȹ��ж�
  /*for(i=1;i<len;i++)
  {
      AT24__Read_Page((RF433_PRE_ADDR+OffsetAddr*n+i*2),eeprombuf,2);
      IR.cmd[i]=readbuf[0]*256+readbuf[1];
      for(int j=0;j<1000;j++) {;}  // ��ʱ
  }*/
  if(rfs.type==2)   //433M
  {
    if(len*2<=128)
    {
      AT24__Read_Page((RF433_PRE_ADDR+OffsetAddr*n),eeprombuf,len*2);
    }
    else if(len*2>128)
    { 
      if(len*2<=256)
      {
        AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*n),eeprombuf,128); 
        IWDG_ReloadCounter();			// �߿��Ź�
     // for(dword i=0;i<500;i++){}
        AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*n+128),&(eeprombuf[128]),len*2-128); 
      }
      else if(len*2>256)
      {
        AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*n),eeprombuf,128); 
        IWDG_ReloadCounter();			// �߿��Ź�
        // for(dword i=0;i<500;i++){}
        AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*n+128),&(eeprombuf[128]),128); 
        IWDG_ReloadCounter();			// �߿��Ź�
        // for(dword i=0;i<500;i++){}
        AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*n+256),&(eeprombuf[256]),len*2-256);
      }
    }
  }
  else if(rfs.type==1) //315M
  {
    if(len*2<=128)
    {
      AT24__Read_Page((RF315_PRE_ADDR+OffsetAddr*n),eeprombuf,len*2);
    }
    else if(len*2>128)
    {
      if(len*2<=256)
      {
        AT24__Read_Page((RF315_PRE_ADDR_SPL+OffsetAddr*n),eeprombuf,128); 
        IWDG_ReloadCounter();			// �߿��Ź�
     // for(dword i=0;i<500;i++){}
        AT24__Read_Page((RF315_PRE_ADDR_SPL+OffsetAddr*n+128),&(eeprombuf[128]),len*2-128); 
      }
      else if(len*2>256)
      {
        AT24__Read_Page((RF315_PRE_ADDR_SPL+OffsetAddr*n),eeprombuf,128); 
        IWDG_ReloadCounter();			// �߿��Ź�
        // for(dword i=0;i<500;i++){}
        AT24__Read_Page((RF315_PRE_ADDR_SPL+OffsetAddr*n+128),&(eeprombuf[128]),128); 
        IWDG_ReloadCounter();			// �߿��Ź�
        // for(dword i=0;i<500;i++){}
        AT24__Read_Page((RF315_PRE_ADDR_SPL+OffsetAddr*n+256),&(eeprombuf[256]),len*2-256);
      }
    }
    
  }
  for(i=1;i<len;i++)
  {
      IR.cmd[i]=eeprombuf[i*2]*256+eeprombuf[i*2+1];
  }
  enableInterrupts();  //���������ж�
  rfs.replyCounter=0;
  if(rfs.type==2)   //433M
  {
    rfs.sendflag=2;
  }
  else if(rfs.type==1)   //315M
  {
    rfs.sendflag=1;
  }
  
  TIME2_set(1000);//1ms���������� 
  rfs.PulseN=0;
  rfs.PulseLEN=IR.cmd[0];///��ָ����������
  rfs.SENDSTEP=0;
  rfs.state=0;
	
}
/*******************************************************************************
Function: 		Record_Delete_One()
Description: 	���ⰴťָ���������
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void   Record_Delete_One(u08 type,u08 n)
{
  int i=0;
  
  switch (type)
  {
    case 1://rf315
       disableInterrupts();  //д֮ǰ�ȹ��ж�
       for(i=0;i<OffsetAddr;i++)
       {
        AT24_Write_byte((RF315_PRE_ADDR+OffsetAddr*n+i),0x00);
       }
       enableInterrupts();  //д�������ж�
       break;
    case 2://rf433
       disableInterrupts();  //д֮ǰ�ȹ��ж�
       for(i=0;i<OffsetAddr;i++)
       {
        AT24_Write_byte((RF433_PRE_ADDR+OffsetAddr*n+i),0x00);
       }
       enableInterrupts();  //д�������ж�
       break;
    default:
       break;
  
  }
  

  //write_flash_array((config_addr+(4*n)),4,format);	//��ʽ��ָ��Ŷ�Ӧ��ַ
  //IR.ADDR_QUEUE[n]=0xffffffff;
  
}
/*******************************************************************************
Function: 		Record_Delete_ALL()
Description: 	RF��ťȫ����������
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void   Record_Delete_ALL(void)
{
  u32 i=0;
  for(i=0;i<256;i++)
  {
   	//write_flash_array((config_addr+(4*i)),4,format);	//��ʽ��ָ��Ŷ�Ӧ��ַ
	//By HeYC 0831	IR.ADDR_QUEUE[i]=0xffffffff;
  }


}

/*******************************************************************************
Function: 		GetIRCode(void)
Description: 	RF���ʹ���
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
/*Bgn:Commented by HeYC because not used.
void   GetIRCode(void)
{
   u08  IRNUM;
   if((CodeTail!=CodeHead)&&(IR.SEDFLAG==0)) //���ͻ�������Ϊ�գ��Ҳ��ڷ��ͽ׶�
    {
	  if (++CodeTail >= IRCODELENGTH) 
			CodeTail = 0;
	  IRNUM=CodeBuf[CodeTail];
	  IR_Send(IRNUM); //RFָ���
	}


}
//End:Commented By HeYC because not used */
/*******************************************************************************
Function: 		IRInit(void)
Description: 	�����ʼ��
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void   IRInit(void)
{
  TIM2_Configuration();   /* TIM2 ���� */;
  EXTI_Configuration();
 // GETCONFIG();

}

/* EOF */
