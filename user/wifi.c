/*******************************************************************************
*
*   Application 
*   Soft version:   V1.00
*   File Name:      WiFi.c
*   Author   :      zzw
*   creation date:  2015-04-02
*	module description: 
*   Copyright (C) reserve
********************************************************************************/
#include "wifi.h"
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
#include "IR.h"

#define BSIZE_CMD				50
//#define VER_INFO				"GW_V17"
//make it easy to read in IR releated interrupt PortD and PortE
//#define VER_INFO				"GW_V18"
//Redfine the coding compare of alarm mesage triggered
#define VER_INFO				"GW_V19"
char _cmdIsSend = 0;
static int _cmdRepeat = 0;
static int _wifiRet = WIFI_NONE;
int wifiCmdMsg;
u32 wifiTimer;
u16 lenth;
u32 MSMSendTimer;
u32 wifiSendTimer;

char    wifiCmdbuf[100]; //COM?o3???
char    wifiReadDateStep;
char    wifiReadDateCounter;

WORD PORT,PORT1,PORT2,PORT3,PORT4;
WORD  deviceCmd[10];//
//char stareqstrings[sBUFFER_SIZE];//
uchar tcpcharCmds[10];
char sbuffer[sBUFFER_SIZE];
//DWORD  devicelongCmd[10];//
static char line[sBUFFER_SIZE];
u08 flashbuf[sBUFFER_SIZE];
int ret;//zzw 20150519
int wifiInitMsg = WIFI_INIT_PRE;
int uret=0;
//char version[8]={0x57,0x49,0x46,0x49,0x5F,0X56,0x31,0x31};  //'WIFI_V11'
//�����Ƿ������������֮��Ŀ���Э������
//char Authentication[10]={0x48,0x65,0x59,0x1F,0x00,0xE0,0xFF,0x00,0x00,0x01};                  //�����֤Ҫ��ָ��
char Authentication_answer[10]={0x48,0x65,0x59,0x1B,0x00,0xE4,0xFF,0x50,0x00,0x02};           //�����֤Ҫ��Ӧ��ָ��

char Heartbeat[13]={0x48,0x65,0x59,0x03,0x00,0xFC,0xFF,0x51,0x00,0x01,0xAA,0x55,0x43};        //�������ָ��
//char Heartbeat_answer[13]={0x48,0x65,0x59,0x03,0x00,0xFC,0xFF,0x51,0x00,0x02,0xAA,0x55,0x43};  //�������Ӧ��ָ��

//char BasicCommand[10]={0x48,0x65,0x59,0x0E,0x00,0xF1,0xFF,0x52,0x00,0x03};                   //��������ָ��
char BasicCommand_answer[10]={0x48,0x65,0x59,0x0A,0x00,0xF5,0xFF,0x52,0x00,0x04};             //��������Ӧ��ָ��
char Succ=0;  //0:ʧ��,1���ɹ�

//char aircondition[10]={0x48,0x65,0x59,0x12,0x00,0xED,0xFF,0x53,0x00,0x03};                  //�յ�����ָ��
char aircondition_answer[10]={0x48,0x65,0x59,0x0A,0x00,0xF5,0xFF,0x53,0x00,0x04};            //�յ�����Ӧ��ָ��

//char CurSetOrUnSet[10]={0x48,0x65,0x59,0x06,0x00,0xF9,0xFF,0x54,0x00,0x03};                //�����������ָ��
char CurSetOrUnSet_answer[10]={0x48,0x65,0x59,0x0B,0x00,0xF4,0xFF,0x54,0x00,0x04};         //���������Ӧ����ָ��

//char GetCurSetOrUnSet[10]={0x48,0x65,0x59,0x03,0x00,0xFC,0xFF,0x55,0x00,0x01};              //��ȡ�������״ָ̬��
char GetCurSetOrUnSet_answer[10]={0x48,0x65,0x59,0x0A,0x00,0xF5,0xFF,0x55,0x00,0x05};       //��ȡ�������״̬Ӧ��ָ��

char ProbeAlarm[10]={0x48,0x65,0x59,0x0B,0x00,0xF4,0xFF,0x56,0x00,0x05};                    //̽ͷ����ָ��
//char ProbeAlarm_answer[13]={0x48,0x65,0x59,0x03,0x00,0xFC,0xFF,0x56,0x00,0x06,0xAA,0x55,0x43};//̽ͷ����Ӧ��ָ��
//������APP�������֮���Э������
//char APPconfigure[10]={0x48,0x65,0x59,0x01,0x00,0xF0,0x02,0x00,0x80,0x03};                    //APP����ָ��   0x01,0x00,0xF0,0x02ָ���ݳ��ȣ�����
char APPconfigure_answer[10]={0x48,0x65,0x59,0x0B,0x00,0xF4,0xFF,0x00,0x80,0x04};             //APP����Ӧ��ָ��

//char APPRestartModule[10]={0x48,0x65,0x59,0x12,0x00,0xED,0xFF,0x01,0x80,0x03};              //APP����ģ��ָ��

char AirTypeTest_answer[10]={0x48,0x65,0x59,0x05,0x00,0xFA,0xFF,0x65,0x00,0x04};         //AA�յ��ͺŲ���Ӧ��ָ��
char KZaircondition_answer[10]={0x48,0x65,0x59,0x05,0x00,0xFA,0xFF,0x66,0x00,0x04};     //APP�յ�����Ӧ��ָ��
//�����Ǿ���������Э������
//char DeviceDiscover[10]={0x48,0x65,0x59,0x03,0x00,0xFC,0xFF,0x60,0x00,0x01};             //�����豸ָ��
char DeviceDiscover_answer[10]={0x48,0x65,0x59,0x0E,0x00,0xF1,0xFF,0x60,0x00,0x02};      //�����豸Ӧ��ָ��

//char WirelearnAndControl[10]={0x48,0x65,0x59,0x08,0x00,0xF7,0xFF,0x61,0x00,0x03};       //����ѧϰ�����ָ��
char WirelearnAndControl_answer[10]={0x48,0x65,0x59,0x06,0x00,0xF9,0xFF,0x61,0x00,0x04};       //����ѧϰ�����Ӧ��ָ��

//char KZCurSetOrUnSet[10]={0x48,0x65,0x59,0x06,0x00,0xF9,0xFF,0x62,0x00,0x03};                //�����������ָ��
//char KZCurSetOrUnSet_answer[10]={0x48,0x65,0x59,0x0A,0x00,0xF5,0xFF,0x62,0x00,0x04};         //�����������Ӧ��ָ��

//char GetKZCurSetOrUnSet[10]={0x48,0x65,0x59,0x03,0x00,0xFC,0xFF,0x63,0x00,0x01};              //��ȡ�������״ָ̬��
char GetKZCurSetOrUnSet_answer[10]={0x48,0x65,0x59,0x0A,0x00,0xF5,0xFF,0x63,0x00,0x05};       //��ȡ�������״̬Ӧ��ָ��

char KZProbeAlarm[10]={0x48,0x65,0x59,0x0B,0x00,0xF4,0xFF,0x64,0x00,0x05};                    //̽ͷ�����㲥ָ��

char RCR_ETX[3]={0xC1,0xC2,0x43};   //֡β

