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
* 版本创建日志：
* -版本-               -创建日期-               -说明-
*
* coffeeM-1.00        2006-08-25             创建版本
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
Description: 	系统空闲任务，完成系统实时处理任务
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
				 10ms任务
 ***************************************************************************************************************************************************/
 		 
	if(sys1msFlag)	
	{
   		sys1msFlag=0;
   		AirControltask();//红外芯片控制任务，串口3

		//He:WifiBusy never be se to WIFI_DATA_BUSY
   		//He:if(((WifiBusy==WIFI_IDEL)||(WifiBusy==WIFI_DATA_BUSY))&&(wifiEnable==WIFI_STATE_INIT_FINISH))
   		//HeYC 0907	if( 	(WifiBusy==WIFI_IDEL) &&
		//HeYC 0907		(wifiATCmdSetStat==WIFI_AT_CMD_OVER))//By HeYC
		if( (wifiATCmdSetStat==WIFI_AT_CMD_OVER) )//By HeYC, WifiBusy is a strange variable
   		{
     			Auto_WIFI_Send();//发送数据
   		}
   
   		//HeYC 0907if(	( (WifiBusy==WIFI_IDEL)||(WifiBusy==WIFI_AT_BUSY) )&&
		//HeYC 0907	( wifiATCmdSetStat==WIFI_AT_CMD_OVER))
		if( (wifiATCmdSetStat==WIFI_AT_CMD_OVER) )//By HeYC, WifiBusy is a strange variable
   		{
     			if(flagBgnConnectSvr)//20秒连接一次
     			{
        			connectRmtSvrResult=ConnectToRmtSvrInSTAMode();
        			if (connectRmtSvrResult!= WIFI_NONE)//连接云服务器
        			{
          				if( connectRmtSvrResult!=WIFI_OK)//HeYC:from 5 to 2
            				{
	            				if(++ClientCounterA>=2)
						{
		              			//By HeYC0820 Authenticationflag=0;//关闭心跳
		              			//By HeYC0820 HFlagWhereAuthSetZero=1;
		              			wifi_monitor_step=WIFI_RESTART_PRE;
			      				wifiATCmdSetStat=WIFI_AT_CMD_NOT_BGN;//By HeYC 0822
		              			ChangeTask(wifiResetTaskID); // 启动重启模块任务
		              			ClientCounterA=0;
		              			flagConnetedWithSvr=1;//停止连接服务器
	            				}
					}  
            
            				flagBgnConnectSvr=0;
        			}
      				// if (wifiCmdCIPSTART(staClientIP,staClientPort,1)!= WIFI_NONE)//连接云服务器
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
      					// ChangeTask(wifiResetTaskID); // 启动重启模块任务
       				ChangeTask(wifiMonitorTaskID);//启动wifi初始化任务
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
    			IWDG_ReloadCounter();			// 踢看门狗
    			if (++sysTickfor100ms >= 10) // 100ms
			{
          			if(LED_staflag)  
          			{
            				if(++LED_stpflag>=20)
            				{
              				LED_staflag=0;
              				LED_stpflag=0;
              				ledSetParam(50,100);//指示灯回复正常闪烁
              
            				}
          			}
	   			sysTickfor100ms = 0;
				
       			alarmRFTask();//100ms检查一次是否有报警数据
       
       			if(secModChangedFlag==1 || secModChangedFlag==2)  ////安防状态广播
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
         				SMSSEND(lenth,sbuffer,1);   //广播
					//By HeYC         SetOrUnsetflag=0;
	 				secModChangedFlag--;//By HeYC
       			}
	   			if((KEYPRESS_DelayFlg)&&(KEYPRESS_Long==0))//长按键处理
           			{
            				if(++KEYPRESS_Counter>=30)//长按3秒
            				{
             					KEYPRESS_Long=1;
             					KEYPRESS_Counter=0;
            					// KEYPRESS_DelayFlg=0;
            
             					ledSetParam(10,20);//快速闪烁进入配置模式
            				}
           			}
	  			/// <!-- 500mS定时器
	  			if (++sysTickfor500ms > 5)	//500mS
     				// if (++sysTickfor500ms > 30)	//3S 测试用
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
          					SMSSEND(lenth,sbuffer,1);    //广播报警信息
          					almBrdcstFlag--;
        				}
        
        				if(secModChangedFlag==3)//安防状态改变上报服务器
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
          					SMSSEND(lenth,sbuffer,4);  //发送给服务器
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
             
               
                 			//By HeYC0820	if(Authenticationflag)//8S   心跳
                 			//By HeYC 0820	{
                   			if(++sysTickfor8s>=30)// 30*500ms=15S (in fact , 19.x sec)
                   			{
                   				sysTickfor8s=0;
						
                   				if(Authenticationflag)//HeYC:0820
						{//By HeYC 0820
	                     			ClientCounterA=0;  //服务器从连次数标志
	                     			//sysTickfor8s=0;
	                     			for(int i=0;i<13;i++)
	                     			{
	                        				sbuffer[i]=Heartbeat[i];
	                     			}
	                     			lenth=13;//strlen(sbuffer);
	                     			SMSSEND(lenth,sbuffer,4);//发送给服务器
						}//By HeYC 0820


						//He:check if no heart beat cnt exceed max
						if(++Heartbeattimer5s_count>=5)//By HeYC from 3 to 5
                     			{ 
                       				Authenticationflag=0;//关闭心跳
//By HeYC 0907	                       				HFlagWhereAuthSetZero=2;
                       				Heartbeattimer5s_count=0;
                        				wifi_monitor_step=WIFI_RESTART_PRE;
                       				// ChangeTask(wifiResetTaskID); // 启动重启模块任务
                       				flagConnetedWithSvr=0; //启动连接云服务器任务
                        
                     			}
                   			}
                 			//By HeYC0820}
                   
                   			//5s判断一次安防状态改变否
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
                 				if((++timerOfReConnectSvr>=15)&&(wifiATCmdSetStat==WIFI_AT_CMD_OVER)&&(flagBgnConnectSvr==0))//60秒连接一次(120)
                 				{
							//By HeYC                  Authenticationflag=0;//关闭心跳
							//By HeYC                  HFlagWhereAuthSetZero=3;
                  					flagBgnConnectSvr=1;
                  					timerOfReConnectSvr=0;
                  					statOfConnectToRmtSvr=CON_RMT_SVR_STAT_READY;
                  
                 				}
                			}
                   
                			//By HeYC 0907	if(wifiReset)
                			//By HeYC 0907	{
                 			//By HeYC 0907		if(++wifiCounter>=2)//启动延时2秒后重新初始化WIFI模块
                 			//By HeYC 0907		{
                  			//By HeYC 0907			wifiReset=0;
                  			//By HeYC 0907			wifiATCmdSetStat=WIFI_STATE_POWER_ON;//HeYC: from 1 to WIFI_STATE_POWER_ON 
                  			//By HeYC 0907			uartFlushReceiveBuffer(COM_WIFI);
                  			//By HeYC 0907			ledSetParam(10,30);
                  			//By HeYC 0907			ChangeTask(wifiMonitorTaskID);//启动收发任务
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
Description: 	系统应用入口，实现用户任务的创建，初始化系统状态
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
		sys10msFlag	  = 0;        /* 10ms标志 				*/
		TaskInitFlag = 1;
		sTimeout(&SystemTimer, 0);
                ledSetParam(10,30);
	}
	
	if (sTimeout(&SystemTimer, 200)) // 启动延时2秒,等待外设初始化完毕
	{
		 
               // sysInit();//稳定系统
		CreatTask(&wifiMonitorTaskID, wifiMonitorTask);//WiFi初始化任务
                //wifiReset
                CreatTask(&wifiResetTaskID, wifiResetTask);//wifi重启任务
               
		GETCONFIG();  //获取默认配置参数
//By HeYC 0907			wifiReset=1;
		wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
              uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
              
                ClientID=0;
                WaitMONITOR_IDELSend=0;
                WIFI_Hail=0;
                WIFI_Head=0;
		//////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//CANsenden=0x01;          //CAN发送使能
		//StartADC_DMA();    //启动ADC采集
		//ChangeTask(0); // 挂起任务
		ChangeTask(wifiMonitorTaskID);//启动wifi初始化任务
                
	}
}

