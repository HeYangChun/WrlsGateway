/*******************************************************************************
*
*   sw 6
*   Soft version:   sw
*   File Name:      initTask.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
/*
*
* �汾������־��
* -�汾-               -��������-               -˵��-
*
* coffeeM-1.00        2006-08-25             �����汾
* 
*
********************************************************************************/
#include "task.h"
#include "init.h"
#include "initTask.h"
#include "GLOBAL.h"
#include "gpio.h"
#include "sTimeout.h"
#include "APP.h"
#include "string.h"
#include "stdio.h"
#include "COMM.h"
#include "wifi.h"
#include "buffer.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include "IR.h"
#include "AT24C32A.h"
//#include "ADCProcess.h"


u32 SystemTimer;
//By HeYC 0907	u32 wifiRetDelayTimer;
u32 wifiRestartTimer;
u32 irDelayTimer;

//byte ledTimer;
//By HeYC 0907	word wKeyTime;
char lastkey;
char key;
//By HeYC 0907	int wifiCounter=0;
//By HeYC 0907	uchar wifiReset=0;
uchar wifi_monitor_step=0;
//By HeYC 0907	u16 lenths;

u08 KEYPRESS_Counter;
u08 KEYPRESS_DelayFlg;
u08 KEYPRESS_Long;

//By HeYC 0907	u08 rets;
//By HeYC 0907	u08  TEMP_cmd[2];	//
u08 almBrdcstFlag=0;
volatile byte secModChangedFlag=0;//He from u08 to volatile byte
u08 LED_staflag=0;
u08 LED_stpflag=0;

/*******************************************************************************
Function: 		IdelTask()
Description: 	ϵͳ�����������ϵͳʵʱ��������
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
u08 connectRmtSvrResult=0;

void ResponseToDevFind();

TASK IdelTask(void)
{
	u32 i;
   
	GetSWSTATES();

 
 /***************************************************************************************************************************************************
				 10ms����
 ***************************************************************************************************************************************************/
 		 
	if(sys1msFlag)	
	{
   		sys1msFlag=0;
   		AirControltask();//����оƬ�������񣬴���3

		//He:WifiBusy never be se to WIFI_DATA_BUSY
   		//He:if(((WifiBusy==WIFI_IDEL)||(WifiBusy==WIFI_DATA_BUSY))&&(wifiEnable==WIFI_STATE_INIT_FINISH))
   		//HeYC 0907	if( 	(WifiBusy==WIFI_IDEL) &&
		//HeYC 0907		(wifiATCmdSetStat==WIFI_AT_CMD_OVER))//By HeYC
		if( (wifiATCmdSetStat==WIFI_AT_CMD_OVER) )//By HeYC, WifiBusy is a strange variable
   		{
     			Auto_WIFI_Send();//��������
   		}
   
   		//HeYC 0907if(	( (WifiBusy==WIFI_IDEL)||(WifiBusy==WIFI_AT_BUSY) )&&
		//HeYC 0907	( wifiATCmdSetStat==WIFI_AT_CMD_OVER))
		if( (wifiATCmdSetStat==WIFI_AT_CMD_OVER) )//By HeYC, WifiBusy is a strange variable
   		{
     			if(flagBgnConnectSvr)//20������һ��
     			{
        			connectRmtSvrResult=ConnectToRmtSvrInSTAMode();
        			if (connectRmtSvrResult!= WIFI_NONE)//�����Ʒ�����
        			{
          				if( connectRmtSvrResult!=WIFI_OK)//HeYC:from 5 to 2
            				{
	            				if(++ClientCounterA>=2)
						{
		              			//By HeYC0820 Authenticationflag=0;//�ر�����
		              			//By HeYC0820 HFlagWhereAuthSetZero=1;
		              			wifi_monitor_step=WIFI_RESTART_PRE;
			      				wifiATCmdSetStat=WIFI_AT_CMD_NOT_BGN;//By HeYC 0822
		              			ChangeTask(wifiResetTaskID); // ��������ģ������
		              			ClientCounterA=0;
		              			flagConnetedWithSvr=1;//ֹͣ���ӷ�����
	            				}
					}  
            
            				flagBgnConnectSvr=0;
        			}
      				// if (wifiCmdCIPSTART(staClientIP,staClientPort,1)!= WIFI_NONE)//�����Ʒ�����
        			//{
        			// staClientConnentStart=0;
       			//  staClientConnent=1;
       			// }
     			}
   		}
  		 /*:Bgn:Commented by HeYC
   		if(((WifiBusy==WIFI_IDEL)||(WifiBusy==WIFI_AP_BUSY))&&(wifiEnable==WIFI_STATE_INIT_FINISH))
   		{
    			if(ATCWSAPEnable)
    			{
      				if(setssidtask()!= WIFI_NONE)
      				{
       				ATCWSAPEnable=0;
      				}
    			}
   		}
   
   		if(((WifiBusy==WIFI_IDEL)||(WifiBusy==WIFI_STA_BUSY))&&(wifiEnable==WIFI_STATE_INIT_FINISH))
   		{
     			if(setSSid)
     			{
      				if(setstassidtask()!= WIFI_NONE)
      				{
       				setSSid=0;
       				strcpy(UDPPort,"59999");
       				wifiMDInitflag=0;
       				hartconnectflag=0;
       				//wifi_monitor_step=WIFI_RESTART_PRE;
      					// ChangeTask(wifiResetTaskID); // ��������ģ������
       				ChangeTask(wifiMonitorTaskID);//����wifi��ʼ������
      				}
     			}
  		}
   		//End:commented by HeYC*/
  		if((sys10msFlag)) // 10ms
  		{	 
			sys10msFlag = 0;
			ledCtrlProc();
			commTask();
    			IRProcess();  
			//By HeYC 0906 move it into 500ms cycle    CheckTimeOutAfterAPPCfg();
    			IWDG_ReloadCounter();			// �߿��Ź�
    			if (++sysTickfor100ms >= 10) // 100ms
			{
          			if(LED_staflag)  
          			{
            				if(++LED_stpflag>=20)
            				{
              				LED_staflag=0;
              				LED_stpflag=0;
              				ledSetParam(50,100);//ָʾ�ƻظ�������˸
              
            				}
          			}
	   			sysTickfor100ms = 0;
				
       			alarmRFTask();//100ms���һ���Ƿ��б�������
       
       			if(secModChangedFlag==1 || secModChangedFlag==2)  ////����״̬�㲥
       			{
       				//HeYC: broadcast security state changed
         				for(i=0;i<10;i++)
         				{
           					sbuffer[i]=GetKZCurSetOrUnSet_answer[i];
         				}
         				for(i=10;i<16;i++)
         				{
            					sbuffer[i]=macidHEX[i-10];
         				}
         				// strcat(&sbuffer[10],macidHEX);
         				sbuffer[16] = curSecuModeShadow;//By HeYC: from SetOrUnsetState[0] to curSecuModeShadow
         				strcat(&sbuffer[17],RCR_ETX);
					//  strcat(&sbuffer[20],"\r\n");
         				lenth=20;//strlen(sbuffer);
         				SMSSEND(lenth,sbuffer,1);   //�㲥
					//By HeYC         SetOrUnsetflag=0;
	 				secModChangedFlag--;//By HeYC
       			}
	   			if((KEYPRESS_DelayFlg)&&(KEYPRESS_Long==0))//����������
           			{
            				if(++KEYPRESS_Counter>=30)//����3��
            				{
             					KEYPRESS_Long=1;
             					KEYPRESS_Counter=0;
            					// KEYPRESS_DelayFlg=0;
            
             					ledSetParam(10,20);//������˸��������ģʽ
            				}
           			}
	  			/// <!-- 500mS��ʱ��
	  			if (++sysTickfor500ms > 5)	//500mS
     				// if (++sysTickfor500ms > 30)	//3S ������
	  			{
	   				sysTickfor500ms=0;


					CheckTimeOutAfterAPPCfg();//By HeYC

					if(flagReplyDevFind>0){
						flagReplyDevFind--;

						ResponseToDevFind();
					}
        
        				if(almBrdcstFlag>0)
        				{
          					for(i=0;i<10;i++)     
          					{
            						sbuffer[i]=KZProbeAlarm[i];
          					}
          					for(i=10;i<16;i++)
          					{
            						sbuffer[i]=macidHEX[i-10];
          					}
           					//strcat(&sbuffer[10],macidHEX);
          					sbuffer[16] = alarm.NO>>8;//keyValH 
          					sbuffer[17] = alarm.NO&0xff;//keyValL 
          					strcat(&sbuffer[18],RCR_ETX);
          					// strcat(&sbuffer[16],"\r\n");
          					lenth=21;//strlen(sbuffer);
          					SMSSEND(lenth,sbuffer,1);    //�㲥������Ϣ
          					almBrdcstFlag--;
        				}
        
        				if(secModChangedFlag==3)//����״̬�ı��ϱ�������
        				{
        					//HeYC: send stat to server
          					for(i=0;i<10;i++)
          					{
             						sbuffer[i]=GetCurSetOrUnSet_answer[i];
          					}
          					for(i=10;i<16;i++)
          					{
            						sbuffer[i]=macidHEX[i-10];
          					}
          					//strcat(&sbuffer[10],macidHEX);
          					sbuffer[16] = curSecuModeShadow;//By HeYC: from SetOrUnsetState[0] to curSecuModeShadow
          					strcat(&sbuffer[17],RCR_ETX);
          					// strcat(&sbuffer[20],"\r\n");
          					lenth=20;//strlen(sbuffer);
          					SMSSEND(lenth,sbuffer,4);  //���͸�������
						//By HeYC          SetOrUnsetflag=2;
						secModChangedFlag--;//By HeYC

						//ByHeYC 0822		if(SetOrUnsetState[0]!=curSecuModeShadow){
						//ByHeYC 0822			SetOrUnsetState[0]=curSecuModeShadow;
						//ByHeYC 0822			write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);
						//ByHeYC 0822		}
        				}
       
					if (uartwifiTimer > 0)
					{
		  				uartwifiTimer--;
					}
             
               
                 			//By HeYC0820	if(Authenticationflag)//8S   ����
                 			//By HeYC 0820	{
                   			if(++sysTickfor8s>=30)// 30*500ms=15S (in fact , 19.x sec)
                   			{
                   				sysTickfor8s=0;
						
                   				if(Authenticationflag)//HeYC:0820
						{//By HeYC 0820
	                     			ClientCounterA=0;  //����������������־
	                     			//sysTickfor8s=0;
	                     			for(int i=0;i<13;i++)
	                     			{
	                        				sbuffer[i]=Heartbeat[i];
	                     			}
	                     			lenth=13;//strlen(sbuffer);
	                     			SMSSEND(lenth,sbuffer,4);//���͸�������
						}//By HeYC 0820


						//He:check if no heart beat cnt exceed max
						if(++Heartbeattimer5s_count>=5)//By HeYC from 3 to 5
                     			{ 
                       				Authenticationflag=0;//�ر�����
//By HeYC 0907	                       				HFlagWhereAuthSetZero=2;
                       				Heartbeattimer5s_count=0;
                        				wifi_monitor_step=WIFI_RESTART_PRE;
                       				// ChangeTask(wifiResetTaskID); // ��������ģ������
                       				flagConnetedWithSvr=0; //���������Ʒ���������
                        
                     			}
                   			}
                 			//By HeYC0820}
                   
                   			//5s�ж�һ�ΰ���״̬�ı��
					/*By HeYC
                			if(++sysTickfor6s>=20)//6S
                 			{
                   				sysTickfor6s=0;
                   				Read_Flash(SW_SETorUNSETSTATE,SetOrUnsetState,1); 
                   				if(curSecuModeShadow!=SetOrUnsetState[0])
                   				{
                     				SetOrUnsetflag=1;
                   				}
                   				curSecuModeShadow=SetOrUnsetState[0];
                 			}
					//End:By HeYC */

					//He:Check if connected with tcp server
                			if(flagConnetedWithSvr==0)
                			{
                 				if((++timerOfReConnectSvr>=15)&&(wifiATCmdSetStat==WIFI_AT_CMD_OVER)&&(flagBgnConnectSvr==0))//60������һ��(120)
                 				{
							//By HeYC                  Authenticationflag=0;//�ر�����
							//By HeYC                  HFlagWhereAuthSetZero=3;
                  					flagBgnConnectSvr=1;
                  					timerOfReConnectSvr=0;
                  					statOfConnectToRmtSvr=CON_RMT_SVR_STAT_READY;
                  
                 				}
                			}
                   
                			//By HeYC 0907	if(wifiReset)
                			//By HeYC 0907	{
                 			//By HeYC 0907		if(++wifiCounter>=2)//������ʱ2������³�ʼ��WIFIģ��
                 			//By HeYC 0907		{
                  			//By HeYC 0907			wifiReset=0;
                  			//By HeYC 0907			wifiATCmdSetStat=WIFI_STATE_POWER_ON;//HeYC: from 1 to WIFI_STATE_POWER_ON 
                  			//By HeYC 0907			uartFlushReceiveBuffer(COM_WIFI);
                  			//By HeYC 0907			ledSetParam(10,30);
                  			//By HeYC 0907			ChangeTask(wifiMonitorTaskID);//�����շ�����
                 			//By HeYC 0907		}
                			//By HeYC 0907	}
	  			}
			
			
        		}


			//Bgn:By heYC 0822
			if(curSecuMode!=curSecuModeShadow){
				
				curSecuMode=curSecuModeShadow;
				
				if(curSecuMode==0){
					//unset, clear all alm source no
					memset(&alarm.almSrcNoCache[0],	INVALID_ALM_SRC_NO,		MAX_ALM_SENSOR_NUM);
				}
				
				write_flash_array(SW_SETorUNSETSTATE,sizeof(curSecuMode),(u08 *)&curSecuMode);
			}
			//End:By HeYC 0822
   		}	
 	}
}