void UpdateMacIDHex();
//Bgn:By HeYC 0906
void InitCfgParaBeforeUpdate(){

	memset(&gCfgPara.SSIDToJoin[0],0,20);

	memset(&gCfgPara.SSIDPwd[0],0,20);

	memset(&gCfgPara.serverAddr[0],0,20);

	memset(&gCfgPara.serverPort[0],0,10);	
}
//End:By heYC 09906
/*******************************************************************************
Function: 		ZigbeeReadResponse()
Description: 	͸������  �� UART1��wifiģ���ж�ȡ��������жϲ����ض�Ӧ��Ӧ��
Calls: 			
Called By: 		
Input: 			
				
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
u08 wifiReadResponse(void)
{
   int i;
   u08 cmdBuf[5];
   u08 cmdLength=4;//Ĭ��4�ֽ�
   u08 intflash[40]={0};
 // char strings[sBUFFER_SIZE]={0};
   char profilecmds[sBUFFER_SIZE];
   char stringstemp[20];
   char *p;
   u08  readPos=0;//By heYC 0906
   u08  tmpLength=0;//By HeYC 0906
  // DWORD time;
  // DWORD temp;

       // memset(clock,0,sBUFFER_SIZE);
        memset(sbuffer,0,sBUFFER_SIZE);
        memset(profilecmds,0,sBUFFER_SIZE);
        memset(line,0,sBUFFER_SIZE);//����������
        
	uartwifiTimer = TIMING_MONITOR_WIFI;//���ó�ʱ������
       
        lenth=uartReadLineWifi(COM_WIFI, line, sBUFFER_SIZE);//��ȡ���ڻ���������
     
	for(i=0;i<sBUFFER_SIZE;i++)
        {
         if(line[i]!='*')//�ַ�����**��β
         {
          profilecmds[i]=line[i];//��������
         }
         else
         {
          profilecmds[i]='\0';
          break;
         }
         
        }
    
        //strcpy(profilecmds,line);//
        p=strchr(line,':');     //����line���״γ��֡������ĵط�
        strcpy(profilecmds,(p+1));//���ݿ������������ //�Ѵ�p+1��ַ��ʼ�Һ���NULL���������ַ������Ƶ�profilecmds��ʼ�ĵ�ַ�ռ�,p+1��profilecmds��ָ�ڴ����򲻿����ص���profilecmds�������㹻�Ŀռ�������p+1���ַ���,����ָ��profilecmds��ָ��
        decodestring(profilecmds);////�������ݣ�����Э���޸�
       // uartSendBuffer(COM_WIFI, line, lenth);//test zzw 20150424
	if (lenth > 0) 
	{
		//debugTrace(COM_DEBUG,"Zigbee rcvd: ");
		//debugTrace(COM_DEBUG,line);
		//if(CmdIsSend())	//����ظ�
		//{
		 // _cmdIsSend = 0;
        if((strstr(line, "OK"))||(strstr(line, ">"))) ////(strstr(line, "SEND OK"))ģ���ATָ����Ӧ������
		  {
		     if(strstr(line, "SEND OK"))//���ݷ��ͳɹ�   //strstr() ��������һ���ַ�������һ���ַ����еĵ�һ�γ��֡��ú��������ַ��������ಿ�֣���ƥ��㣩�����δ�ҵ����������ַ������򷵻� false
                {
                  if(smsSMSStep==SMS_SD_SEND_REV)
                     {
                        _wifiRet = WIFI_OK;
                     }
                  else
                     {
                       ;//�ų����ݷ���Ӧ�����
                     }
                      
                 }
              else
                 {
                    _wifiRet = WIFI_OK;
                 }
                     
              if((strcmp(line, ">") == 0))//���wifiģ���������ݷ��ͱ�־λ
                 {
                   waitsmssend=1;
                 }
			
		  }
        else if(strncmp(line, "+CIFSR:APMAC",12)==0)//��ȡģ��MacID //extern int strcmp(char *s1,char * s2��int n);
          {//+CIFSR:APMAC,"1a:fe:34:a1:58:4a"                        //���ܣ��Ƚ��ַ���s1��s2��ǰn���ַ���
             p=strchr(line,'"');                                        //��s1<s2ʱ������ֵ<0����s1=s2ʱ������ֵ=0�� ��s1>s2ʱ������ֵ>0
             gCfgPara.macAddr[0]=p[1];
             gCfgPara.macAddr[1]=p[2];
             gCfgPara.macAddr[2]=p[4];
             gCfgPara.macAddr[3]=p[5];
             gCfgPara.macAddr[4]=p[7];
             gCfgPara.macAddr[5]=p[8];
             gCfgPara.macAddr[6]=p[10];
             gCfgPara.macAddr[7]=p[11];
             gCfgPara.macAddr[8]=p[13];
             gCfgPara.macAddr[9]=p[14];
             gCfgPara.macAddr[10]=p[16];
             gCfgPara.macAddr[11]=p[17];
             //By HeYC gCfgPara.macAddr[12]='\0';
//by HeYC 0906             write_flash_array(SW_ID_ADDR,12,(u08 *)&gCfgPara.macAddr);//����ģ��MAC��ַ��Ϊ����ID��            
//by HeYC 0906 will be saved into flash later		write_flash_array(SW_ID_ADDR,(u16)sizeof(gCfgPara),(u08*)&gCfgPara);
		UpdateMacIDHex();
           }
        else if(strncmp(line, "+CIFSR:STAIP",12)==0)//��ȡstaģʽIP
                  {//+CIFSR:APMAC,"0.0.0.0"
                    p=strchr(line,'"');
                    if(p[1]=='0')//δ������·����
                    {
                     STAEnable=0;
                    }
                    else
                    {
                     STAEnable=1;
                    }
                  }
        
		else //�������Ԥ��
		  {
		   // _wifiRet = WIFI_OK;
             if(strncmp(line,"+IPD",4)==0)//���� +IPD,0,16:......
               {
                 p=strchr(line,',');
                 if((p[1] <= '9') && (p[1] >= '0'))
                   {
    	              ClientID = p[1] - '0';
                   }
                 else 
                   {
  	                  ClientID = p[1] - 'A' + 10;  
                   }
                 memset(profilecmds,0,sBUFFER_SIZE);
                 p=strchr(line,':');
                // strcpy(clock,(p+1));//���ݿ������������,��ʱ�� 
                 strcpy(profilecmds,(p+1));//���ݿ������������ 
                 if(strncmp(profilecmds,"?WSWITCH",8)==0)//���Ƽ̵���״̬   //�����￪ʼ�ж��յ������������Ӧ�Ĵ���
                   {//!SWITCH#< state >**
                     if(deviceCmd[1]==0)
		               {
		                 // REALY_OFF();
//by heyc 0906	                          keystate=0;
                          ledSetParam(50,100);
		               }
		             else//
		               {
                        // REALY_ON();
//by heyc 0906	                          keystate=1;
                          ledSetParam(100,100);
                       }
                     strcpy(sbuffer,"!SWITCH#");
                     strcat(sbuffer,gCfgPara.macAddr);
                     strcat(sbuffer,"#");
                     memset(stringstemp,0,20);
                     sprintf(stringstemp,"%u",deviceCmd[1]);//
                     strcat(sbuffer,stringstemp);//
                     strcat(sbuffer,"**\r\n");	
                     lenth=strlen(sbuffer);
                     SMSSEND(lenth,sbuffer,ClientID);
                   }
                 else  if(strncmp(profilecmds,"?RSWITCH",8)==0)//��ѯ�̵���״̬
                   {
//by heyc 0906	                     strcpy(sbuffer,"!SWITCH#");
//by heyc 0906	                     strcat(sbuffer,gCfgPara.macAddr);
//by heyc 0906	                     strcat(sbuffer,"#");
//by heyc 0906	                     memset(stringstemp,0,20);
//by heyc 0906	                     sprintf(stringstemp,"%u",keystate);//
//by heyc 0906	                     strcat(sbuffer,stringstemp);//
//by heyc 0906	                     strcat(sbuffer,"**\r\n");	
//by heyc 0906	                     lenth=strlen(sbuffer);
//by heyc 0906	                     SMSSEND(lenth,sbuffer,ClientID);                   
                   }
                 else if(strncmp(profilecmds,"?GETSWID**",8)==0)//����?GETSWID**���������� macid "1a:fe:34:a1:58:4a"
                   {  //? GETSWID**
                       //!SWID#<id>**
                      strcpy(sbuffer,"!SWID#");
                      strcat(sbuffer,gCfgPara.macAddr);
                      strcat(sbuffer,"**\r\n");	
                      lenth=strlen(sbuffer);
                      SMSSEND(lenth,sbuffer,2); 
                      //SMSSEND(8,qbuffer,ClientID);
                      
                    }
                  else if(strncmp(profilecmds,"?ATCWJAP",8)==0)//
                    {  //?ATCWJAP#<ssid>#<password>**
                      //!CWJAP**
                      //����WIFIģ�����ӵ�·����
//By HeYC 0822	                      setSSid=1;
                      flagConnetedWithSvr=0;
                      strcpy(sbuffer,"!CWJAP**");
                      strcat(sbuffer,"\r\n");	
                      lenth=strlen(sbuffer);
                      SMSSEND(lenth,sbuffer,ClientID);
                    }
                  else if(strncmp(profilecmds,"?ATCWSAP",8)==0)//
                    {  //?ATATCWSAP#<ssid>#<password>**
                      //!CWSAP**
//By HeYC 0822	                      apsetSSid=WIFI_AP_SET;
//by HeYC 0907	                      ATCWSAPEnable=1;
                      strcpy(sbuffer,"!CWSAP**");
                      strcat(sbuffer,"\r\n");	
                      lenth=strlen(sbuffer);
                      SMSSEND(lenth,sbuffer,ClientID);
                    }
                  else if(strncmp(profilecmds,"?ATSTA",6)==0)//
                    {  //?ATSTA#<IP>#<PORT>**
                      //!STA**
                      strcpy(sbuffer,"!STA**");
                      strcat(sbuffer,"\r\n");	
                      lenth=strlen(sbuffer);
                      SMSSEND(lenth,sbuffer,ClientID);
                    }
                 
                  else if(strstr(line, "4,CLOSED"))//
                  {
                    Authenticationflag=0;
					HFlagWhereAuthSetZero=5;
                    wifiCmdCIPSTART(gCfgPara.serverAddr,gCfgPara.serverPort,1);
                  }
                  else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x00)&&(line[18]==0x00)&&(line[19]==0x01))// �����������֤Ҫ����Ӧ��   (������)
                    {  
                      //strcpy(sbuffer,Authentication_answer);
                      for(i=0;i<10;i++)
                         {
                           sbuffer[i]=Authentication_answer[i];
                         }
                      strcat(&(sbuffer[10]),"Topan2014");
                      strcat(&sbuffer[19],VER_INFO);
                      sbuffer[25]=0x00;
                      sbuffer[26]=0x00;
                      for(i=27;i<33;i++)
                         {
                           sbuffer[i]=macidHEX[i-27];
                         }
                      //strcat(&sbuffer[27],macidHEX);
                      sbuffer[33]= curSecuModeShadow;//By HeYC from SetOrUnsetState[0] to SetOrUnsetStateflag;  //�ϱ�����״̬
                      strcat(&sbuffer[34],RCR_ETX);
                      
                     // strcat(&sbuffer[34],"\r\n");
                      lenth=37;//strlen(sbuffer);
                      
                      SMSSEND(lenth,sbuffer,4);
                      Auto_WIFI_Send();//��������
                      
                      Authenticationflag=1;  //�����������֤ͨ����־λ����ʼ����
                      HFlagWhereAuthSetZero=0;
                      Heartbeattimer5s_count=0;//By HeYc
                      flagConnetedWithSvr=1;//By HeYC
                      sysTickfor8s=0;
                    }
                 // else if(strncmp(profilecmds,BasicCommand,10)==0)// ���������ͻ��������Ӧ��   (������)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x52)&&(line[18]==0x00)&&(line[19]==0x03))
                    {  
                      //strcpy(sbuffer,Authentication_answer);
                      rfs.number=line[26]*256+line[27];//��ֵ
                      CNT=line[28];//����
                      if(line[25]==0x02)//����ָ��
                      {
                        if((line[24]==0x02)||(line[24]==0x03))  //433 or 315
                        {
                          rfs.state=1;//��ʾ����  
                        }
                        else if(line[24]==0x01)    //����
                        {
                          cmdBuf[0]=0x86;
                          cmdBuf[1]=(u08)(rfs.number&0x00FF);
                          cmdBuf[2]=0x00;
                          cmdBuf[3]=0x00;
                          cmdBuf[4]=XOR(cmdBuf,cmdLength);
                          uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���Ƶ�����ͳ�ȥ
                          
                          for(int i=0;i<10;i++)     //���Ϳ��Ƴɹ�Ӧ��
                          {
                            sbuffer[i]=WirelearnAndControl_answer[i];
                          }
                          sbuffer[10] = rfs.number>>8;//keyValH 
                          sbuffer[11] = rfs.number&0xff;//keyValL 
                          sbuffer[12] = 1;
                          strcat(&sbuffer[13],RCR_ETX);
                          // strcat(&sbuffer[16],"\r\n");
                          lenth=16;//strlen(sbuffer);
                          SMSSEND(lenth,sbuffer,4);
                        }
                          
                      }
                      if(line[24]==0x01)
                      {
                        //����
                      }
                      else if(line[24]==0x02)
                      {
                          rfs.type=1;//��315M
                      }
                      else if(line[24]==0x03)
                      {
                        rfs.type=2; //��433M
                      }
                      
                      send_flag=1;   //���Ƶ�ʱ�������ж��ǻظ���������Ӧ����APP��Ӧ��1��ʾ�ظ���������2��ʾ�ظ�APP
                      if(rfs.state==1)
                      {
                       IR_Send(rfs.number);
                      } 

                    }  
                
                // else if(strncmp(profilecmds,aircondition,10)==0)// �������յ����������Ӧ��   (������)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x53)&&(line[18]==0x00)&&(line[19]==0x03))
                    {  
                      //strcpy(sbuffer,Authentication_answer);
                      
                      air.step=IR_SEND_PRE;//����������Ҫ����
                      air.func=line[24];  
                      air.codeValH=line[25];
                      air.codeValL=line[26];
                      air.TimeMin=line[27];  
                      air.onoff=line[28];
                      air.mode=line[29];
                      air.temp=line[30];
                      air.windspeed=line[31];
                      air.winddir=line[32];
                      if(air.func==0x03)//������03����Ч
                      {
                        air.enable=1;//����������Ҫ����
                        air.codeask=0;//�յ��ͺ�δ����
                        air.cmdsetp=0;
                        
                      }
                     
                    }  
                 // else if(strncmp(profilecmds,CurSetOrUnSet,10)==0)// ����������������������Ӧ��   (������)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x54)&&(line[18]==0x00)&&(line[19]==0x03))
                    {  
                      //strcpy(sbuffer,Authentication_answer);
                      /*Bgn:Commented by HeYC
                      for(i=0;i<10;i++)
                      {
                         sbuffer[i]=CurSetOrUnSet_answer[i];
                      }
                      for(i=10;i<16;i++)
                      {
                         sbuffer[i]=macidHEX[i-10];
                      }
                      //strcat(&sbuffer[10],macidHEX);
                      sbuffer[16] = 1;
			//End:Commented by HeYC */
			//Bgn:By HeYC
			 
			  //if(SetOrUnsetStateflag!=line[22]){
					 curSecuModeShadow=line[22];
				//	SetOrUnsetState[0]=SetOrUnsetStateflag;
				//	sbuffer[17] =SetOrUnsetStateflag;
                        	//	write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);//���볷��״̬

			  //}
			  secModChangedFlag=2;//let loop send out stat report
			  //else{
			  	//Test
			  //	SetOrUnsetStateflag=SetOrUnsetStateflag;
			  //}
			  //End:By HeYC
			  /*Bgn:Commented by HeYC
                      if(line[22]==0x00)  //��������ʹ����Ҳ��������
                      {
                        SetOrUnsetState[0]=0;
                        sbuffer[17] =0;
                        write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);//���볷��״̬
                      }
                      else if(line[22]==0x01)  //���
                      {
                        SetOrUnsetState[0]=1;
                        sbuffer[17] =1;
                        write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);//�������״̬
                      }
                      
                      strcat(&sbuffer[18],RCR_ETX);
                    //  strcat(&sbuffer[21],"\r\n");
                      lenth=21;//strlen(sbuffer);
			SMSSEND(lenth,sbuffer,4);
			//End:Commented by HeYC */
                    }
                
                 //else if(strncmp(profilecmds,ProbeAlarm_answer,13)==0)// ������̽ͷ����Ӧ��   (������)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x56)&&(line[18]==0x00)&&(line[19]==0x06))
                    {  
                      //�˴���һ��־λ����ʾ̽ͷ�������ͳɹ�
                    }
                // else if(strncmp(profilecmds,APPconfigure,10)==0)// APP����ģ��ָ���Ӧ�� (APP)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x00)&&(line[18]==0x80)&&(line[19]==0x03))
                    {  
                    //He:0906save cfg data into flash...
                    //-----------------------------------------------------------------------------------------------------------------
                    //-----------------------------------------------------------------------------------------------------------------
                        
			InitCfgParaBeforeUpdate();
			//SSID len and context
			tmpLength=line[29];
			readPos=30;
			for(i=0;i<tmpLength;i++){
				gCfgPara.SSIDToJoin[i]=line[readPos++];
			}
			//PWD len and context
			tmpLength=line[readPos++];
			for(i=0;i<tmpLength;i++){
				gCfgPara.SSIDPwd[i]=line[readPos++];
			}
			//server addr len adn context
			tmpLength=line[readPos++];
			for(i=0;i<tmpLength;i++){
				gCfgPara.serverAddr[i]=line[readPos++];
			}
			//Server port and context
			tmpLength=line[readPos++];
			for(i=0;i<tmpLength;i++){
				gCfgPara.serverPort[i]=line[readPos++];
			}

			write_flash_array(SW_ID_ADDR,(u16)sizeof(gCfgPara),(u08*)&gCfgPara);
			
                      //By HeYC 0906	write_flash_array(SW_SERVICE_IP_ADDR,40,(u08 *)intflash);//����ԭ�ȵ�����
                      //By HeYC 0906	write_flash_array(SW_SSID_ADDR,40,(u08 *)intflash);//����ԭ�ȵ�����
                      //By HeYC 0906	memset(gCfgPara.serverAddr,0,20);
                      //By HeYC 0906	memset(gCfgPara.serverPort,0,10);
                      //By HeYC 0906	memset(ssid,0,20);
                      //By HeYC 0906	memset(ssidPassword,0,20);
                      //By HeYC 0906	for(i=0;i<line[29];i++)
                      //By HeYC 0906	{
                      //By HeYC 0906	  ssid[i]=line[30+i];        //����·������
                      //By HeYC 0906	}
                      //By HeYC 0906	write_flash_array(SW_SSID_ADDR,20,(u08 *)ssid);//����SSID
 			//By HeYC 0906	
                      //By HeYC 0906	for(i=0;i<line[30+line[29]];i++)
                      //By HeYC 0906	{
                      //By HeYC 0906	  ssidPassword[i]=line[(31+line[29])+i];             //����·��������
                      //By HeYC 0906	}
                      //By HeYC 0906	write_flash_array(SW_SSIDPASSWORD_ADDR,20,(u08 *)ssidPassword);//����SSID
                                                                
                      //By HeYC 0906	for(i=0;i<line[(31+line[29])+line[(30+line[29])]];i++)
                      //By HeYC 0906	{
                      //By HeYC 0906	  gCfgPara.serverAddr[i]=line[(31+line[29])+line[(30+line[29])]+1+i];  //���շ�����IP��ַ
                      //By HeYC 0906	}
                      //By HeYC 0906	write_flash_array(SW_SERVICE_IP_ADDR,20,(u08 *)gCfgPara.serverAddr);//����Զ�̷�����IP
                     
                      
                     /* PORT1=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+2];
                      PORT2=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+3];
                      PORT3=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+4];
                      PORT4=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+5];
                      PORT=(PORT4<<24)+(PORT3<<16)+(PORT2<<8)+PORT1;
                      sprintf(staClientPort, "%u", PORT);*/
                      //By HeYC 0906	gCfgPara.serverPort[0]=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+2];
                      //By HeYC 0906	gCfgPara.serverPort[1]=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+3];
                      //By HeYC 0906	gCfgPara.serverPort[2]=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+4];
                      //By HeYC 0906	gCfgPara.serverPort[3]=line[(31+line[29])+line[(30+line[29])]+line[(31+line[29])+line[(30+line[29])]]+5];
                     
                      //By HeYC 0906	write_flash_array(SW_SERVICE_PORT_ADDR,10,(u08 *)gCfgPara.serverPort);//����Զ�̷������˿ں�
                                  
                      //By HeYC move to ResponseToAppCfg	for(i=0;i<10;i++)
                      //By HeYC move to ResponseToAppCfg	   {
                      //By HeYC move to ResponseToAppCfg	     sbuffer[i]=APPconfigure_answer[i];
                      //By HeYC move to ResponseToAppCfg	   }
                      //By HeYC move to ResponseToAppCfg	sbuffer[10] = 1;
                      //By HeYC move to ResponseToAppCfg	for(i=11;i<17;i++)
                      //By HeYC move to ResponseToAppCfg	{
                      //By HeYC move to ResponseToAppCfg	   sbuffer[i]=macidHEX[i-11];
                      //By HeYC move to ResponseToAppCfg	}
                     //By HeYC move to ResponseToAppCfg	// strcat(&sbuffer[11],macidHEX);
                      //By HeYC move to ResponseToAppCfg	sbuffer[17] = 0x04;
                      //By HeYC move to ResponseToAppCfg	strcat(&sbuffer[18],RCR_ETX);
                     // strcat(&sbuffer[21],"\r\n");
                      //By HeYC move to ResponseToAppCfg	lenth=21;//strlen(sbuffer);
                      //By HeYC move to ResponseToAppCfg	SMSSEND(lenth,sbuffer,2);
                      //By HeYC move to ResponseToAppCfg	Auto_WIFI_Send();//��������
                       flagGotAppCfgInfo=1;
                       sTimeout(&gAppCfgSuccTmout, 0);
			ledSetParam(1,1);
                      
                    }
                // else if(strncmp(profilecmds,APPRestartModule,10)==0)// APP����ģ��ָ��  (APP)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x01)&&(line[18]==0x80)&&(line[19]==0x03))
                    {  
                      if(strncmp(&line[29],macidHEX,6)==0)
                      {
                        ChangeTask(wifiResetTaskID); // ��������ģ������
                      }
                    }
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x65)&&(line[18]==0x00)&&(line[19]==0x03))//APP�յ��ͺŲ���ָ���Ӧ��
                    { 
                      air.step=IR_SEND_PRE;//����������Ҫ����
                      air.codeValH=line[20];//
                      air.codeValL=line[21];//
                      air.onoff=0xFF;//����
                      air.enable=3;//����������Ҫ����
                      air.codeask=0;//�յ��ͺ�δ����
                      air.cmdsetp=0;  
                      
                     /* for(i=0;i<10;i++)
                         {
                           sbuffer[i]=AirTypeTest_answer[i];
                         }
                      sbuffer[10] = line[20];   //KeyValH
                      sbuffer[11] = line[21];    //KeyValL
                      strcat(&sbuffer[12],RCR_ETX);
                    //  strcat(&sbuffer[23],"\r\n");
                      lenth=15;//strlen(sbuffer);
                      SMSSEND(lenth,sbuffer,ClientID);*/
                    }
                  else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x66)&&(line[18]==0x00)&&(line[19]==0x03))//APP�յ����Ƽ���Ӧ��
                    {  
                      //strcpy(sbuffer,Authentication_answer);
                      
                      air.step=IR_SEND_PRE;//����������Ҫ����
                     // air.func=line[24];
                      air.codeValH=line[20];//
                      air.codeValL=line[21];//
                     // air.TimeMin=line[27];
                      air.onoff=line[22];//
                      air.mode=line[23];//
                      air.temp=line[24];//
                      air.windspeed=line[25];
                      air.winddir=line[26];
                      air.enable=2;//����������Ҫ����
                      air.codeask=0;//�յ��ͺ�δ����
                      air.cmdsetp=0;   
                    }  