/*******************************************************************************
Function: 		wifiMonitorTask()
Description: 	        WIFI模块初始化任务
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
	if (ret == WIFI_ERR) // 模块初始化错误
	{
	  wifi_monitor_step=WIFI_RESTART_PRE;
          ChangeTask(wifiResetTaskID); // 启动重启模块任务
          
	}
	else
	{
//By HeYC0907           if(wifiATCmdSetStat)
          {
           wifiATCmdSetStat=WIFI_AT_CMD_OVER;//HeYC: from 2 to WIFI_STATE_INIT_FINISH
           //By HeYC 0907	strcpy(sbuffer,"?GTIME**");
           //By HeYC 0907	strcat(sbuffer,"\r\n");	
           //By HeYC 0907	lenths=strlen(sbuffer);
          // SMSSEND(lenths,sbuffer,ClientID);//发送给手机
          // SMSSEND(lenths,sbuffer,4);//发送给服务器
          }
		  
          //By HeYC 0923	ledSetParam(50,100);
	   uartFlushReceiveBuffer(COM_WIFI); //By HeYC
	   ChangeTask(0); // 
	   
          if(cfgWifiForAppCfg)
          {
           flagConnetedWithSvr=1; //关闭连接云服务器任务   
          }
          else if(cfgWifiForAppCfg==0)
          {
          ledSetParam(50,100);//Toggle led to normal heat beat frequency
            flagConnetedWithSvr=0; //启动连接云服务器任务 
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
                 // SMSSEND(lenths,sbuffer,4);//发送给服务器

		    wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
                  uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
                  ChangeTask(wifiMonitorTaskID);//启动wifi初始化任务
   		  break;
         default:
	         WIFI_POWER_ON();
	         wifi_monitor_step=WIFI_RESTART_PRE;

		   wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
                  uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
                 ChangeTask(wifiMonitorTaskID);//启动wifi初始化任务
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
//*函数名称：GETCONFIG()
//*功能描述：参数初始化
//*入口参数：void
//*出口参数：void
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
     //按键使能
//By HeYC 0906	          memset(flashbuf,0,sBUFFER_SIZE);
//By HeYC 0906	     Read_Flash(ENBALE_KEY_ADDR,flashbuf,1);
//By HeYC 0906	     if(flashbuf[0]==0)//
//By HeYC 0906	     {
//By HeYC 0906	       ENBALE_KEY=1;//默认本地按键有效
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
//By HeYC 0906	       delayOffTime=0;//默认不延时
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
Description: 	数字转字符
Calls: 			
Called By: 		no
Input: 			数字
Output: 		字符
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
Description: 	字符转数字
Calls: 			
Called By: 		no
Input: 			字符
Output: 		数字
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
   if(key!=lastkey)   //检测按键按下
   {
      for(i=0;i<1000;i++)
      {
         if(READ_KEY==key)
         {
           couter++;
         }
      }
      if(couter>950)//消除抖动
      {
        lastkey=key;
        if(key==0)
        {      
          KEYPRESS_DelayFlg=1;//按键按下
          KEYPRESS_Long=0;
          KEYPRESS_Counter=0;     
        }     
      }
    
   }
   if(KEYPRESS_DelayFlg)//检测按键释放
   {
      key=READ_KEY;
      if(key)
      {
        couter=0;
        for(i=0;i<1000;i++)
        {
          if(READ_KEY==key)//按键释放
          {
            couter++;
          }
        }
        if(couter>950)//消除抖动
        {
          lastkey=key;
          Authenticationflag=0;//关闭心跳
//By HeYC 0907	          HFlagWhereAuthSetZero=4;
          flagConnetedWithSvr=1;
          cfgWifiForAppCfg=1;
          KEYPRESS_DelayFlg=0;
          wifiMDInitflag=1; //初始化模式标志
          strcpy(UDPPort,"40001");
         // wifi_monitor_step=WIFI_RESTART_PRE;
         // ChangeTask(wifiResetTaskID); // 启动重启模块任务
         wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
         uartFlushReceiveBuffer(COM_WIFI); //By HeYC
          ChangeTask(wifiMonitorTaskID);//启动wifi初始化任务
         // IR_Decoding(2,1);//TEST  长按3秒运行指示灯快速闪烁，进入学习学习RF序列//改到命令哪里，前边的2代表433，1，315，后边的1改成对应的键值
         // sTimeout(&TimeOut, 0);  //10S学习时间，超时退出
        }
      }
   }
   

}
//////
/*******************************************************************************
Function: 	    setssidtask(void)
Description: 	    WIFI模块ssid设置（wifi名称和密码）
Calls: 			
Called By: 		no
Input: 			字符
Output: 		数字
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
           //设置模块的SSID
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
            //修改了AP的SSID
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
            ChangeTask(wifiResetTaskID);//启动wifi初始化任务
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
Description: 	    WIFI模块stassid设置
Calls: 			
Called By: 		no
Input: 			字符
Output: 		数字
Return: 		no
Others: 		no
********************************************************************************/
/*Bgn:By HeYC 
u08 setstassidtask(void)
{  
 rets=WIFI_NONE;
 u08 ret=WIFI_NONE;
 
   WifiBusy=WIFI_STA_BUSY;//模块开始发送数据，期间不可打断
   if ((rets=wifiCmdCWJAP(ssid,ssidPassword,1)) != WIFI_NONE)   //
   {
     //连接的路由器的SSID
     WifiBusy=WIFI_IDEL;//
     if(rets==WIFI_OK)
     {
      staClientConnentStart=1;//准备连接云服务器
      ret=WIFI_OK;
     }
     else 
     {
      ret=WIFI_ERR;//连接失败
     }
       
   }

   return ret;
}
//End:Commented by HeYC*/