/*******************************************************************************
Function: 		InitTask()
Description: 	ϵͳӦ����ڣ�ʵ���û�����Ĵ�������ʼ��ϵͳ״̬
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
TASKCFUNC(Init)
{
	if (!TaskInitFlag)
	{
		sys10msFlag	  = 0;        /* 10ms��־ 				*/
		TaskInitFlag = 1;
		sTimeout(&SystemTimer, 0);
                ledSetParam(10,30);
	}
	
	if (sTimeout(&SystemTimer, 200)) // ������ʱ2��,�ȴ������ʼ�����
	{
		 
               // sysInit();//�ȶ�ϵͳ
		CreatTask(&wifiMonitorTaskID, wifiMonitorTask);//WiFi��ʼ������
                //wifiReset
                CreatTask(&wifiResetTaskID, wifiResetTask);//wifi��������
               
		GETCONFIG();  //��ȡĬ�����ò���
//By HeYC 0907			wifiReset=1;
		wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
              uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
              
                ClientID=0;
                WaitMONITOR_IDELSend=0;
                WIFI_Hail=0;
                WIFI_Head=0;
		//////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//CANsenden=0x01;          //CAN����ʹ��
		//StartADC_DMA();    //����ADC�ɼ�
		//ChangeTask(0); // ��������
		ChangeTask(wifiMonitorTaskID);//����wifi��ʼ������
                
	}
}