//He:local control begin-----------------------------------------------------------------------------------------------------
                // else if(strncmp(profilecmds,DeviceDiscover,10)==0)// �豸����ָ���Ӧ�𣨾�������
                  else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x60)&&(line[18]==0x00)&&(line[19]==0x01))
                    {  

					flagReplyDevFind=2;
                      //By HeYC 0907	for(i=0;i<10;i++)
                      //By HeYC 0907   {
                      //By HeYC 0907     sbuffer[i]=DeviceDiscover_answer[i];
                      //By HeYC 0907   }
                      //By HeYC 0907for(i=10;i<16;i++)
                      //By HeYC 0907{
                      //By HeYC 0907   sbuffer[i]=macidHEX[i-10];
                      //By HeYC 0907}
                      //strcat(&sbuffer[10],macidHEX);
                      //By HeYC 0907sbuffer[16] = 0x31;
                      //By HeYC 0907sbuffer[17] = 0x31;
                      //By HeYC 0907sbuffer[18] = 0x66;
                      //By HeYC 0907sbuffer[19] = 0x66;
                      //By HeYC 0907sbuffer[20] = curSecuModeShadow;//By HeYC from SetOrUnsetState[0] to SetOrUnsetStateflag
                      //By HeYC 0907strcat(&sbuffer[21],RCR_ETX);
                      //By HeYC 0907//  strcat(&sbuffer[23],"\r\n");
                      //By HeYC 0907lenth=24;//strlen(sbuffer);
                      //By HeYC 0907SMSSEND(lenth,sbuffer,2);
                    }
                 //else if(strncmp(profilecmds,WirelearnAndControl,10)==0)// ����ѧϰ�����ָ���Ӧ�𣨾�������
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x61)&&(line[18]==0x00)&&(line[19]==0x03))
                    {
                    	rfs.number=line[22]*256+line[23];//��ֵ
                     CNT=line[24];  //����
                     send_flag=2;     //���Ƶ�ʱ�������ж��ǻظ���������Ӧ����APP��Ӧ��1��ʾ�ظ���������2��ʾ�ظ�APP
                     if(line[20]==0x01)//����
                     {
                     }
                     else if(line[20]==0x02)  //315M
                     {
                     	//315M
                     	rfs.type=1;
			}
                     else if(line[20]==0x03)  //433M
                     {
                     	//433M
                     	rfs.type=2;
			}

                      if(line[21]==0x01)  //ѧϰ
                      {
                      	ledSetParam(10,20);//ѧϰ�ڼ������˸
                        	sTimeout(&TimeOut, 0);
                        	if(line[20]==0x01)//����
                        	{
                          		IR.REVFLAG=RF_TYPE_INFRA;
	                          	cmdBuf[0]=0x88;
                          		cmdBuf[1]=(u08)(rfs.number&0x00FF);
                          		cmdBuf[2]=0x00;
                          		cmdBuf[3]=0x00;
                          		cmdBuf[4]=XOR(cmdBuf,cmdLength);
                          		uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫѧϰ������ͳ�ȥ
                          	}
                        	else if(line[20]==0x02)//315M
                        	{
                          		IR_Decoding(1,rfs.number);
                        	}
                        	else if(line[20]==0x03)//433M
                        	{
                          		IR_Decoding(2,rfs.number);
                        	}
                        	sTimeout(&TimeOut, 0);  //10Sѧϰʱ�䣬��ʱ�˳�
                      }                      
                      else if(line[21]==0x02)  //����
                      {
                        	if((line[20]==0x02)||(line[20]==0x03))//315��433
                        	{
                          		rfs.state=1;//��ʾ����
                        	}
                        	else if(line[20]==0x01)  //����
                        	{
                          		cmdBuf[0]=0x86;
                          		cmdBuf[1]=(u08)(rfs.number&0x00FF);
                          		cmdBuf[2]=0x00;
                          		cmdBuf[3]=0x00;
                          		cmdBuf[4]=XOR(cmdBuf,cmdLength);
                          		uartSendBuffer(COM_IRBALL, (char *)(cmdBuf), 5); //�ѽ�Ҫ���Ƶ�����ͳ�ȥ
                          
                          		for(int i=0;i<10;i++)     //���Ϳ��Ƴɹ�Ӧ��
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
                      } 

                      if(rfs.state==1)
                      {
                       	IR_Send(rfs.number);
                      }                     
                      
                 }
               //  else if(strncmp(profilecmds,KZCurSetOrUnSet,10)==0)// ����������������Ӧ�� ����������
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x62)&&(line[18]==0x00)&&(line[19]==0x03))
                    {  
                    	//Bgn:By HeYC

			  //if(SetOrUnsetStateflag!=line[22]){
					  curSecuModeShadow=line[22];
			//		SetOrUnsetState[0]=SetOrUnsetStateflag;
			//		sbuffer[17] =SetOrUnsetStateflag;
                      //  		write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);//���볷��״̬                        		
			  //}	
			  secModChangedFlag=2;//let loop send out stat report
			  
			  //End:By HeYC
                    /*Bgn:Commented by HeYC
                      //strcpy(sbuffer,Authentication_answer);
                     if(line[22]==0x00)  //��������ʹ����Ҳ��������
                      {
                        SetOrUnsetState[0]=0;
                        write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);//���볷��״̬
                      }
                      else if(line[22]==0x01)  //���
                      {
                        SetOrUnsetState[0]=1;
                        write_flash_array(SW_SETorUNSETSTATE,1,(u08 *)SetOrUnsetState);//�������״̬
                      }
                     
                      for(i=0;i<10;i++)
                         {
                           sbuffer[i]=GetKZCurSetOrUnSet_answer[i];
                         }
                     for(i=10;i<16;i++)
                      {
                         sbuffer[i]=macidHEX[i-10];
                      }
                     // strcat(&sbuffer[10],macidHEX);
                      sbuffer[16] =SetOrUnsetState[0];
                      strcat(&sbuffer[17],RCR_ETX);
                     // strcat(&sbuffer[20],"\r\n");
                      lenth=20;//strlen(sbuffer);
              	SMSSEND(lenth,sbuffer,2);
				//End:Commented by HeYC*/
                    }
                
                // else if(strncmp(profilecmds,Heartbeat_answer,13)==0)// ������������⼰��Ӧ��   (������)
                 else if((line[10]==0x48)&&(line[11]==0x65)&&(line[12]==0x59)&&(line[17]==0x51)&&(line[18]==0x00)&&(line[19]==0x02))
                    {  
                      Heartbeattimer5s_count=0;
                    }
                
               }   
	   }
	}
  return 1;	
}
/*******************************************************************************
 Function: 		replace()
 Description: 	           �ַ����滻����
 Called By: 		no
 Input: 			 

 Output: 		no
 Return: 		no
 Others: 		no
 
 ********************************************************************************/
