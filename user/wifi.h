/*******************************************************************************
*
*   Application 
*   Soft version:   V1.00
*   File Name:      gsm.h
*   Author   :      zzw
*   creation date:  2012-02-16
*	module description: 
*   Copyright (C) reserve
*
********************************************************************************/
#if !defined(WIFI_H)
#define WIFI_H
#include "GLOBAL.h"
#include "task.h"




//#define BSIZE						2048
#define   WIFI_ERR				  2
#define 	WIFI_NONE			  0
#define 	WIFI_OK				  1


#define         IR_ERR				  2
#define 	IR_NONE			  0
#define 	IR_OK				  1

#define  WIFI_AT_BUSY     1  //模块忙着处理AT指令
#define  WIFI_DATA_BUSY   2  //模块忙着发数据
#define  WIFI_AP_BUSY     3  //模块忙着处理AT指令
#define  WIFI_STA_BUSY     4  //模块忙着处理AT指令
#define  WIFI_IDEL        0  //模块空闲

enum GSM_COMMAND   //枚举类型

{
	CMD_PRE = -1,
	CMD_SEND,
	CMD_WAITACK,
	CMD_SLEEP,
	CMD_REPSLEEP
};
enum GSM_SMSSD     //枚举类型
{
	SMS_SD_PRE = -1,
	SMS_SD_CMGS,
        SMS_SD_WAIT_CMGS,
	SMS_SD_SEND,
        SMS_SD_SEND_REV,
	SMS_SD_WAIT,
	SMS_SD_FINISH,
	SMS_SD_ERR
};

enum WIFI_INIT    //枚举类型
{
	WIFI_INIT_PRE = -1,
        WIFI_INIT_CWMODE, 
        WIFI_INIT_RST,
        WIFI_INIT_WAIT,
	//WIFI_INIT_CWMODE,  
	//WIFI_INIT_CIFSR,
	WIFI_INIT_CIPMUX,
        WIFI_INIT_CIFSR,
     WIFI_INIT_CIPCLOSE,
	WIFI_INIT_CIPSERVER,
    WIFI_INIT_CIPSTART,
    WIFI_INIT_CIPSTART_RADIO,
	WIFI_INIT_ERR
};
enum WIFI_AP_COMMAND
{
	WIFI_AP_PRE = 0,
	WIFI_AP_SET,
        WIFI_AP_SET_WAIT,
	WIFI_AP_RST,
        WIFI_AP_WAIT,
        WIFI_AP_ERROR
};
enum WIFI_STATE_COMMAND
{
	WIFI_AT_CMD_NOT_BGN = 0,
	WIFI_AT_CMD_GOING,
        WIFI_AT_CMD_OVER,
//By HeYC 0907 not used        WIFI_STATE_STA_FINISH,
};
enum WIFI_STA_COMMAND
{
	WIFI_STA_PRE = 0,
        WIFI_STA_INIT_CIFSR,
        WIFI_STA_CONNET_CIPSERVER,
        WIFI_STA_ERROR
};
enum restartwifi    //枚举类型
{
        WIFI_RESTART_PRE = 0,
	WIFI_RESTART_POWERDOWM,
	WIFI_RESTART_WAIT,
	WIFI_RESTART_POWERUP,
	WIFI_RESTART_WAITRUN,
	WIFI_RESTART_END
};
enum IR_SEND     //枚举类型
{
	IR_SEND_PRE = 0,
	IR_SEND_INIT,
        IR_SEND_MODE,
	IR_SEND_TEMP,
        IR_SEND_WIND_SPEED,
        IR_SEND_WIND_DIR,
	IR_SEND_FINISH,
        IR_SEND_ON,
	IR_SEND_ERR
};
#define  sBUFFER_SIZE  128
extern u8  flashbuf[sBUFFER_SIZE];
extern char _cmdIsSend;
extern char aircondition_answer[10];
extern char WirelearnAndControl_answer[10];
extern char BasicCommand_answer[10];
extern char APPconfigure_answer[10];
extern char Heartbeat[13];
extern char RCR_ETX[3];
extern char GetCurSetOrUnSet_answer[10];
extern char GetKZCurSetOrUnSet_answer[10];
extern char ProbeAlarm[10];
extern char KZProbeAlarm[10];
extern char KZaircondition_answer[10];
extern char AirTypeTest_answer[10];

u08 wifiReadResponse(void);
char CmdIsSend(void);
extern char sbuffer[sBUFFER_SIZE];
int WifiCommand(char* cmd, int answer, int timeout, int repeat);
int WifiCommandHex(char* cmd, int answer, int timeout, int repeat,int len);
void  SMSSEND(u16 len,char*  smscontent,u08 clientID);
void decodestring (char *str);
void replace(char originalString[], char key[], char swap[]);
u8 Auto_WIFI_Send(void);
int wifiMDInit(void);
int wifiCmdRST(int repeat);
int wifiCmdCWMODE(int repeat);
int wifiCmdCWMODE_AP(int repeat);
int wifiCmdCIFSR(int repeat);
int wifiCmdCWSAP(char* ip,char* port,int repeat);
int wifiCmdCWSAP_int(char* ip,char* port,int repeat);
int wifiCmdCIPSTART(char* ip,char* port,int repeat);
int wifiCmdCIPSTART_UDP(char* port,int repeat);
int wifiCmdCIPSTART_UDPRADIO(int repeat);
int wifiCmdCIPMUX(int repeat);
int wifiCmdCIPSERVER(int repeat);
int wifiCmdCIPSEND(int len, u08 clientIDS,int repeat);//发送数据
int wifiCmdCIPCLOSE(int repeat);


//By HeYC 0906	u08 write_flash_array(u32 begin_addr,u16 counter,u08 array[]);
u08 write_flash_array(u32 begin_addr,u16 counter,u08* array);//By HeYC 0906
void Read_Flash(u32 begin_addr,u08 *pReadBuf,u16 Count);
u08 IRcontrol(void);
int wifiCmdCWJAP(char* ip,char* port,int repeat);
int wifiCmdCWJAP_int(char* ip,char* port,int repeat);
int wifiCmdCWSAP(char* ip,char* port,int repeat);
extern char Heartbeat[13];
extern u16 lenth;
extern u32 wifiSendTimer;
#endif // 
/* EOF */