/*******************************************************************************
Function: 		wifiMonitorTask()
Description: 	        WIFIģ���ʼ������
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/

TASKCFUNC(wifiMonitor)
{
  char ret=WIFI_NONE;
   if ((ret = wifiMDInit()) != WIFI_NONE)
   {
	if (ret == WIFI_ERR) // ģ���ʼ������
	{
	  wifi_monitor_step=WIFI_RESTART_PRE;
          ChangeTask(wifiResetTaskID); // ��������ģ������
          
	}
	else
	{
//By HeYC0907           if(wifiATCmdSetStat)
          {
           wifiATCmdSetStat=WIFI_AT_CMD_OVER;//HeYC: from 2 to WIFI_STATE_INIT_FINISH
           //By HeYC 0907	strcpy(sbuffer,"?GTIME**");
           //By HeYC 0907	strcat(sbuffer,"\r\n");	
           //By HeYC 0907	lenths=strlen(sbuffer);
          // SMSSEND(lenths,sbuffer,ClientID);//���͸��ֻ�
          // SMSSEND(lenths,sbuffer,4);//���͸�������
          }
		  
          //By HeYC 0923	ledSetParam(50,100);
	   uartFlushReceiveBuffer(COM_WIFI); //By HeYC
	   ChangeTask(0); // 
	   
          if(cfgWifiForAppCfg)
          {
           flagConnetedWithSvr=1; //�ر������Ʒ���������   
          }
          else if(cfgWifiForAppCfg==0)
          {
          ledSetParam(50,100);//Toggle led to normal heat beat frequency
            flagConnetedWithSvr=0; //���������Ʒ��������� 
          }
		  cfgWifiForAppCfg=0;//By HeYC 0923
	}
   } 
}
/*******************************************************************************
Function: 		wifiResetTask()
Description: 	
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
TASKCFUNC(wifiReset)
{
   
   switch (wifi_monitor_step)
   {
         case WIFI_RESTART_PRE:
	      wifi_monitor_step++;        
	      break;
	 case WIFI_RESTART_POWERDOWM:
	       WIFI_POWER_OFF();
	       wifi_monitor_step++; //
               sTimeout(&wifiRestartTimer, 0);
	       break;
	 case WIFI_RESTART_WAIT:
	       if (sTimeout(&wifiRestartTimer, 500))//5s
	       {
		  wifi_monitor_step++; 
	       }
		  break;
	 case WIFI_RESTART_POWERUP:
	          WIFI_POWER_ON();
	          wifi_monitor_step++; 
	          sTimeout(&wifiRestartTimer, 0); 
		  break;
	 case WIFI_RESTART_WAITRUN:
	          if (sTimeout(&wifiRestartTimer, 500))//3s
		  {
		   wifi_monitor_step++; 
                   
		  }
                  break;
	 case WIFI_RESTART_END:
   		  wifi_monitor_step=WIFI_RESTART_PRE;
                 // strcpy(sbuffer,"?GTIME**");
                //  strcat(sbuffer,"\r\n");	
                //  lenths=strlen(sbuffer);
                 // SMSSEND(lenths,sbuffer,ClientID);
                 // SMSSEND(lenths,sbuffer,4);//���͸�������

		    wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
                  uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
                  ChangeTask(wifiMonitorTaskID);//����wifi��ʼ������
   		  break;
         default:
	         WIFI_POWER_ON();
	         wifi_monitor_step=WIFI_RESTART_PRE;

		   wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
                  uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
                 ChangeTask(wifiMonitorTaskID);//����wifi��ʼ������
   		 break;
   
   }
}

//Bgn:By HeYC 0906
void UpdateMacIDHex(){
	int i=0;
	u08 mH,mL;
	for(i=0;i<12;i+=2){

		// "1A"
		//High 4 bits  "1"->1
		if(gCfgPara.macAddr[i]>='a')
		{
			mH=gCfgPara.macAddr[i]-'a'+10;
		}
		else if(gCfgPara.macAddr[i]>='A')
		{
			mH=gCfgPara.macAddr[i]-'A'+10;
		}
		else
		{
			mH=gCfgPara.macAddr[i]-'0';
		}

		//low 4 bits	"A"->0x0a
		if(gCfgPara.macAddr[i+1]>='a')
		{
			mL=gCfgPara.macAddr[i+1]-'a'+10;
		}
		else if(gCfgPara.macAddr[i+1]>='A')
		{
			mL=gCfgPara.macAddr[i+1]-'A'+10;
		}
		else
		{
			mL=gCfgPara.macAddr[i+1]-'0';
		}

		macidHEX[i/2]=mH<<4|mL;
	}

	//Reverse order
	for(i=0;i<3;i++){
		
		mH=macidHEX[i];

		macidHEX[i]=macidHEX[5-i];
		macidHEX[5-i]=mH;
	}

}
//By HeYC 0906
//**************************************************************************************************
//*�������ƣ�GETCONFIG()
//*����������������ʼ��
//*��ڲ�����void
//*���ڲ�����void
//*			 
//**************************************************************************************************
void GETCONFIG(void)
{
//By HeYC 0906     u32 i,j,t=0;
//By HeYC 0906     u16 lenth;
//By HeYC 0906     int temp1,temp2;
//By HeYC 0906     char temp3;


	//Bgn:By HeYC Init gCfgPara
	memset(&gCfgPara,0,sizeof(gCfgPara));
	Read_Flash(SW_ID_ADDR,(u08*)&gCfgPara,sizeof(gCfgPara));

	//check valid
	if(strcmp(gCfgPara.magicStr,"topan")!=0){
		//failed, format flash

		//magic string
		strcpy(gCfgPara.magicStr,"topan");
		//default mac addr
		strcpy(gCfgPara.macAddr,"1afe34a1584a");
		//Server address and port
		strcpy(gCfgPara.serverAddr,"topansh.wicp.net");
		strcpy(gCfgPara.serverPort,"7269");		
		//SSID and PWD
		strcpy(gCfgPara.SSIDToJoin,"andy");
		strcpy(gCfgPara.SSIDPwd,"andyandy");
	}

	UpdateMacIDHex();
	//End:By HeYC 


	Read_Flash(SW_SETorUNSETSTATE,(u08*)&curSecuMode,sizeof(curSecuMode));
	if(curSecuMode!=0 && curSecuMode!=1 && curSecuMode!=2){
		//unprogrammed in flash yet
		curSecuMode=0;
	}
	curSecuModeShadow=curSecuMode;
     //����ʹ��
//By HeYC 0906	          memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     Read_Flash(ENBALE_KEY_ADDR,flashbuf,1);
//By HeYC 0906	     if(flashbuf[0]==0)//
//By HeYC 0906	     {
//By HeYC 0906	       ENBALE_KEY=1;//Ĭ�ϱ��ذ�����Ч
//By HeYC 0906	     }
//By HeYC 0906	     else
//By HeYC 0906	     {
//By HeYC 0906	       ENBALE_KEY=flashbuf[0];
//By HeYC 0906	     }
     //macid
    // strcpy(macid,"1afe34a1584a");//test mac
    // write_flash_array(SW_ID_ADDR,13,macid);//test mac
//By HeYC 0906	     	memset(macid,0,20);
//By HeYC 0906	          	memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	          	Read_Flash(SW_ID_ADDR,flashbuf,12);
//By HeYC 0906	          	if(flashbuf[0]==0)//
//By HeYC 0906	          	{
//By HeYC 0906	            	strcpy(macid,"1afe34a1584a");//test mac
//By HeYC 0906	          	}
//By HeYC 0906	          	else
//By HeYC 0906	          	{
//By HeYC 0906	            	for(i=0;i<12;i++)
//By HeYC 0906	            	{
//By HeYC 0906	            		macid[i]=flashbuf[i];
//By HeYC 0906	            	}
//By HeYC 0906	     	}
//By HeYC 0906	           	for(int i=0;macid[i]!='\0';i=i+2) 
//By HeYC 0906	            { 
//By HeYC 0906	            	if(macid[i]>='a') 
//By HeYC 0906	                 		temp1=macid[i]-'a'+10; 
//By HeYC 0906	             	else 
//By HeYC 0906	                 		temp1=macid[i]-'0'; 
//By HeYC 0906	     
//By HeYC 0906	     		if(macid[i+1]>='a') 
//By HeYC 0906	                 		temp2=macid[i+1]-'a'+10; 
//By HeYC 0906	             	else 
//By HeYC 0906	                 		temp2=macid[i+1]-'0';
//By HeYC 0906	             
//By HeYC 0906	             	macidHEX[t]=(temp1<<4)+temp2; 
//By HeYC 0906	             	t++;
//By HeYC 0906	     	}
	//reverse the order of macidHEX
//By HeYC 0906	          	t=0;
//By HeYC 0906	          	temp3=macidHEX[0];
//By HeYC 0906	          	macidHEX[0]=macidHEX[5];
//By HeYC 0906	          	macidHEX[5]=temp3;
     
//By HeYC 0906	          	temp3=macidHEX[1];
//By HeYC 0906	          	macidHEX[1]=macidHEX[4];
//By HeYC 0906	          	macidHEX[4]=temp3;
     
//By HeYC 0906	          	temp3=macidHEX[2];
//By HeYC 0906	          	macidHEX[2]=macidHEX[3];
//By HeYC 0906	          	macidHEX[3]=temp3;
     
    	//pin
//By HeYC 0906	      memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	      Read_Flash(SW_PIN_ADDR,flashbuf,3);
//By HeYC 0906	      if(flashbuf[0]==0)//
//By HeYC 0906	      {
//By HeYC 0906	       macpin=123;
       
//By HeYC 0906	      }
//By HeYC 0906	      else
//By HeYC 0906	      {
//By HeYC 0906	       macpin=flashbuf[0]*100+flashbuf[1]*10+flashbuf[2];
//By HeYC 0906	      }
      
//By HeYC 0906	     memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     Read_Flash(SW_DELAY_TIME_ADDR,flashbuf,2);
//By HeYC 0906	     if((flashbuf[0]==0)&&(flashbuf[1]==0))//
//By HeYC 0906	     {
//By HeYC 0906	       delayOffTime=0;//Ĭ�ϲ���ʱ
//By HeYC 0906	     }
//By HeYC 0906	     else
//By HeYC 0906	     {
//By HeYC 0906	       delayOffTime=(u16)(flashbuf[0]<<8)+flashbuf[1];
//By HeYC 0906	     }
    
//By HeYC 0906		memset(staClientIP,0,20);
//By HeYC 0906	     	memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     	Read_Flash(SW_SERVICE_IP_ADDR,flashbuf,20);
//By HeYC 0906	    	// strcpy(staClientIP,"192.168.42.6");//test 122.245.111.254
//By HeYC 0906	     	if(flashbuf[0]==0)//
//By HeYC 0906	     	{
//By HeYC 0906	       	strcpy(staClientIP,"topansh.wicp.net");//test 122.245.111.254
//By HeYC 0906	     	}
//By HeYC 0906	     	else
//By HeYC 0906	     	{
//By HeYC 0906	       	lenth=strlen(flashbuf);
//By HeYC 0906	      
//By HeYC 0906	       	for(i=0;i<lenth;i++)
//By HeYC 0906	       	{
//By HeYC 0906	        		staClientIP[i]=flashbuf[i];
//By HeYC 0906	       	}
//By HeYC 0906	     	}
     
//By HeYC 0906	     	memset(staClientPort,0,10);
//By HeYC 0906	     	memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     	Read_Flash(SW_SERVICE_PORT_ADDR,flashbuf,10);
//By HeYC 0906	     	if(flashbuf[0]==0)//
//By HeYC 0906	     	{
//By HeYC 0906	       	strcpy(staClientPort,"7269");//test 
//By HeYC 0906	     	}
//By HeYC 0906	     	else
//By HeYC 0906	     	{
//By HeYC 0906	       	lenth=strlen(flashbuf);
//By HeYC 0906	       	for(i=0;i<lenth;i++)
//By HeYC 0906	       	{
//By HeYC 0906	        		staClientPort[i]=flashbuf[i];
//By HeYC 0906	       	}
//By HeYC 0906	     	}
     
//By HeYC 0906	     	Read_Flash(SW_SETorUNSETSTATE,SetOrUnsetState,1);  
//By HeYC 0906	     	curSecuModeShadow=SetOrUnsetState[0];
//By HeYC 0906	     
//By HeYC 0906	     	memset(ssid,0,20);
//By HeYC 0906	     	memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     	Read_Flash(SW_SSID_ADDR,flashbuf,20);
//By HeYC 0906	     	lenth=strlen(flashbuf);
//By HeYC 0906	     	for(i=0;i<lenth;i++)
//By HeYC 0906	     	{
//By HeYC 0906	       	ssid[i]=flashbuf[i];
//By HeYC 0906	     	}
     
//By HeYC 0906	     memset(ssidPassword,0,20);
//By HeYC 0906	     memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     Read_Flash(SW_SSIDPASSWORD_ADDR,flashbuf,20);
//By HeYC 0906	     lenth=strlen(flashbuf);
//By HeYC 0906	     for(i=0;i<lenth;i++)
//By HeYC 0906	     {
//By HeYC 0906	        ssidPassword[i]=flashbuf[i];
//By HeYC 0906	     }
    // strcpy(ssid,"HiWiFi_T2");//test 
     //strcpy(ssidPassword,"hhzn2012");//test 
     
     
   
}
/*******************************************************************************
Function: 	    Char_convert_ASCII(char data)
Description: 	����ת�ַ�
Calls: 			
Called By: 		no
Input: 			����
Output: 		�ַ�
Return: 		no
Others: 		no
********************************************************************************/
//By HeYC 0907	char  Char_convert_ASCII(char data)
//By HeYC 0907	{
//By HeYC 0907	    char ASdata;
//By HeYC 0907	    if(data<=0x09)
//By HeYC 0907	    {
//By HeYC 0907	     ASdata=data+'0';
//By HeYC 0907	    }
//By HeYC 0907	    else
//By HeYC 0907	    {
//By HeYC 0907	     ASdata=data-10+'A';
//By HeYC 0907	    }
//By HeYC 0907	   return ASdata;
//By HeYC 0907	}
/*******************************************************************************
Function: 	    ASCII_convert_Char(char data)
Description: 	�ַ�ת����
Calls: 			
Called By: 		no
Input: 			�ַ�
Output: 		����
Return: 		no
Others: 		no
********************************************************************************/
//By HeYC 0907	char  ASCII_convert_Char(char ASdata)
//By HeYC 0907	{
//By HeYC 0907	    char data;
//By HeYC 0907	
//By HeYC 0907		if ((ASdata <= '9') && (ASdata >= '0'))
//By HeYC 0907			data=ASdata - '0';
//By HeYC 0907		else 
//By HeYC 0907			data=ASdata - 'A' + 10;
//By HeYC 0907	
//By HeYC 0907	   return data;
//By HeYC 0907	}