void replace(char originalString[], char key[], char swap[])
{
    int lengthOfOriginalString, lengthOfKey, lengthOfSwap, i, j , flag;
    char tmp[sBUFFER_SIZE];
    
    //
    lengthOfOriginalString = strlen(originalString);
    lengthOfKey = strlen(key);
    lengthOfSwap = strlen(swap);
    
    for( i = 0; i <= lengthOfOriginalString - lengthOfKey; i++){
        flag = 1;
        //
        for(j  = 0; j < lengthOfKey; j ++){
            if(originalString[i + j] != key[j]){
                flag = 0;
                break;
            }
        }
        //
        if(flag){
            strcpy(tmp, originalString);
            strcpy(&tmp[i], swap);
            strcpy(&tmp[i + lengthOfSwap], &originalString[i  + lengthOfKey]);
            strcpy(originalString, tmp);
            i += lengthOfSwap - 1;
            lengthOfOriginalString = strlen(originalString);
        }
    }
}
/*******************************************************************************
Function: 		decodestring()
Description: 	�ָ��ַ������ݣ�����Э���޸�
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no

********************************************************************************/
void decodestring (char *str)
{
  
    const char* split = "#";//
    char * p;
    char counter;//,num;
    char string[sBUFFER_SIZE];
    int i;
    int strlenth;

    
    //printf ("old:%s\n",str);
    //num=countChar(str,'#');
    p=NULL;
    while(strstr(str, "##")!=NULL)
    {
     replace(str, "##", "#|#");
    }
    p = strtok(str,split);//
    counter=0;
    while(p!=NULL)
    {
        
        switch(counter)
        {
            case 0:
                
                //printf ("0:%s\n",p);
                strlenth=strlen(p);
                if(strlenth<sBUFFER_SIZE)
                {
                    strcpy(string,p);

                }
                
                break;
            case 1:
		//if(strcmp(string,"?PRESET")==0)
                //{
                    if(strcmp(p,"|")!=0)
                    {
                        deviceCmd[1]=atoi(p);//
                        //devicelongCmd[1]=atol(p);
                        tcpcharCmds[1]=p[0];
                    }
                    else
                    {
                        deviceCmd[1]=0xffff;
                    }
                   
                    if(strncmp(string,"?ATCWJAP",8)==0)//·�����˺�
                    {
                     strcpy(gCfgPara.SSIDToJoin,p);
                    }
                    if(strncmp(string,"?ATCWSAP",8)==0)//·�����˺�
                    {
                     strcpy(apssid,p);
                    }
                     //revmacid
                    if((strncmp(string,"?ENBALE_PROTECTION",18)==0)||((strncmp(string,"?ENBALE_KEY",11)==0))||((strncmp(string,"?SWCHANGEPASSWD",15)==0)))
                    {
                     //by heyc 0906	strcpy(revmacid,p);
                    }
                    //?ATSTA
                    if(strncmp(string,"?ATSTA",6)==0)//
                    {
                     strcpy(gCfgPara.serverAddr,p);
                    }
               // }
               // else
               // {
               //     strcpy(stareqstrings,p);//sta
               // }
                break;
            case 2:
               if(strcmp(p,"|")!=0)
                {
		           deviceCmd[2]=atoi(p);
                   //devicelongCmd[2]=atol(p);
                }
	      else
                {
                   deviceCmd[2]=0xffff;
                }
               if(strncmp(string,"?ATCWJAP",8)==0)//·��������
               {
                 for(i=0;i<20;i++)
                 {
                   if(p[i]=='*')
                   {
                    gCfgPara.SSIDPwd[i]='\0';
                    break;
                   }
                   else
                   {
                    gCfgPara.SSIDPwd[i]=p[i];
                   }
                  
                 }
                 //strcpy(ssidPassword,p);
               }
               if(strncmp(string,"?ATSTA",6)==0)//
               {
                 for(i=0;i<20;i++)
                 {
                   if(p[i]=='*')
                   {
                    gCfgPara.serverPort[i]='\0';
                    break;
                   }
                   else
                   {
                    gCfgPara.serverPort[i]=p[i];
                   }
                  
                 }
                 //strcpy(ssidPassword,p);
               }
               if(strncmp(string,"?ATCWSAP",8)==0)//·��������
               {
                 for(i=0;i<20;i++)
                 {
                   if(p[i]=='*')
                   {
                    apssidPassword[i]='\0';
                    break;
                   }
                   else
                   {
                    apssidPassword[i]=p[i];
                   }
                  
                 }
                // strcpy(apssidPassword,p);
               }
                break;
            case 3:
               if(strcmp(p,"|")!=0)
                {
		   deviceCmd[3]=atoi(p);
                }
		else
                {
                   deviceCmd[3]=0xffff;
                }
                break;
            case 4:
               if(strcmp(p,"|")!=0)
                {
		  deviceCmd[4]=atoi(p);
                }
		else
                {
                  deviceCmd[4]=0xffff;
                }
                break;
            case 5:
               if(strcmp(p,"|")!=0)
                {
		  deviceCmd[5]=atoi(p);
                }
	       else
                {
                   deviceCmd[5]=0xffff;
                }
                break;
            case 6:
               if(strcmp(p,"|")!=0)
                {
		  deviceCmd[6]=atoi(p);
                }
		else
                {
                   deviceCmd[6]=0xffff;
                }
                break;
            default:
                break;
                
        }
        
        counter++;
        p = strtok(NULL,split);
    }
}