/*******************************************************************************
Function: 	    STAClienttask(void)
Description: 	    连接远程服务器
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
//By HeYC 0907	                        WifiBusy=WIFI_AT_BUSY;//模块开始发送数据，期间不可打断
			break;
		case CON_RMT_SVR_STAT_INIT_CIFSR:
			if (wifiCmdCIFSR(1) != WIFI_NONE)   //
			{
                         if(STAEnable)//已连接路由器
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
                     if (sTimeout(&wifiSendTimer, 200))  //5s等待重启完成
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
Description: 	    空调控制任务1ms调用一次
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
 	u08 sendEnable=0;//发送使能，置1时表示需要发送数据给服务器
 	int i=0;
  
 	if((air.enable==1)||(air.enable==2)||(air.enable==3))//有新数据需要发送控制
 	{
 		if((air.codeask==0)&&(air.onoff!=0x88))//先设定空调型号
 		{
 			if(ret=AirControlCmd(0x02,0x00)!=IR_NONE)//设定空调型号
 			{
 				if(ret==IR_OK)//空调型号设定成功
 				{
 					air.codeask=1; 
				}
				else//空调型号设定失败
				{
					sendEnable=1;
				}
			}
		}
		else//空调型号设定成功
		{
			if(air.onoff==0x00)//关机
			{
				if(ret=AirControlCmd(0x04,0x00)!=IR_NONE)
				{
					sendEnable=1;
				}
			}
			else if(air.onoff==0x88)//单控
			{
				if(air.mode!=0xee)
				{
					if(ret=AirControlCmd(0x05,0x00)!=IR_NONE)//运行模式
					{
						sendEnable=1;
					}
				}
				else if(air.temp!=0xee)
				{
					if(ret=AirControlCmd(0x06,0xFF)!=IR_NONE)//温度
					{
						sendEnable=1;
					}
				}
				else if(air.windspeed!=0xee)
				{
					if(ret=AirControlCmd(0x07,0x00)!=IR_NONE)//风速
					{
						sendEnable=1;
					}
				}
				else if(air.winddir!=0xee)
				{
					if(ret=AirControlCmd(0x08,0x00)!=IR_NONE)//风向
					{
						sendEnable=1;
					}
				}
				else
				{
					;//EE不动作
				} 
			}
			else if(air.onoff==0xff)//开机操作
			{
				switch(air.step)
				{
					case IR_SEND_PRE:
						air.step++; 
						break;
					case IR_SEND_INIT:
						if(ret=AirControlCmd(0xaa,0x00)!=IR_NONE)//初始化启动命令
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
						if(ret=AirControlCmd(0x05,0x00)!=IR_NONE)//运行模式
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
						if(ret=AirControlCmd(0x06,0x00)!=IR_NONE)//温度
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
						if(ret=AirControlCmd(0x07,0x00)!=IR_NONE)//风速
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
						if(ret=AirControlCmd(0x08,0x00)!=IR_NONE)//风向
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
						if(ret=AirControlCmd(0xcc,0x00)!=IR_NONE)//初始化结束命令
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
						if(ret=AirControlCmd(0x04,0xFF)!=IR_NONE)//发送开机命令
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
      
		if((sendEnable==1)&&(air.enable==1))//有数据发送    服务器
		{
			air.enable=0;//发送完毕不在执行
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
    
		if((sendEnable==1)&&(air.enable==2))//有数据发送    APP
		{
			air.enable=0;//发送完毕不在执行
			for(i=0;i<10;i++)
			{
				sbuffer[i]=KZaircondition_answer[i];
			}
			sbuffer[10] = air.codeValH;//keyValH 
			sbuffer[11] = air.codeValL;//keyValL 
			strcat(&sbuffer[12],RCR_ETX);
			SMSSEND(15,sbuffer,2);
		}

		if((sendEnable==1)&&(air.enable==3))//有数据发送    APP空调型号测试回应
		{
			air.enable=0;//发送完毕不在执行
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
Description: 	    空调控制命令解码,state只对开关机有效，FF： 开，00：关，其余参数无效
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
  u08 cmdLength=4;//默认4字节
  
  if(air.cmdsetp)//等待命令返回
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
     case 0x02://设定空调型号
         cmdBuf[0]=0x02;
         cmdBuf[1]=air.codeValH;
         cmdBuf[2]=air.codeValL;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0x04://发送开关命令
         cmdBuf[0]=0x04;
         cmdBuf[1]=state;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0x05://发送模式命令
         cmdBuf[0]=0x05;
         cmdBuf[1]=air.mode;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0x06://发送温度命令
         cmdBuf[0]=0x06;
         cmdBuf[1]=air.temp;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0x07://发送风速命令
         cmdBuf[0]=0x07;
         cmdBuf[1]=air.windspeed;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0x08://发送风向命令
         cmdBuf[0]=0x08;
         cmdBuf[1]=air.winddir;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0xaa://发初始化启动命令
         cmdBuf[0]=0xaa;
         cmdBuf[1]=0xaa;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
         break;
     case 0xcc://发送风向命令
         cmdBuf[0]=0xcc;
         cmdBuf[1]=0xcc;
         cmdBuf[2]=0x08;
         cmdBuf[3]=0x08;//air.TimeMin;
         cmdBuf[4]=XOR(cmdBuf,cmdLength);
         air.cmdsetp=1;
         sTimeout(&irDelayTimer, 0);
         uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //把将要发送的数据发送出去
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
Description: 	   RF学习完成处理函数
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
	 	(IR.REVFLAG==RF_TYPE_INFRA)	)  //433M 315M 红外
	{
		if (sTimeout(&wrlsStudyTimeOut, 1000)) // 10S超时退出
		{
			IR.REVFLAG=RF_TYPE_NULL;//超过10秒为学习成功自动退出
			ledSetParam(50,100);//指示灯回复正常闪烁
			rfs.type=0;
	           	//学习超时退出发送学习失败应答
	           	IR.Learnflag=0;;//超过10秒为学习成功自动退出
			ledSetParam(50,100);//指示灯回复正常闪烁
			rfs.type=0;
			for(i=0;i<10;i++)     //发送学习失败应答
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
				IR.PulseN++;//输出时滞后一个脉冲，所以加一
				//IR.Rcounter=(TIM2->CNTRH<<8)+TIM2->CNTRL;//
				IR.Rcounter=(TIM2->CNTRH)*256;
				IR.Rcounter+=TIM2->CNTRL;//
				TIM2->CNTRH=0;
				TIM2->CNTRL=0;
				IR.cmd[IR.PulseN]=IR.Rcounter;
				IR.Learnflag=1;//
				IR.REVFLAG=0;
				IR.cmd[0]=IR.PulseN;	//脉冲长度
			}*/
		}
	}
	if(IR.Learnflag==1) //学习正常完成
 	{
		IR.Learnflag=0;//
		len=IR.PulseN+1;//包括长度保存至flash
		disableInterrupts();  //写之前先关中断
 		/*for(i=0;i<len;i++)
 		{
			TEMP_cmd[0]=(u08)((IR.cmd[i]>>8)&0x00ff);
			TEMP_cmd[1]=(u08)(IR.cmd[i]&0x00ff);
			AT24_Write_page((RF433_PRE_ADDR+OffsetAddr*rfs.number+i*2),TEMP_cmd,2);//RAM有限，2字节存储一次
			for(int j=0;j<1000;j++) {;}  // 8us延时
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
				AT24_Write_page((RF433_PRE_ADDR+OffsetAddr*rfs.number),eeprombuf,len*2);//RAM有限，2字节存储一次
			}
			else if(len*2>128)
			{
      
				if(len*2<=256)
				{
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// 踢看门狗
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),len*2-128);
				}
				else if(len*2>256)
				{
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// 踢看门狗
					AT24_Write_page((RF433_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// 踢看门狗
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
				AT24_Write_page((RF315_PRE_ADDR+OffsetAddr*rfs.number),eeprombuf,len*2);//RAM有限，2字节存储一次
			}
			else if(len*2>128)
			{
      
				if(len*2<=256)
				{
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// 踢看门狗
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),len*2-128);
				}
				else if(len*2>256)
				{
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number),eeprombuf,128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// 踢看门狗
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number+128),&(eeprombuf[128]),128);
					for(dword i=0;i<1500;i++){}
					IWDG_ReloadCounter();			// 踢看门狗
					AT24_Write_page((RF315_PRE_ADDR_SPL+OffsetAddr*rfs.number+256),&(eeprombuf[256]),len*2-256);
				}
				for(dword i=0;i<1500;i++){}
				AT24_Write_page((RF315_PRE_ADDR+OffsetAddr*rfs.number),splflage,2);
			}
		}

		enableInterrupts();  //写结束后开中断
		rfs.type=0;
		for(i=0;i<10;i++)     //发送学习成功应答
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
		ledSetParam(1,1);//停止闪烁常亮
	}
	else if(IR.Learnflag==2) //红外学习成功退出
	{
		IR.Learnflag=0;;//超过10秒为学习成功自动退出
		ledSetParam(50,100);//指示灯回复正常闪烁
		rfs.type=0;
		for(i=0;i<10;i++)     //发送学习失败应答
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
	else if(IR.Learnflag==0xff) //学习异常退出
	{
		IR.Learnflag=0;;//超过10秒为学习成功自动退出
		ledSetParam(50,100);//指示灯回复正常闪烁
		rfs.type=0;
		for(i=0;i<10;i++)     //发送学习失败应答
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
//By HeYC 0822		     setSSid=1; //启动WIFI模块stassid设置
     		flagConnetedWithSvr=0; 
     		ledSetParam(10,20);//快速闪烁进入wifi重启
	     flagGotAppCfgInfo=0;

		 //Bgn:By HeYC 0822
		   wifiMDInitflag=0;
	          strcpy(UDPPort,"59999");

		  wifiInitMsg=WIFI_INIT_PRE;//Ensure task will be started correctly

		  wifiATCmdSetStat=WIFI_AT_CMD_GOING;	//By HeYC 0907
                uartFlushReceiveBuffer(COM_WIFI);		//By HeYC 0907
       	   ChangeTask(wifiMonitorTaskID);//启动wifi初始化任务
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
Description: 	   报警任务
Calls: 			
Called By: 		no
Input: 			no
Output: 		state
Return: 		no
Others: 		no
********************************************************************************/

void  alarmRFTask(void)//100ms任务
{
	 int i,j,len;
//by HeYC	 int alarmDate;//报警读取的数据高低判断
//By HeYC	 int rfDate;//flash读取的数据高低判断
//By HeYC	 int rfcmd;//flash读取的8位数据转16位数据
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
 //By HeYC	if((SetOrUnsetState[0]==1)&&((rfInterLock==3)||(rfInterLock==4))&&(IR.REVFLAG==0)&&(rfs.type==0))//处于设防状态,且有报警信号，且不再学习和控制状态
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
				disableInterrupts();  //读之前先关中断
       			AT24__Read_Page((RF315_PRE_ADDR+OffsetAddr*i),readbuf,2);
       			enableInterrupts();  //读结束后开中断
       			break;

			case IRRS_433_RCV_SUCC://rf433
       			disableInterrupts();  //读之前先关中断  
       			AT24__Read_Page((RF433_PRE_ADDR+OffsetAddr*i),readbuf,2);
       			enableInterrupts();  //读结束后开中断
       			break;

			default:
       			break;
		}

		//He:only len is equal, can other thing continue
	    	len=readbuf[0]*256+readbuf[1];
	    	readbuf[0]=0;
	    	readbuf[1]=0;
	    	if(len!=IR.cmd[0])//长度相等，继续判断数据
		    	continue;
			
	    	//{
	    	//He:Why len++? because the first  bytes is saved as length.
	      	len++;
	      	disableInterrupts();  //读之前先关中断
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
	      	enableInterrupts();  //读结束后开中断
	      	//判断数据
	      	//He:below is fucking logic.
	      	//By HeYC 0830	if(len>5)
	      	//By HeYC 0830	{
	       //By HeYC 0830		alarmActive=1;
	      	//By HeYC 0830	}

		//He:The last is the stop flag, addr 0 is len, addr 1 and addr 2 not used
		alarmTriggered=1;// intial with 1, if not equal, will be set to 0;
	      	//By HeYC	for(j=3;j<(len-1);j++)//去除数据长度、引导码、同步码不做判断
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
				( (IR.cmd[j]>=100)&&(IR.cmd[j]<=200) )	)//探头报警数据解码
	       	{
		        	alarmDate=1;//短脉冲至1
	       	}
	       	else
	       	{
	       		alarmDate=0;//长脉冲至0
	       	}

			//He:Check logic 1 or 0 of studied and saved in eeprom.
			rfcmd=eeprombuf[j*2]*256+eeprombuf[j*2+1];//flash数据转换为16位数据
	       	if(	( (rfcmd>=255)&&(rfcmd<=500) ) ||
				( (IR.cmd[j]>=100)&&(IR.cmd[j]<=200) ) )//探头报警数据解码
	       	{
	       		rfDate=1;//短脉冲至1
	       	}
	       	else
	       	{
		       	rfDate=0;//长脉冲至0
	       	}
	       	if(alarmDate!=rfDate)//一个数据不相等跳出本次循环
	       	{
	       		alarmActive=0;//对码失败
	        		break;
	       	}
			//End:commented by HeYC */
	      	}      
	    //}
			
		if(alarmTriggered==1)//对码成功，发送报警信号
	    	{
	    		if(IsAlarmSrcNoTriggered(i)==0){
				//Should report this alarm
				CacheAlarmSrcNoTriggered(i);
				
				alarm.NO=i;//探头编号
				
		     		secModChangedFlag=3;		//By HeYC let loop send out stat report
		     		
		     		curSecuModeShadow=2;	//By HeYC	     		
		     		
		     		//By HeYC 0908	if(alarm.sending==0)		//同一次报警只发送一次, Fucking logic  HeYC
		     		//By HeYC 0908	{
		      		//By HeYC 0908		alarm.sending=1;
		      		//By HeYC 0908		alarm.timeCounter=0;
		      			//报警信号发送，通过发送函数发送探头编号
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
		      			SMSSEND(lenth,sbuffer,4);    //向服务器发送报警信息

			  		//He:this flag will be detected in initTask loops and will be used to send a broadcast in local network
		      			almBrdcstFlag=2;		//broadcast 2 times
				//By HeYC 0908	}
		     		//By HeYC 0908	else
		     		//By HeYC 0908	{
		       	//By HeYC 0908		alarm.timeCounter=0;
		     		//By HeYC 0908	}
		     		//
	    		}
	     		break;//跳出循环80
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
	     	//	rfInterLock=IRRS_IDLE;//解除锁定
		//}

		//He, watch dog should be keeped here??
	}
 //HeYC0830}
 
//By HeYC 0908	if(++alarm.timeCounter>=100)//超过10秒未收到报警信号，报警信号消失，再次触发需报警
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