void GetSWSTATES(void)
{
  
   int i,couter;;
   
   couter=0;
   key=READ_KEY;
   if(key!=lastkey)   //��ⰴ������
   {
      for(i=0;i<1000;i++)
      {
         if(READ_KEY==key)
         {
           couter++;
         }
      }
      if(couter>950)//��������
      {
        lastkey=key;
        if(key==0)
        {      
          KEYPRESS_DelayFlg=1;//��������
          KEYPRESS_Long=0;
          KEYPRESS_Counter=0;     
        }     
      }
    
   }
   if(KEYPRESS_DelayFlg)//��ⰴ���ͷ�
   {
      key=READ_KEY;
      if(key)
      {
        couter=0;
        for(i=0;i<1000;i++)
        {
          if(READ_KEY==key)//�����ͷ�
          {
            couter++;
          }
        }
        if(couter>950)//��������
        {
          lastkey=key;
          Authenticationflag=0;//�ر�����
//By HeYC 0907	          HFlagWhereAuthSetZero=4;
          flagConnetedWithSvr=1;
          cfgWifiForAppCfg=1;
          KEYPRESS_DelayFlg=0;
          wifiMDInitflag=1; //��ʼ��ģʽ��־
          strcpy(UDPPort,"40001");
         // wifi_monitor_step=WIFI_RESTART_PRE;
         // ChangeTask(wifiResetTaskID); // ��������ģ������
         wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
         uartFlushReceiveBuffer(COM_WIFI); //By HeYC
          ChangeTask(wifiMonitorTaskID);//����wifi��ʼ������
         // IR_Decoding(2,1);//TEST  ����3������ָʾ�ƿ�����˸������ѧϰѧϰRF����//�ĵ��������ǰ�ߵ�2����433��1��315����ߵ�1�ĳɶ�Ӧ�ļ�ֵ
         // sTimeout(&TimeOut, 0);  //10Sѧϰʱ�䣬��ʱ�˳�
        }
      }
   }
   

}
//////
/*******************************************************************************
Function: 	    setssidtask(void)
Description: 	    WIFIģ��ssid���ã�wifi���ƺ����룩
Calls: 			
Called By: 		no
Input: 			�ַ�
Output: 		����
Return: 		no
Others: 		no
********************************************************************************/
/*Bgn:By HeYC 
u08 setssidtask(void)
{  
 rets=WIFI_NONE;
 u08 ret=WIFI_NONE;
 
 switch (apsetSSid)
 {
    case WIFI_AP_SET:
        WifiBusy=WIFI_AP_BUSY;
        rets=wifiCmdCWSAP(apssid,apssidPassword,3);//
        if (rets != WIFI_NONE)   //
        {
           //����ģ���SSID
           if(rets==WIFI_OK)
           {
            apsetSSid=WIFI_AP_SET_WAIT;//
            sTimeout(&wifiRetDelayTimer, 0);
           }
           else
           {
            apsetSSid=WIFI_AP_ERROR;
           }
    
         }
         break;
     
    case WIFI_AP_SET_WAIT:
          if (sTimeout(&wifiRetDelayTimer, 300))//3s
          {
            apsetSSid=WIFI_AP_RST;
          } 
         break;
    case WIFI_AP_RST:
         sTimeout(&wifiRetDelayTimer, 0);
         apsetSSid=WIFI_AP_WAIT;//
         //rets=wifiCmdRST(1);
         //if (rets != WIFI_NONE)   //
         //{
            //�޸���AP��SSID
           //if(rets==WIFI_OK)
           //{
            //apsetSSid=WIFI_AP_WAIT;//
            //sTimeout(&wifiRetDelayTimer, 0);
           //}
           //else
           //{
            //apsetSSid=WIFI_AP_PRE;
           //}
          break;
     case WIFI_AP_WAIT:
          if (sTimeout(&wifiRetDelayTimer, 100))//1s
          {
            ret=WIFI_OK;
            apsetSSid=WIFI_AP_PRE;
            WifiBusy=WIFI_IDEL;
            wifiEnable=WIFI_STATE_PRE;
            wifi_monitor_step=WIFI_RESTART_PRE;
            ChangeTask(wifiResetTaskID);//����wifi��ʼ������
          }
          break;
     case WIFI_AP_ERROR:
            apsetSSid=WIFI_AP_PRE;
            WifiBusy=WIFI_IDEL;
            ret=WIFI_ERR;
            break;
     default:
            apsetSSid=WIFI_AP_PRE;
            WifiBusy=WIFI_IDEL;
            ret=WIFI_ERR;
             break;
 
 
  }
 
 return ret;
}
//End:Commented by HeYC*/
/*******************************************************************************
Function: 	    setstassidtask(void)
Description: 	    WIFIģ��stassid����
Calls: 			
Called By: 		no
Input: 			�ַ�
Output: 		����
Return: 		no
Others: 		no
********************************************************************************/
/*Bgn:By HeYC 
u08 setstassidtask(void)
{  
 rets=WIFI_NONE;
 u08 ret=WIFI_NONE;
 
   WifiBusy=WIFI_STA_BUSY;//ģ�鿪ʼ�������ݣ��ڼ䲻�ɴ��
   if ((rets=wifiCmdCWJAP(ssid,ssidPassword,1)) != WIFI_NONE)   //
   {
     //���ӵ�·������SSID
     WifiBusy=WIFI_IDEL;//
     if(rets==WIFI_OK)
     {
      staClientConnentStart=1;//׼�������Ʒ�����
      ret=WIFI_OK;
     }
     else 
     {
      ret=WIFI_ERR;//����ʧ��
     }
       
   }

   return ret;
}
//End:Commented by HeYC*/