/*******************************************************************************
Function: 		WifiCommand()
Description: 	        ATָ������̣��ַ�����ʽ���ͣ���״̬����ʽ
Calls: 			          ״̬����ʽ����ָ��ִ��ĳһ״̬��ʱ������ifĳһ����������ִ�����Ӧ�Ķ���
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no

********************************************************************************/

int WifiCommand(char* cmd, int answer, int timeout, int repeat)
{
     char buf[100];
     strcpy(buf,cmd);
  //   
     ret= WIFI_NONE;
		
	switch (wifiCmdMsg)
	{
		case CMD_PRE:
			wifiCmdMsg++;
			_cmdIsSend=1;
		        //_RSIsSend=1;//zzw 20150210
			_cmdRepeat = 0;
                        _wifiRet = WIFI_NONE;
			break;
		case CMD_SEND:
			uartPrintln(COM_WIFI, cmd);//2?o???3��??DD
			//debugTrace(COM_DEBUG, "ZIGBEE send: ");
			//debugTrace(COM_DEBUG, cmd);
			_wifiRet = WIFI_NONE;
			sTimeout(&wifiTimer, 0);
			wifiCmdMsg++;
			break;
		case CMD_WAITACK:
			if (_wifiRet == WIFI_OK)
			{
				wifiCmdMsg++;
				sTimeout(&wifiTimer, 0);
                                _wifiRet = WIFI_NONE;//zzw 20150519
			}
			else if (sTimeout(&wifiTimer, timeout) )
			{
				if (++_cmdRepeat < repeat)
				{
					wifiCmdMsg = CMD_REPSLEEP;
				}
				else
				{
					wifiCmdMsg = CMD_PRE;
					ret = WIFI_ERR;
                                        //waitsmssend=0;//�����־λ
					_cmdIsSend = 0;
				}
				sTimeout(&wifiTimer, 0);
			}
			else
			{}
			break;
	   case CMD_SLEEP:
			if (sTimeout(&wifiTimer, 1))
			{
				wifiCmdMsg = CMD_PRE;
				ret = WIFI_OK;
                                //waitsmssend=0;//�����־λ
				_cmdIsSend = 0;
			}
			break;
	   case CMD_REPSLEEP:
			if (sTimeout(&wifiTimer, 20))
			{
				wifiCmdMsg = CMD_SEND;
			}
			break;
		default:
			wifiCmdMsg = CMD_PRE;
                        _wifiRet = WIFI_NONE;
                        //waitsmssend=0;//�����־λ
			break;
	}
	
	return ret;
}