/*******************************************************************************
Function: 	    STAClienttask(void)
Description: 	    ����Զ�̷�����
Calls: 			
Called By: 		no
Input: 			no
Output: 		staClientConnent
Return: 		no
Others: 		no
********************************************************************************/
u08 ConnectToRmtSvrInSTAMode(void)
{
	u08 wret=WIFI_NONE;
        u08 ret=WIFI_NONE;
        
	switch (statOfConnectToRmtSvr)
	{
		case CON_RMT_SVR_STAT_READY:
			statOfConnectToRmtSvr++;
//By HeYC 0907	                        WifiBusy=WIFI_AT_BUSY;//ģ�鿪ʼ�������ݣ��ڼ䲻�ɴ��
			break;
		case CON_RMT_SVR_STAT_INIT_CIFSR:
			if (wifiCmdCIFSR(1) != WIFI_NONE)   //
			{
                         if(STAEnable)//������·����
                          {
                           
                           statOfConnectToRmtSvr++;
                           sTimeout(&wifiSendTimer, 0);
                          }
                          else
                          {
                           statOfConnectToRmtSvr = CON_RMT_SVR_STAT_ERROR;
                          }
                        }
			break;
                case CON_RMT_SVR_STAT_CIPSERVER:
                     if (sTimeout(&wifiSendTimer, 200))  //5s�ȴ��������
                     {
                       wret=wifiCmdCIPSTART(gCfgPara.serverAddr,gCfgPara.serverPort,1);
                       
                       if (wret != WIFI_NONE)//
			            {
                           statOfConnectToRmtSvr = CON_RMT_SVR_STAT_READY;
//By HeYC 0907	                           WifiBusy=WIFI_IDEL;
                           ret=WIFI_OK;
                           if(wret==WIFI_OK)
                           {
                            flagConnetedWithSvr=1;
                           }
                           else
                           {
                              
                             flagConnetedWithSvr=0;
                             timerOfReConnectSvr=0;
                           }
                        }
                     }
                       break;
		case CON_RMT_SVR_STAT_ERROR:
//By HeYC 0907				WifiBusy=WIFI_IDEL;
			statOfConnectToRmtSvr = CON_RMT_SVR_STAT_READY;
                        ret=WIFI_ERR;
			break;
		default:
//By HeYC 0907				WifiBusy=WIFI_IDEL;
			statOfConnectToRmtSvr = CON_RMT_SVR_STAT_READY;
                        ret=WIFI_ERR;
			break;
	}
	
   return ret;
}


//By HeYC 0907	void get_flag(int module) 
//By HeYC 0907	{
//By HeYC 0907	    char s[5];
//By HeYC 0907	    int low = 0, high = 0;
//By HeYC 0907	    sprintf(s, "%04x", module);
//By HeYC 0907	
//By HeYC 0907	    low = strtol(&s[2], NULL, 16);
//By HeYC 0907	    s[2] = '\0';
//By HeYC 0907	    high = strtol(&s[0], NULL, 16);
//By HeYC 0907	
//By HeYC 0907	    printf("%02x, %02x\n", high, low);
//By HeYC 0907	}