/*******************************************************************************
Function: 		WifiCommand()
Description: 	        ATָ������̣�16�������ݷ�ʽ���ͣ���״̬����ʽ
Calls: 			            ״̬����ʽ����ָ��ִ��ĳһ״̬��ʱ������ifĳһ����������ִ�����Ӧ�Ķ���
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no

********************************************************************************/

int WifiCommandHex(char* cmd, int answer, int timeout, int repeat,int len)
{
 	int ret = WIFI_NONE;
		
	switch (wifiCmdMsg)
	{
		case CMD_PRE:
			wifiCmdMsg++;
			_cmdIsSend=1;
			_cmdRepeat = 0;
                        //_RSIsSend=1;
		        uartFlushReceiveBuffer(COM_WIFI); // 
		        //wifiReadDateStep=WIFI_PRE;//
			break;
		case CMD_SEND:
			uartSendBuffer(COM_WIFI, cmd, len);
			_wifiRet = WIFI_NONE;
			sTimeout(&wifiTimer, 0);
			wifiCmdMsg++;
			break;
		case CMD_WAITACK:
			if (_wifiRet == WIFI_OK)
			{
				wifiCmdMsg++;
				sTimeout(&wifiTimer, 0);
			}
			else if (sTimeout(&wifiTimer, timeout) )
			{
				if (++_cmdRepeat < repeat)
				{
					wifiCmdMsg = CMD_REPSLEEP;
				}
				else
				{
					wifiCmdMsg = CMD_PRE;
					ret = WIFI_ERR;
					_cmdIsSend = 0;
					//_RSIsSend = 0;//zzw 20150210
				}
				sTimeout(&wifiTimer, 0);
			}
			else
			{}
			break;
	   case CMD_SLEEP:
			if (sTimeout(&wifiTimer, 1))
			{
				wifiCmdMsg = CMD_PRE;
				ret = WIFI_OK;
				_cmdIsSend = 0;
			}
			break;
	   case CMD_REPSLEEP:
			if (sTimeout(&wifiTimer, 20))
			{
				wifiCmdMsg = CMD_SEND;
			}
			break;
		default:
			wifiCmdMsg = CMD_PRE;
			break;
	}
	
	return ret;
}
char CmdIsSend(void)
{
	return _cmdIsSend;
}

                      
                      






/*******************************************************************************
Function: 	    SMSSEND(u08 len,u08*  smscontent)
Description: 	�������ݵ����ͻ������������̶�ʱ��ѯ���������ݲ�����
Calls: 			
Called By: 		no
Input: 			
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void  SMSSEND(u16 len,char*  smscontent,u08 clientID)
{
	int i;
//by HeYC 0906	  WIFIENBLE=1;
//by HeYC 0906	  if((WIFIENBLE==1)&&(len!=0))  //
//by HeYC 0906	  {
   	if(++WIFI_Head>=WIFILENGTH)
   	{
		WIFI_Head=0;
   	}
   	WIFISBUF[WIFI_Head].len=len;
   	WIFISBUF[WIFI_Head].clientID=clientID;//���ӵ�ID�û���
   	for(i=0;i<WIFISBUF[WIFI_Head].len;i++)
   	{
    		WIFISBUF[WIFI_Head].buf[i]=smscontent[i];	//
   	}
   
  
//by HeYC 0906	  }
  

}

/*******************************************************************************
Function: 		Auto_WIFI_Send()
Description: 	        �Զ�����WIFI�������������̵���
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no

********************************************************************************/

u8 Auto_WIFI_Send(void)
{
  
  if((WIFI_Head!=WIFI_Hail)&&(WaitMONITOR_IDELSend==0))
     {
	if (++WIFI_Hail >= WIFILENGTH) 
        {
         WIFI_Hail = 0;
        }
       WaitMONITOR_IDELSend=1;
       WIFITxMessage=WIFISBUF[WIFI_Hail];//�˴��ѽ�Ҫ���͵�������WIFISBUF�������͵�buff--WIFITxMessage��
    }
  if(WaitMONITOR_IDELSend==1)
  {
     switch  (smsSMSStep)
      {
        case SMS_SD_PRE:
                  smsSMSStep++;
		  //ledSetParam(10, 30);
	          break;
        case SMS_SD_CMGS:
	          if (wifiCmdCIPSEND((WIFITxMessage.len),WIFITxMessage.clientID, 1)!=WIFI_NONE)//
		  {
                      smsSMSStep++;	 //�ȴ�'>'
                      sTimeout(&MSMSendTimer, 0);
                  }
	          break;
        case SMS_SD_WAIT_CMGS:
                   if(waitsmssend)
                   {
                    smsSMSStep++;//��������
                   }
                   if (sTimeout(&MSMSendTimer, 100))  //1s
                     {
                      smsSMSStep=SMS_SD_FINISH;//��ʱ�˳�
                     }
                  
                  break;
	case SMS_SD_SEND:	
                 //uartFlushReceiveBuffer(COM_WIFI); 
                 _wifiRet = WIFI_NONE;//��ձ�־λ
		 uartSendBuffer(COM_WIFI, (char *)(WIFITxMessage.buf), WIFITxMessage.len); //�ѽ�Ҫ���͵����ݷ��ͳ�ȥ
		 smsSMSStep++;//
	         sTimeout(&MSMSendTimer, 0);
	         break;
        case SMS_SD_SEND_REV:	
                 if((sTimeout(&MSMSendTimer, 200))||(_wifiRet ==WIFI_OK ))  //2s
                 {
                  smsSMSStep++;//
                  _wifiRet = WIFI_NONE;//��ձ�־λ
                  sTimeout(&MSMSendTimer, 0);
                 }
	         break;
        case SMS_SD_WAIT:
                 if (sTimeout(&MSMSendTimer, 10))  //100ms
                 {
                  smsSMSStep++;//
                 }
                 break;
        case SMS_SD_FINISH:
                 smsSMSStep=SMS_SD_PRE;
                 WaitMONITOR_IDELSend=0;
                 uartFlushReceiveBuffer(COM_WIFI); 
                 break;
        default:
                 smsSMSStep=SMS_SD_PRE;
                 WaitMONITOR_IDELSend=0;
                 break;
      }
  
  }
 return 1;
}