/*******************************************************************************
Function: 	    AirControltask(void)
Description: 	    �յ���������1ms����һ��
Calls: 			
Called By: 		no
Input: 			no
Output: 		state
Return: 		no
Others: 		no
********************************************************************************/
void AirControltask(void)
{
 	u08 ret=0;
 	u08 sendEnable=0;//����ʹ�ܣ���1ʱ��ʾ��Ҫ�������ݸ�������
 	int i=0;
  
 	if((air.enable==1)||(air.enable==2)||(air.enable==3))//����������Ҫ���Ϳ���
 	{
 		if((air.codeask==0)&&(air.onoff!=0x88))//���趨�յ��ͺ�
 		{
 			if(ret=AirControlCmd(0x02,0x00)!=IR_NONE)//�趨�յ��ͺ�
 			{
 				if(ret==IR_OK)//�յ��ͺ��趨�ɹ�
 				{
 					air.codeask=1; 
				}
				else//�յ��ͺ��趨ʧ��
				{
					sendEnable=1;
				}
			}
		}
		else//�յ��ͺ��趨�ɹ�
		{
			if(air.onoff==0x00)//�ػ�
			{
				if(ret=AirControlCmd(0x04,0x00)!=IR_NONE)
				{
					sendEnable=1;
				}
			}
			else if(air.onoff==0x88)//����
			{
				if(air.mode!=0xee)
				{
					if(ret=AirControlCmd(0x05,0x00)!=IR_NONE)//����ģʽ
					{
						sendEnable=1;
					}
				}
				else if(air.temp!=0xee)
				{
					if(ret=AirControlCmd(0x06,0xFF)!=IR_NONE)//�¶�
					{
						sendEnable=1;
					}
				}
				else if(air.windspeed!=0xee)
				{
					if(ret=AirControlCmd(0x07,0x00)!=IR_NONE)//����
					{
						sendEnable=1;
					}
				}
				else if(air.winddir!=0xee)
				{
					if(ret=AirControlCmd(0x08,0x00)!=IR_NONE)//����
					{
						sendEnable=1;
					}
				}
				else
				{
					;//EE������
				} 
			}
			else if(air.onoff==0xff)//��������
			{
				switch(air.step)
				{
					case IR_SEND_PRE:
						air.step++; 
						break;
					case IR_SEND_INIT:
						if(ret=AirControlCmd(0xaa,0x00)!=IR_NONE)//��ʼ����������
						{
							if(ret==IR_OK)
							{
								air.step++; 
							}
							else
							{
								sendEnable=1;
							}
						}
						break;
					case IR_SEND_MODE:
						if(ret=AirControlCmd(0x05,0x00)!=IR_NONE)//����ģʽ
						{
							if(ret==IR_OK)
							{
								air.step++; 
							}
							else
							{
								sendEnable=1;
							}
						}
						break;
					case IR_SEND_TEMP:
						if(ret=AirControlCmd(0x06,0x00)!=IR_NONE)//�¶�
						{
							if(ret==IR_OK)
							{
								air.step++; 
							}
							else
							{
								sendEnable=1;
							}
						}
						break;
					case IR_SEND_WIND_SPEED:
						if(ret=AirControlCmd(0x07,0x00)!=IR_NONE)//����
						{
							if(ret==IR_OK)
							{
								air.step++; 
							}
							else
							{
								sendEnable=1;
							}
						}
						break;

					case IR_SEND_WIND_DIR:
						if(ret=AirControlCmd(0x08,0x00)!=IR_NONE)//����
						{
							if(ret==IR_OK)
							{
								air.step++; 
							}
							else
							{
								sendEnable=1;
							}
						}
						break;

					case IR_SEND_FINISH:
						if(ret=AirControlCmd(0xcc,0x00)!=IR_NONE)//��ʼ����������
						{
							if(ret==IR_OK)
							{
								air.step++; 
							}
							else
							{
								sendEnable=1;
							}
						}
						break;

					case IR_SEND_ON:
						if(ret=AirControlCmd(0x04,0xFF)!=IR_NONE)//���Ϳ�������
						{
							if(ret==IR_OK)
							{
								sendEnable=1;
							}
							else
							{
								air.step++;
							}
						}
						break;

					case IR_SEND_ERR:
						ret=IR_OK;
						sendEnable=1;
						break;
						
					default:
						ret=IR_ERR;
						sendEnable=1;
						break;
				}
			}
		}
      
		if((sendEnable==1)&&(air.enable==1))//�����ݷ���    ������
		{
			air.enable=0;//������ϲ���ִ��
			for(i=0;i<10;i++)
			{
				sbuffer[i]=aircondition_answer[i];
			}
			for(i=10;i<16;i++)
			{
				sbuffer[i]=macidHEX[i-10];
			}
			//strcat(&sbuffer[10],macidHEX);
			if(ret==IR_OK)
			{
				sbuffer[16] = 1;
			}
			else
			{
				sbuffer[16] = 0;
			}
			strcat(&sbuffer[17],RCR_ETX);
			SMSSEND(20,sbuffer,4);
		}
    
		if((sendEnable==1)&&(air.enable==2))//�����ݷ���    APP
		{
			air.enable=0;//������ϲ���ִ��
			for(i=0;i<10;i++)
			{
				sbuffer[i]=KZaircondition_answer[i];
			}
			sbuffer[10] = air.codeValH;//keyValH 
			sbuffer[11] = air.codeValL;//keyValL 
			strcat(&sbuffer[12],RCR_ETX);
			SMSSEND(15,sbuffer,2);
		}

		if((sendEnable==1)&&(air.enable==3))//�����ݷ���    APP�յ��ͺŲ��Ի�Ӧ
		{
			air.enable=0;//������ϲ���ִ��
			for(i=0;i<10;i++)
			{
				sbuffer[i]=AirTypeTest_answer[i];
			}
			sbuffer[10] = air.codeValH;//keyValH 
			sbuffer[11] = air.codeValL;//keyValL 
			strcat(&sbuffer[12],RCR_ETX);
			SMSSEND(15,sbuffer,2);
		}   
 	}
  
}

/*******************************************************************************
Function: 	    AirControlCmd(u08 cmd,u08 state)
Description: 	    �յ������������,stateֻ�Կ��ػ���Ч��FF�� ����00���أ����������Ч
Calls: 			
Called By: 		no
Input: 			no
Output: 		state
Return: 		no
Others: 		no
********************************************************************************/
u08 AirControlCmd(u08 cmd,u08 state)
{
  u08 ret=IR_NONE;
  //air.ack=IR_NONE;
  u08 cmdBuf[5];
  u08 cmdLength=4;//Ĭ��4�ֽ�
  
  if(air.cmdsetp)//�ȴ������
  {
        //By HeeYC 0907	if((sTimeout(&wifiRetDelayTimer, 100))||(air.ack!=IR_NONE))//1s
        if(air.ack!=IR_NONE)// By HeYC 0907
          {
            if(air.ack==IR_OK)
            {
             ret=IR_OK;
            }
            else if(air.ack==IR_ERR)
            {
             ret=IR_ERR;
            }
            else
            {
             ret=IR_NONE;
            }
            air.cmdsetp=0;
          }
  }
  else
  {
   air.ack=IR_NONE;
   switch(cmd)
   {
     case 0x02://�趨�յ��ͺ�
         cmdBuf[0]=0x02;
         cmdBuf[1]=air.codeValH;
         cmdBuf[2]=air.codeValL;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0x04://���Ϳ�������
         cmdBuf[0]=0x04;
         cmdBuf[1]=state;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0x05://����ģʽ����
         cmdBuf[0]=0x05;
         cmdBuf[1]=air.mode;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0x06://�����¶�����
         cmdBuf[0]=0x06;
         cmdBuf[1]=air.temp;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0x07://���ͷ�������
         cmdBuf[0]=0x07;
         cmdBuf[1]=air.windspeed;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0x08://���ͷ�������
         cmdBuf[0]=0x08;
         cmdBuf[1]=air.winddir;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0xaa://����ʼ����������
         cmdBuf[0]=0xaa;
         cmdBuf[1]=0xaa;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     case 0xcc://���ͷ�������
         cmdBuf[0]=0xcc;
         cmdBuf[1]=0xcc;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
         break;
     default:
         air.cmdsetp=0;
         ret=IR_ERR;
         break;
  
  
   }
  
  }
  
 
  

  return ret;
}

u08 XOR(unsigned char* puchMsg, unsigned short usDataLen)
{
    unsigned char uchCRC = 0x00 ;
    
    while (usDataLen--)
    {
     uchCRC = uchCRC ^ *puchMsg++;
    }
    return (uchCRC) ;
}



/*******************************************************************************
Function: 	   IRProcess(void)
Description: 	   RFѧϰ��ɴ�����
Calls: 			
Called By: 		no
Input: 			no
Output: 		state
Return: 		no
Others: 		no
********************************************************************************/
void  IRProcess(void)
{
  u16 len,i;
 //u08 addr[4];

	if(	(IR.REVFLAG==RF_TYPE_433) ||
	 	(IR.REVFLAG==RF_TYPE_315) ||
	 	(IR.REVFLAG==RF_TYPE_INFRA)	)  //433M 315M ����
	{
		if (sTimeout(&wrlsStudyTimeOut, 1000)) // 10S��ʱ�˳�
		{
			IR.REVFLAG=RF_TYPE_NULL;//����10��Ϊѧϰ�ɹ��Զ��˳�
			ledSetParam(50,100);//ָʾ�ƻظ�������˸
			rfs.type=0;
	           	//ѧϰ��ʱ�˳�����ѧϰʧ��Ӧ��
	           	IR.Learnflag=0;;//����10��Ϊѧϰ�ɹ��Զ��˳�
			ledSetParam(50,100);//ָʾ�ƻظ�������˸
			rfs.type=0;
			for(i=0;i<10;i++)     //����ѧϰʧ��Ӧ��
			{
				sbuffer[i]=WirelearnAndControl_answer[i];
			}
			sbuffer[10] = rfs.number>>8;//keyValH 
			sbuffer[11] = rfs.number&0xff;//keyValL 
			sbuffer[12] = 0;
			strcat(&sbuffer[13],RCR_ETX);
			// strcat(&sbuffer[16],"\r\n");
			lenth=16;//strlen(sbuffer);
			SMSSEND(lenth,sbuffer,2);
		}
		else
		{
			/*if((IR.Tout>=IR_END)&&(IR.DECODESTEP==IR_DECODE))
			{
				IR.PulseN++;//���ʱ�ͺ�һ�����壬���Լ�һ
				//IR.Rcounter=(TIM2->CNTRH<<8)+TIM2->CNTRL;//
				IR.Rcounter=(TIM2->CNTRH)*256;
				IR.Rcounter+=TIM2->CNTRL;//
				TIM2->CNTRH=0;
				TIM2->CNTRL=0;
				IR.cmd[IR.PulseN]=IR.Rcounter;
				IR.Learnflag=1;//
				IR.REVFLAG=0;
				IR.cmd[0]=IR.PulseN;	//���峤��
			}*/
		}
	}
	if(IR.Learnflag==1) //ѧϰ�������
 	{
		IR.Learnflag=0;//
		len=IR.PulseN+1;//�������ȱ�����flash
		disableInterrupts();  //д֮ǰ�ȹ��ж�
 		/*for(i=0;i<len;i++)
 		{
			TEMP_cmd[0]=(u08)((IR.cmd[i]>>8)&0x00ff);
			TEMP_cmd[1]=(u08)(IR.cmd[i]&0x00ff);
			AT24_Write_page((RF433_PRE_ADDR+OffsetAddr*rfs.number+i*2),TEMP_cmd,2);//RAM���ޣ�2�ֽڴ洢һ��
			for(int j=0;j<1000;j++) {;}  // 8us��ʱ
		}*/
		for(i=0;i<len;i++)
		{
			eeprombuf[i*2]=(u08)((IR.cmd[i]>>8)&0x00ff);
			eeprombuf[i*2+1]=(u08)(IR.cmd[i]&0x00ff);    
		}
		if(rfs.type==2)  //433
		{
			if(len*2<=128)
			{
				AT24_Write_page((RF433_PRE_ADDR+OffsetAddr*rfs.number),eeprombuf,len*2);//RAM���ޣ�2�ֽڴ洢һ��
			}
			else if(len*2>128)
			{
      
				if(len*2<=256)
				{
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// �߿��Ź�
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),len*2-128);
				}
				else if(len*2>256)
				{
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// �߿��Ź�
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// �߿��Ź�
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number+256),&(eeprombuf[256]),len*2-256);
				}
				for(dword i=0;i<1500;i++){}
				AT24_Write_page((RF433_PRE_ADDR+OffsetAddr*rfs.number),splflage,2);
			}
		}
		else if(rfs.type==1)  //315
		{
			if(len*2<=128)
			{
				AT24_Write_page((RF315_PRE_ADDR+OffsetAddr*rfs.number),eeprombuf,len*2);//RAM���ޣ�2�ֽڴ洢һ��
			}
			else if(len*2>128)
			{
      
				if(len*2<=256)
				{
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// �߿��Ź�
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),len*2-128);
				}
				else if(len*2>256)
				{
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// �߿��Ź�
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// �߿��Ź�
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number+256),&(eeprombuf[256]),len*2-256);
				}
				for(dword i=0;i<1500;i++){}
				AT24_Write_page((RF315_PRE_ADDR+OffsetAddr*rfs.number),splflage,2);
			}
		}

		enableInterrupts();  //д�������ж�
		rfs.type=0;
		for(i=0;i<10;i++)     //����ѧϰ�ɹ�Ӧ��
		{
			sbuffer[i]=WirelearnAndControl_answer[i];
		}
		sbuffer[10] = rfs.number>>8;//keyValH 
		sbuffer[11] = rfs.number&0xff;//keyValL 
		sbuffer[12] = 1;
		strcat(&sbuffer[13],RCR_ETX);
		// strcat(&sbuffer[16],"\r\n");
		lenth=16;//strlen(sbuffer);
		SMSSEND(lenth,sbuffer,2);
		LED_staflag=1;
		ledSetParam(1,1);//ֹͣ��˸����
	}
	else if(IR.Learnflag==2) //����ѧϰ�ɹ��˳�
	{
		IR.Learnflag=0;;//����10��Ϊѧϰ�ɹ��Զ��˳�
		ledSetParam(50,100);//ָʾ�ƻظ�������˸
		rfs.type=0;
		for(i=0;i<10;i++)     //����ѧϰʧ��Ӧ��
		{
			sbuffer[i]=WirelearnAndControl_answer[i];
		}
		sbuffer[10] = rfs.number>>8;//keyValH 
		sbuffer[11] = rfs.number&0xff;//keyValL 
		sbuffer[12] = 1;
		strcat(&sbuffer[13],RCR_ETX);
		// strcat(&sbuffer[16],"\r\n");
		lenth=16;//strlen(sbuffer);
		SMSSEND(lenth,sbuffer,2);
	}
	else if(IR.Learnflag==0xff) //ѧϰ�쳣�˳�
	{
		IR.Learnflag=0;;//����10��Ϊѧϰ�ɹ��Զ��˳�
		ledSetParam(50,100);//ָʾ�ƻظ�������˸
		rfs.type=0;
		for(i=0;i<10;i++)     //����ѧϰʧ��Ӧ��
		{
			sbuffer[i]=WirelearnAndControl_answer[i];
		}
		sbuffer[10] = rfs.number>>8;//keyValH 
		sbuffer[11] = rfs.number&0xff;//keyValL 
		sbuffer[12] = 0;
		strcat(&sbuffer[13],RCR_ETX);
		// strcat(&sbuffer[16],"\r\n");
		lenth=16;//strlen(sbuffer);
		SMSSEND(lenth,sbuffer,2);
	}
 	else
 	{
 	}
 
}
void ResponseToAppCfg();

void  CheckTimeOutAfterAPPCfg(void)
{

  if(flagGotAppCfgInfo==1)
  {

	//send response to avoid data packet lose	
	ResponseToAppCfg();
	
    if (sTimeout(&gAppCfgSuccTmout, 300)) //5s  He: from 500 to 300
   {
//By HeYC 0822		     setSSid=1; //����WIFIģ��stassid����
     		flagConnetedWithSvr=0; 
     		ledSetParam(10,20);//������˸����wifi����
	     flagGotAppCfgInfo=0;

		 //Bgn:By HeYC 0822
		   wifiMDInitflag=0;
	          strcpy(UDPPort,"59999");

		  wifiInitMsg=WIFI_INIT_PRE;//Ensure task will be started correctly

		  wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
                uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
       	   ChangeTask(wifiMonitorTaskID);//����wifi��ʼ������
		 //End:By HeYC 0822
   }
  } 
}

u08 IsAlarmSrcNoTriggered(u08 almSrcNo)
{
	int i=0;
	for(i=0;i<MAX_ALM_SENSOR_NUM;i++){
		if(alarm.almSrcNoCache[i]==almSrcNo)
			return 1;
	}

	return 0;
}

void CacheAlarmSrcNoTriggered(u08 almSrcNo)
{
	int i=0;
	for(i=0;i<MAX_ALM_SENSOR_NUM;i++){
		if(alarm.almSrcNoCache[i]==INVALID_ALM_SRC_NO){
			alarm.almSrcNoCache[i]=almSrcNo;
		}
	}
}


/*******************************************************************************
Function: 	 void  alarmRFTask(void)
Description: 	   ��������
Calls: 			
Called By: 		no
Input: 			no
Output: 		state
Return: 		no
Others: 		no
********************************************************************************/