//Bgn:By HeYC 0906
void ResponseToAppCfg(){
	int i=0;
	
	for(i=0;i<10;i++)
       {
       	sbuffer[i]=APPconfigure_answer[i];
        }
        sbuffer[10] = 1;
        for(i=11;i<17;i++)
        {
        	sbuffer[i]=macidHEX[i-11];
         }
         // strcat(&sbuffer[11],macidHEX);
         sbuffer[17] = 0x04;
         strcat(&sbuffer[18],RCR_ETX);
         // strcat(&sbuffer[21],"\r\n");
         lenth=21;//strlen(sbuffer);
         SMSSEND(lenth,sbuffer,2);
         Auto_WIFI_Send();//��������
	
}

void ResponseToDevFind(){
	int i=0;
	
	for(i=0;i<10;i++)
       {
       	sbuffer[i]=DeviceDiscover_answer[i];
	}
       for(i=10;i<16;i++)
       {
       	sbuffer[i]=macidHEX[i-10];
	}
       //strcat(&sbuffer[10],macidHEX);
       sbuffer[16] = 0x31;
       sbuffer[17] = 0x31;
       sbuffer[18] = 0x66;
       sbuffer[19] = 0x66;
       sbuffer[20] = curSecuModeShadow;//By HeYC from SetOrUnsetState[0] to SetOrUnsetStateflag
       strcat(&sbuffer[21],RCR_ETX);
       //  strcat(&sbuffer[23],"\r\n");
       lenth=24;//strlen(sbuffer);
       SMSSEND(lenth,sbuffer,2);
}
//End:By HeYC 0906

/*******************************************************************************
Function: 		wifiMDInit()
Description: 	        WIFIģ���ʼ������
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no

********************************************************************************/

int wifiMDInit(void)
{
	int ret = WIFI_NONE;
	if(wifiMDInitflag==0)    //����������ʼ��
        {
	  switch (wifiInitMsg)
	  {
		case WIFI_INIT_PRE:
			wifiInitMsg++;
			break;
                case WIFI_INIT_CWMODE:                  //����ģ��Ĺ���ģʽ
			if (wifiCmdCWMODE(3) != WIFI_NONE)//
			{
                         wifiInitMsg++;
                        }
			break;
                case WIFI_INIT_RST:
			if (wifiCmdRST(1) != WIFI_NONE)//����ģ�飬��ȴ�5��
			{
                         wifiInitMsg++;
                         sTimeout(&wifiSendTimer, 0);
                        }
			break; 
                case WIFI_INIT_WAIT:
                       if (sTimeout(&wifiSendTimer, 300))  //5s�ȴ��������
                       {
                         wifiInitMsg++;//
                       }
                       break;
		
                 case WIFI_INIT_CIPMUX:
                     
                     
			if (wifiCmdCIPMUX(3) != WIFI_NONE)//����ģʽ������������ģʽ��1����������ģʽ��0��
			{
                         wifiInitMsg++;
                         
                        }
                    
			break;
		case WIFI_INIT_CIFSR:                 
			if (wifiCmdCWJAP(gCfgPara.SSIDToJoin,gCfgPara.SSIDPwd,1) != WIFI_NONE)   
                       
			{
                         wifiInitMsg++;
                         sTimeout(&wifiSendTimer, 0);
                        }
			break;
		
        case WIFI_INIT_CIPCLOSE:                  //�ر�����ͨ��
                     if (sTimeout(&wifiSendTimer, 200))  //2s�ȴ��������
                     {
			//if (wifiCmdCIPCLOSE(2) != WIFI_NONE)//
			//{
                         wifiInitMsg++;
                       // }
                     }
			break;
        case WIFI_INIT_CIPSERVER:               
			//if (wifiCmdCIPSTART_UDP(UDPPort,1) != WIFI_NONE)//
			//{
                         wifiInitMsg++;
                         sTimeout(&wifiSendTimer, 0);
           // }
			break;
        case WIFI_INIT_CIPSTART: 
          if (sTimeout(&wifiSendTimer, 300))  //1s�ȴ��������
          {
			if (wifiCmdCIPSTART_UDP(UDPPort,1) != WIFI_NONE)//����UDPģʽ
			{
                         wifiInitMsg++;
                         sTimeout(&wifiSendTimer, 0);
            }
          }
			break;
        case WIFI_INIT_CIPSTART_RADIO: 
          if (sTimeout(&wifiSendTimer, 100))  //1s�ȴ��������
          {
			if (wifiCmdCIPSTART_UDPRADIO(1) != WIFI_NONE)// //����UDP�㲥ģʽ
			{
                          wifiInitMsg = WIFI_INIT_PRE;
                          ret =WIFI_OK;
            }
          }
			break;
		case WIFI_INIT_ERR:
			ret = WIFI_ERR;
			wifiInitMsg = WIFI_INIT_PRE;
			break;
		default:
			wifiInitMsg = WIFI_INIT_PRE;
			break;
	}
      }
      else if(wifiMDInitflag==1)   //����ģʽ��ʼ��
      {
        switch (wifiInitMsg)
	  {
		case WIFI_INIT_PRE:
			wifiInitMsg++;
			break;
                case WIFI_INIT_CWMODE:                  //����ģ��Ĺ���ģʽ
			if (wifiCmdCWMODE(3) != WIFI_NONE)//APģʽ
			{
                         wifiInitMsg++;
                        // sTimeout(&wifiSendTimer, 0);
                        }
			break;
                case WIFI_INIT_RST:
                    // if (sTimeout(&wifiSendTimer, 0))  //500ms�ȴ�
                    // {
			if (wifiCmdRST(1) != WIFI_NONE)//����ģ�飬��ȴ�5��
			{
                         wifiInitMsg++;
                         sTimeout(&wifiSendTimer, 0);
                        }
                    // }
			break; 
                case WIFI_INIT_WAIT:
                       if (sTimeout(&wifiSendTimer, 300))  //5s�ȴ��������
                       {
                         wifiInitMsg++;//
                         
                       }
                       break;
		case WIFI_INIT_CIPMUX:
                      
			if (wifiCmdCIPMUX(2) != WIFI_NONE)//����ģʽ������������ģʽ��1����������ģʽ��0��
			{
                         wifiInitMsg++;
                        // sTimeout(&wifiSendTimer, 0);
                        }
                    
			break;
        
		case WIFI_INIT_CIFSR: 
                    // if (sTimeout(&wifiSendTimer, 50))  //500ms�ȴ��������
                     //{
			if ((wifiCmdCWSAP_int(apssid,apssidPassword,1))!= WIFI_NONE)   //   
			{
                         wifiInitMsg++;
                         sTimeout(&wifiSendTimer, 0);
                        }
                    // }
			break;
		
        case WIFI_INIT_CIPCLOSE:                  //�ر�����ͨ��
                      if (sTimeout(&wifiSendTimer, 200))  //3s�ȴ�
                      {
			//if (wifiCmdCIPCLOSE(2) != WIFI_NONE)//
			//{
                         wifiInitMsg++;
                         //sTimeout(&wifiSendTimer, 0);
                        //}
                      }
			break;
        case WIFI_INIT_CIPSERVER:               //����ģ��Ϊ������
                    // if (sTimeout(&wifiSendTimer, 50))  //500ms�ȴ�
                    // {
			//if (wifiCmdCIPSERVER(3) != WIFI_NONE)//
			//{
                          wifiInitMsg++;
                          sTimeout(&wifiSendTimer, 0);
                       // }
                    // }
			break;
        case WIFI_INIT_CIPSTART:               //����UDPģʽ
                     if (sTimeout(&wifiSendTimer, 300))  //2s�ȴ�
                     {
			if (wifiCmdCIPSTART_UDP(UDPPort,1) != WIFI_NONE)//
			{
                         wifiInitMsg++;
                        }
                     }
			break;
        case WIFI_INIT_CIPSTART_RADIO:               //����UDP�㲥ģʽ
			//if (wifiCmdCIPSTART_UDPRADIO(2) != WIFI_NONE)//
			//{
                          wifiInitMsg = WIFI_INIT_PRE;
                          ret =WIFI_OK;
                       // }
			break;
		case WIFI_INIT_ERR:
			ret = WIFI_ERR;
			wifiInitMsg = WIFI_INIT_PRE;
			break;
		default:
			wifiInitMsg = WIFI_INIT_PRE;
			break;
	}
      }
	
	return ret;
}
int wifiCmdRST(int repeat)
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
		sprintf(buf, "AT+RST");  //����ģ�飬
        // sprintf(buf, "AT+RESTORE");  //�ָ��������ã�
	
	return (WifiCommand(buf, WIFI_NONE, 500, repeat));
}
int wifiCmdCWMODE(int repeat)
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
	   if(wifiMDInitflag==0) 
           {
              sprintf(buf, "AT+CWMODE=1");  //����ģ��Ĺ���ģʽ��STA��
           }
           else if(wifiMDInitflag==1) 
           {
              sprintf(buf, "AT+CWMODE=2");  //����ģ��Ĺ���ģʽ��AP��
           }
        }
	
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIPCLOSE(int repeat)
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
		sprintf(buf, "AT+CIPCLOSE=5");  //�ر�����ͨ��
	
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIFSR(int repeat)
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
		sprintf(buf, "AT+CIFSR");   ////��ȡģ��IP��ַ������APIP��STAIP�� APģʽ����Ч
	
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCWSAP(char* ip,char* port,int repeat)   //APģʽ�£�����ģ������ƺ������Լ�ͨ����
{
        uret=0;
        char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
          memset(buf,0,BSIZE_CMD);//AT+CWSAP="WIFI","hhzn2012",1,3
          strcpy(buf,"AT+CWSAP=");
          strcat(buf,"\"");
          strcat(buf,ip);
          strcat(buf,"\",\"");
          strcat(buf,port);
          strcat(buf,"\",5,0");   // �˴�0��ʾ��������
        }
		//sprintf(buf, "AT+CWSAP=""\""%s"\""",""\""%s"\"",1,3",ip,port);
	uret=WifiCommand(buf, WIFI_NONE, 200, repeat);
	return uret;
}
int wifiCmdCWSAP_int(char* ip,char* port,int repeat)   //APģʽ�£�����ģ������ƺ������Լ�ͨ����
{
          char buf[BSIZE_CMD];
          memset(buf,0,BSIZE_CMD);//AT+CWSAP="WIFI","hhzn2012",1,3
          strcpy(buf,"AT+CWSAP=");
          strcat(buf,"\"");
          strcat(buf,ip);
          strcat(buf,"\",\"");
          strcat(buf,port);
          strcat(buf,"\",5,0");   // �˴�0��ʾ��������
		//sprintf(buf, "AT+CWSAP=""\""%s"\""",""\""%s"\"",1,3",ip,port);
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCWJAP(char* ip,char* port,int repeat)  //����·����
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
          memset(buf,0,BSIZE_CMD);//AT+CWSAP="WIFI_SW","12345678",1	,3
          strcpy(buf,"AT+CWJAP=");
          strcat(buf,"\"");
          strcat(buf,ip);
          strcat(buf,"\",\"");
          strcat(buf,port);
          strcat(buf,"\"");
        }
		//sprintf(buf, "AT+CWSAP=""\""%s"\""",""\""%s"\"",ip,port);
	
	return (WifiCommand(buf, WIFI_NONE, 500, repeat));
}
int wifiCmdCWJAP_int(char* ip,char* port,int repeat)  //����·����
{
	  char buf[BSIZE_CMD];
          memset(buf,0,BSIZE_CMD);//AT+CWSAP="WIFI_SW","12345678",1	,3
          strcpy(buf,"AT+CWJAP=");
          strcat(buf,"\"");
          strcat(buf,ip);
          strcat(buf,"\",\"");
          strcat(buf,port);
          strcat(buf,"\"");

	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIPSTART(char* ip,char* port,int repeat) //����Զ���Ʒ�����ʱ��
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
          memset(buf,0,BSIZE_CMD);//AT+CIPSTART=id,"type","addr",port
          strcpy(buf,"AT+CIPSTART=4,\"TCP\",");
          strcat(buf,"\"");
          strcat(buf,ip);
          strcat(buf,"\",");
          strcat(buf,port);
        }
	return (WifiCommand(buf, WIFI_NONE, 20, repeat));
}
int wifiCmdCIPSTART_UDP(char* port,int repeat) //����UDP
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
          memset(buf,0,BSIZE_CMD);//AT+CIPSTART=2,"UDP","255.255.255.255",40001,40001,2
          strcpy(buf,"AT+CIPSTART=2,\"UDP\",");
          strcat(buf,"\"");
          strcat(buf,"255.255.255.255");
          strcat(buf,"\",");
          strcat(buf,port);
          strcat(buf,"\,");
          strcat(buf,port);
          strcat(buf,"\,");
          strcat(buf,"2");
        }
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIPSTART_UDPRADIO(int repeat) //����UDP�㲥
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
          memset(buf,0,BSIZE_CMD);//AT+CIPSTART=1,"UDP","255.255.255.255",40003,40003
          strcpy(buf,"AT+CIPSTART=1,\"UDP\",");
          strcat(buf,"\"");
          strcat(buf,"255.255.255.255");
          strcat(buf,"\",");
          strcat(buf,"40003");
          strcat(buf,"\,");
          strcat(buf,"40003");
        }
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIPMUX(int repeat)
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
        {
	  // if(wifiMDInitflag==0) 
         //  {
              sprintf(buf, "AT+CIPMUX=1");//����ģʽ������������ģʽ��1����������ģʽ��0��
         //  }
         //  else if(wifiMDInitflag==1) 
         //  {
         //     sprintf(buf, "AT+CIPMUX=0");//����ģʽ������������ģʽ��1����������ģʽ��0��
         //  }
        }
		
	
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIPSERVER(int repeat)
{
	char buf[BSIZE_CMD];
	if (wifiCmdMsg == CMD_SEND)
		sprintf(buf, "AT+CIPSERVER=1,8080");//1 ���� server ģʽ��0 �ر� server ģʽ
        //sprintf(buf, "AT+CIPSERVER=1,8080");
	return (WifiCommand(buf, WIFI_NONE, 200, repeat));
}
int wifiCmdCIPSEND(int len, u08 clientIDS,int repeat)//��������
{
	char buf[BSIZE_CMD];
        //ClientID=0;//TEST  ONE CLIENT
       // wifiCmdMsg=CMD_SEND;//test  zzw 2015/5/14
	if (wifiCmdMsg == CMD_SEND)
	 {
	 	sprintf(buf, "AT+CIPSEND=%u,%u", clientIDS,len);
		waitsmssend=0;
                //smssenddata=0;
               // uartPrintln(COM_WIFI, buf);//2?o???3��??DD
	 }
       // return 1;
	return (WifiCommand(buf, WIFI_NONE, 1000, repeat));
}

/*******************************************************************************

********************************************************************************/          						

//******************************************************************************
//������  ��write_flash_array
//��������������������Ƭ��EEPROM
//����    ��begin_addr      ������EEPROM�е�λ��
//          array      
//���    ����
//����ֵ  ����
//******************************************************************************
//By HeYC 0906	u08 write_flash_array(u32 begin_addr,u16 counter,u08 array[])
u08 write_flash_array(u32 begin_addr,u16 counter,u08* array)//By HeYC 0906
{
    u32 save_addr,save_addr_temp;//

    if(counter==0)
      return 0;		

    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    save_addr=begin_addr;
    save_addr_temp=begin_addr;
    while (save_addr < counter+save_addr_temp)
    {

      FLASH_ProgramByte(save_addr, *array++);
      save_addr = save_addr + 1;

    }
   FLASH_Lock(FLASH_MEMTYPE_DATA);
   return 1;
}

//******************************************************************************
//������  ��Read_Flash
//������������EEPROM��ȡ��ʱʱ��
//����    ��begin_addr      ����EEPROM�е�λ��
//���    ��pReadBuf     ָ��
//����ֵ  ��
//******************************************************************************
void Read_Flash(u32 begin_addr,u08 *pReadBuf,u16 Count)
{
	//u16  i,cnt;
	//u16  data;
	u32 Address_ready,read_addr;
			
	if(Count==0)
	    return;		
	
	read_addr=begin_addr;
	Address_ready=begin_addr;
	while (Address_ready < read_addr+Count)//
        {
         *pReadBuf++ = FLASH_ReadByte(Address_ready); 
         Address_ready = Address_ready + 1;
        }  
  
}