void  alarmRFTask(void)//100ms����
{
	 int i,j,len;
//by HeYC	 int alarmDate;//������ȡ�����ݸߵ��ж�
//By HeYC	 int rfDate;//flash��ȡ�����ݸߵ��ж�
//By HeYC	 int rfcmd;//flash��ȡ��8λ����ת16λ����
	 u08 alarmTriggered=0;
	 u08 rfCodingRcv=0;//By HeYC 
	 u08 rfCodingERom=0;//By HeYC
	 int data1InCmdERom=0;//by HeYC
	 int data2InCmdERom=0;//by HeYC

	if(curSecuModeShadow==0){
		//not set security, nothing need to do.
		//clear any thing if detected
		if(	rfIdleRcvStat==IRRS_315_GUIDE_RCVD ||
			rfIdleRcvStat==IRRS_433_RCV_SUCC){

			rfIdleRcvStat=IRRS_IDLE;
			alarm.DECODESTEP=IR_GUIDE;
		}
		return;
	}

	if(	rfIdleRcvStat!=IRRS_315_RCV_SUCC && 
		rfIdleRcvStat!=IRRS_433_RCV_SUCC){
		//No alarm message from sensor detected, nothing need to do
		//this variable has another values:
		//IRRS_IDLE,
		//IRRS_315_GUIDE_RCVD,
		//IRRS_433_GUIDE_RCVD,
		//
		return;
	}

	//no special RF waiting for receiving.so it should be alarm
	if(IR.REVFLAG!=RF_TYPE_NULL){
		//
		return;
	}

	if(rfs.type!=0){
		//
		return;
	}
	
//HeYC0830  if(	(curSecuModeShadow==1)&&
//HeYC0830    	( (rfInterLock==3)||(rfInterLock==4) )&&
//HeYC0830    	( IR.REVFLAG==0 )&&
//HeYC0830    	( rfs.type==0)	)//By HeYC
 //By HeYC	if((SetOrUnsetState[0]==1)&&((rfInterLock==3)||(rfInterLock==4))&&(IR.REVFLAG==0)&&(rfs.type==0))//�������״̬,���б����źţ��Ҳ���ѧϰ�Ϳ���״̬
 //HeYC0830{

	// clear eeprom at first
	memset(eeprombuf,0,270);
	//He: why 80 loops, compare with all rf data saved in eeprom. max 80
   	for(i=0;i<80;i++)
   	{
   		//He:Check alarm is 433 or 315M, read data len saved from different address firstly
   		switch (rfIdleRcvStat)
		{
			case IRRS_315_RCV_SUCC://rf315
				disableInterrupts();  //��֮ǰ�ȹ��ж�
       			AT24__Read_Page((RF315_PRE_ADDR+OffsetAddr*i),readbuf,2);
       			enableInterrupts();  //���������ж�
       			break;

			case IRRS_433_RCV_SUCC://rf433
       			disableInterrupts();  //��֮ǰ�ȹ��ж�  
       			AT24__Read_Page((RF433_PRE_ADDR+OffsetAddr*i),readbuf,2);
       			enableInterrupts();  //���������ж�
       			break;

			default:
       			break;
		}

		//He:only len is equal, can other thing continue
	    	len=readbuf[0]*256+readbuf[1];
	    	readbuf[0]=0;
	    	readbuf[1]=0;
	    	if(len!=IR.cmd[0])//������ȣ������ж�����
		    	continue;
			
	    	//{
	    	//He:Why len++? because the first  bytes is saved as length.
	      	len++;
	      	disableInterrupts();  //��֮ǰ�ȹ��ж�
	      	if(rfIdleRcvStat==IRRS_433_RCV_SUCC)   //433M
	      	{
	       	// if(len*2<=128)
	       	// {
	       	AT24__Read_Page((RF433_PRE_ADDR+OffsetAddr*i),eeprombuf,len*2);
	       	// }
	       	// else if(len*2>128)
	       	// { 
	       	//  AT24__Read_Page((RF433_PRE_ADDR_SPL+OffsetAddr*i),eeprombuf,len*2); 
	       	// }
	      	}
	      	else if(rfIdleRcvStat==IRRS_315_RCV_SUCC) //315M
	      	{
	       	//if(len*2<=128)
	       	//{
	       	AT24__Read_Page((RF315_PRE_ADDR+OffsetAddr*i),eeprombuf,len*2);
		      // }
		      // else if(len*2>128)
		      // {
		      //  AT24__Read_Page((RF315_PRE_ADDR_SPL+OffsetAddr*i),eeprombuf,len*2);
		      // }    
	      	}
	      	enableInterrupts();  //���������ж�
	      	//�ж�����
	      	//He:below is fucking logic.
	      	//By HeYC 0830	if(len>5)
	      	//By HeYC 0830	{
	       //By HeYC 0830		alarmActive=1;
	      	//By HeYC 0830	}

		//He:The last is the stop flag, addr 0 is len, addr 1 and addr 2 not used
		alarmTriggered=1;// intial with 1, if not equal, will be set to 0;
	      	//By HeYC	for(j=3;j<(len-1);j++)//ȥ�����ݳ��ȡ������롢ͬ���벻���ж�
	      	len--;//By HeYC:remove len field at the first position
	      	for(j=1;j<(len/2);j++)//By HeYC
	      	{
	      		//3\4, 5\6,7\8,...,51\52
	      		//*Bgn:By HeYC
	      		//Compare the coding of IR.cmd and coding saved in eeprom.
			if(IR.cmd[j*2+1]>IR.cmd[j*2+2]){
				// 10
				rfCodingRcv=1;
			}
			else{
				//01
				rfCodingRcv=0;
			}

			//6&7\8&9, 10&11\12&13, 14&15\16&17,...,102&103\104&105
			data1InCmdERom=eeprombuf[j*4+2]*256+eeprombuf[j*4+3];
			data2InCmdERom=eeprombuf[j*4+4]*256+eeprombuf[j*4+5];
			if(data1InCmdERom>data2InCmdERom){
				//10
				rfCodingERom=1;
			}
			else{
				//01
				rfCodingERom=0;
			}

			if(rfCodingERom!=rfCodingRcv){
				alarmTriggered=0;
				break;
			}
				
	      		//End:By HeYC*/
	      		/*Bgn:Commented by HeYC
	      		//He:Check logic 1 or 0 of alarm messgae read.
	       	if(	( (IR.cmd[j]>=255)&&(IR.cmd[j]<=500) ) ||
				( (IR.cmd[j]>=100)&&(IR.cmd[j]<=200) )	)//̽ͷ�������ݽ���
	       	{
		        	alarmDate=1;//��������1
	       	}
	       	else
	       	{
	       		alarmDate=0;//��������0
	       	}

			//He:Check logic 1 or 0 of studied and saved in eeprom.
			rfcmd=eeprombuf[j*2]*256+eeprombuf[j*2+1];//flash����ת��Ϊ16λ����
	       	if(	( (rfcmd>=255)&&(rfcmd<=500) ) ||
				( (IR.cmd[j]>=100)&&(IR.cmd[j]<=200) ) )//̽ͷ�������ݽ���
	       	{
	       		rfDate=1;//��������1
	       	}
	       	else
	       	{
		       	rfDate=0;//��������0
	       	}
	       	if(alarmDate!=rfDate)//һ�����ݲ������������ѭ��
	       	{
	       		alarmActive=0;//����ʧ��
	        		break;
	       	}
			//End:commented by HeYC */
	      	}      
	    //}
			
		if(alarmTriggered==1)//����ɹ������ͱ����ź�
	    	{
	    		if(IsAlarmSrcNoTriggered(i)==0){
				//Should report this alarm
				CacheAlarmSrcNoTriggered(i);
				
				alarm.NO=i;//̽ͷ���
				
		     		secModChangedFlag=3;		//By HeYC let loop send out stat report
		     		
		     		curSecuModeShadow=2;	//By HeYC	     		
		     		
		     		//By HeYC 0908	if(alarm.sending==0)		//ͬһ�α���ֻ����һ��, Fucking logic  HeYC
		     		//By HeYC 0908	{
		      		//By HeYC 0908		alarm.sending=1;
		      		//By HeYC 0908		alarm.timeCounter=0;
		      			//�����źŷ��ͣ�ͨ�����ͺ�������̽ͷ���
		      			for(i=0;i<10;i++)     
		      			{
		        			sbuffer[i]=ProbeAlarm[i];
		      			}
		      			for(i=10;i<16;i++)
		      			{
		        			sbuffer[i]=macidHEX[i-10];
		      			}

		      			sbuffer[16] = alarm.NO>>8;	//keyValH 
		      			sbuffer[17] = alarm.NO&0xff;	//keyValL 
		      			strcat(&sbuffer[18],RCR_ETX);
		      			lenth=21;
		      			SMSSEND(lenth,sbuffer,4);    //����������ͱ�����Ϣ

			  		//He:this flag will be detected in initTask loops and will be used to send a broadcast in local network
		      			almBrdcstFlag=2;		//broadcast 2 times
				//By HeYC 0908	}
		     		//By HeYC 0908	else
		     		//By HeYC 0908	{
		       	//By HeYC 0908		alarm.timeCounter=0;
		     		//By HeYC 0908	}
		     		//
	    		}
	     		break;//����ѭ��80
	  	}

		//He:fucking logic below, i commented
	  	//if(alarm.NO==79)
		//{
	      	//	alarmActive=0;
	     	//	alarm.DECODESTEP=IR_GUIDE;
	     	//	alarm.Learnflag=0;
	    		// alarm.REVFLAG=2;
	     	//	alarm.PulseN=0;
	     	//	IR.cmd[0]=0;
	     	//	alarm.guide=0;
	     	//	rfInterLock=IRRS_IDLE;//�������
		//}

		//He, watch dog should be keeped here??
	}
 //HeYC0830}
 
//By HeYC 0908	if(++alarm.timeCounter>=100)//����10��δ�յ������źţ������ź���ʧ���ٴδ����豨��
//By HeYC 0908	 {
//By HeYC 0908	 		alarm.sending=0;
//By HeYC 0908	 }

	//reset the flags for new receiving...
	alarm.guide=0;
	rfIdleRcvStat=IRRS_IDLE;//By HeYC, finished dealng, clean the flag
	alarm.DECODESTEP=IR_GUIDE;
	//By HeYC0901	alarm.Learnflag=0;	
	alarm.PulseN=0;
	IR.cmd[0]=0;
}


/* EOF */


