/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      GLOBAL.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:声明DTS AS系统中的全局变量 
*   Copyright (C) 
*
********************************************************************************/
#if !defined(_GLOBAL_H_)
#define _GLOBAL_H_
#include "stm8s_lib.h"
// ************************* Typedef data type  *********************************
typedef unsigned 	char  	u08;
//typedef unsigned 	char  	u8;
typedef          	char  	s08;
//typedef unsigned 	short 	u16;
//typedef          	short 	s16;
//typedef unsigned 	long 	u32;
//typedef          	long 	s32;
typedef unsigned 	char  	uchar;
typedef unsigned 	int   	uint;
typedef unsigned long long  u64;
typedef   signed long long  s64;

typedef unsigned char   	BOOL;
typedef unsigned char		BYTE;
typedef unsigned int		WORD;
typedef unsigned long		DWORD;

typedef unsigned char		byte;
typedef unsigned int		word;
typedef unsigned long		dword;

typedef unsigned char		UCHAR;
typedef unsigned int		UINT;
typedef unsigned short  	USHORT;
typedef unsigned long		ULONG;

#ifndef NULL
#define NULL 0
#endif

//stm8  eeprom

//By HeYC 0906	#define SW_SET_ADDR               ((u32)(FLASH_DATA_START_PHYSICAL_ADDRESS + 0))//插座开始设置地址0x004000
//By HeYC 0906	#define ENBALE_PROTECTION_ADDR    ((u32)(SW_SET_ADDR + 1))//充电保护使能0x004001
//By HeYC 0906	#define ENBALE_KEY_ADDR           ((u32)(ENBALE_PROTECTION_ADDR + 1))//本地按键使能0x004002
#define SW_ID_ADDR                ((u32)(FLASH_DATA_START_PHYSICAL_ADDRESS + 0))//本SW的MAC地址，12字节0x004003
//By HeYC 0906	#define SW_PIN_ADDR               ((u32)(SW_ID_ADDR + 12))//PIN码，3字节		0x00400F
//By HeYC 0906	#define SW_CLOCK_ADDR             ((u32)(SW_PIN_ADDR + 3))//128bytes len+10*(#1#120110#1)		0x004012
//By HeYC 0906	#define SW_DELAY_TIME_ADDR        ((u32)(SW_CLOCK_ADDR + 128))//2字节，单位秒	0x004092
//By HeYC 0906	#define SW_SERVICE_IP_ADDR          ((u32)(SW_DELAY_TIME_ADDR + 2))//最大15字节，210.209.73.47		0x004094
//By HeYC 0906	#define SW_SERVICE_PORT_ADDR        ((u32)(SW_SERVICE_IP_ADDR + 20))//7887								0x0040A8
//By HeYC 0906	#define SW_CURRENT_P_ADDR           ((u32)(SW_SERVICE_PORT_ADDR + 20))//电量总量					0x0040BC
//By HeYC 0906	#define SW_CURRENT_P_DAY_ADDR       ((u32)(SW_CURRENT_P_ADDR + 4))//月电量31天，每天电量		0x0040C0
//By HeYC 0906	#define SW_CURRENT_DAY_ADDR         ((u32)(SW_CURRENT_P_DAY_ADDR + 32))//已运行天数，最大31天，0除外	0x0040E0
//By HeYC 0906	#define SW_NULL_ADDR                ((u32)(SW_CURRENT_P_DAY_ADDR +1))//							0x0040E1
//By HeYC 0906	#define SW_UDP_PORT_ADDR            ((u32)(SW_NULL_ADDR +1))//									0x0040E2
#define SW_SETorUNSETSTATE          ((u32)(SW_ID_ADDR +sizeof(CfgParaInFlash)))//							0x0040E3
//By HeYC 0906	#define SW_SSID_ADDR             ((u32)(SW_SETorUNSETSTATE +20))//								0x0040F7
//By HeYC 0906	#define SW_SSIDPASSWORD_ADDR             ((u32)(SW_SSID_ADDR +20))//							0x00410B

//stm8  eeprom


//at24c512
#define OffsetAddr                        128
#define INFRARED_PRE_ADDR                ((u16)(0))//
#define RF315_PRE_ADDR                   ((u16)(200*OffsetAddr))//
#define RF433_PRE_ADDR                   ((u16)(280*OffsetAddr))//
#define RF315_PRE_ADDR_SPL               ((u16)(360*OffsetAddr))//
#define RF433_PRE_ADDR_SPL               ((u16)(400*OffsetAddr))//


//RTC时间
typedef struct __RFS
{
  u08 type;
  u08 number;
  u08 state;
  u08 sendflag;
  u08 replyCounter;//RF重发次数 
  int PulseN;
  int PulseLEN;
//By HeYC 0831  int sleep;
  int SENDSTEP;
//By HeYC 0831	  int SendFinish;

}RFS;//RTC时间结构体
extern RFS rfs;

//at24c512

//extern char OFTS_AS_VERSION[4];	//版本号

//RTC时间
/*typedef struct __RTCTIME
{
   WORD  year;//年
   byte month;//月
   byte date;//日
   byte day;//星期
   byte hour;//时
   byte min;//分
   byte sec;//秒
   

}RTCTIME;//RTC时间结构体*/

//CLOCK时间
/*typedef struct __CLOCKTIME
{
   byte hour;
   byte min;
   byte sec;
   byte state;
   byte num;
   
}CLOCKTIME;*/

typedef  struct   flash_
{
  u16  UDL;     //没用
  u16  len;
  u08  buf[100];
  u08  clientID;   //例如用手机连模块，那么这个ID就指的是手机的ID,1号手机，2号手机。。。。
} flash;
///
typedef struct __AIRCON
{
   byte func;
   byte codeValH;
   byte codeValL;
   byte TimeMin;
   byte onoff;
   byte mode;
   byte temp;
   byte windspeed;
   byte winddir;
   byte enable;//控制标志位，置1时表示需要控制
   byte step;//任务状态机步骤
   byte ack;//状态返回
   byte codeask;//发送码值成功标志位
   byte cmdsetp;
   
}AIRCON;
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// UART消息及外设ID定义
//
#define COM1		0 // 
#define COM3		2 // 

#define COM_WIFI	COM1 
#define COM_IRBALL	COM3 




#define UART0_RX_BUFFER_SIZE		400	//COM1
#define UART3_RX_BUFFER_SIZE		1	//COM1
#define COMM1_BUF_LEN	10
#define TIMING_MONITOR_WIFI		3600*2	// 

/////////////////////////////////////////////////////////
typedef  byte (*AppCommRecvPtr)(const byte *buf, byte len, byte Addr);
typedef struct APP
{
	byte Port;	// 网络端口号
	AppCommRecvPtr fcom; //应用层函数
} app_com;


/*typedef struct current_
{
	WORD Iave;
        WORD I;	// 
        DWORD P;
        byte DAYP;
        byte day;
        byte Pbuf[40];
}current;*/
//////////////////////////////////////////////////////////////


//extern byte COMM1RecvBuf[COMM1_BUF_LEN];
extern app_com app[5];//创建端口函数数组


/************************ 位变量定义 ***********************/
typedef union {
  byte Byte;
  struct {
    byte bit0        :1;                                       
    byte bit1        :1;                                       
    byte bit2        :1;                                      
    byte bit3        :1;                                       
    byte bit4        :1;                                       
    byte bit5        :1; 
    byte bit6        :1; 
    byte bit7        :1;                                       
  } Bits;
} DBit;

////////////////RF//////////////////////////////////////////////////////////
#define  IR_END        800 //500mS  结束判断
#define  IR_START      1000 //5mS, 起始判断
#define  IR_LEARN_TIMEOUT 1000000 //10S退出学习
#define  IR_MAX_LEN    243//学习最长的字节数
#define  IRCODELENGTH		   10

#define  ALARM_TIMEOUT        5000 //5秒超时退出
enum RF_IR_COMMAND
{
	IR_PRE = 0,
	IR_GUIDE,
	IR_DECODE,
	IR_STOP
};

enum RF_IR_SEND
{
	IRS_PRE = 0,
	IRS_H,
	IRS_L,
	IRS_STOP
};

//Bgn:By HeYC 0830
typedef enum _IDLE_RF_RCV_STAT
{
	IRRS_IDLE=0,
	IRRS_315_GUIDE_RCVD,
	IRRS_433_GUIDE_RCVD,
	IRRS_315_RCV_SUCC,
	IRRS_433_RCV_SUCC
} IDLE_RF_RCV_STAT;

#define RF_TYPE_NULL	0
#define RF_TYPE_315		1
#define RF_TYPE_433		2
#define RF_TYPE_INFRA	3
//End:By HeYC 0830
typedef struct _IR
{
 u16 Rcounter; //接收计数器
 u16 Scounter; //发送计数器
 u08 REVFLAG;  //接收标志位
//By HeYC 0831	 u08 SEDFLAG;  //发送标志位
//By HeYC 0831 u08 COPYFLAG; //开始解码拷贝
 u08 DECODESTEP; //解码步骤
 u08 SENDSTEP;   //发送步奏
 u08 Learnflag;//对码标志，为1表示对码成功
//By HeYC 0831	 u08 SendFinish; //发送完成标志
//By HeYC 0831 u08 sleep;
 u08 guide;//引导码标志
 //u08 num;    
 u08 NO;         //当前指令号
 u32 timeout;  //计数时间到
 u32 PulseN;   //脉冲数，变量
 u32 PulseLEN; //存储脉冲长度，存在flash中
//By HeYC 0831 u32 ADDR_QUEUE[2];//地址队列，序列首脉冲
 u16 cmd[256];     //脉冲序列存储
 u32 rfRcvTimer;//By HeYC from Tout to rfRcvTimer.
}IR_;
extern IR_  IR;				    //RF指令

typedef struct _ALARM_RF
{
 u16 Rcounter; //接收计数器
 //by HeYC 0831	u16 Scounter; //发送计数器
 //u08 REVFLAG;  //接收标志位
//By HeYC 0831	 u08 SEDFLAG;  //发送标志位
//By HeYC 0831	 u08 COPYFLAG; //开始解码拷贝
 u08 DECODESTEP; //解码步骤
//By HeYC 0901	 u08 SENDSTEP;   //发送步奏
//By HeYC 0901	 u08 Learnflag;//对码标志，为1表示对码成功
 //By HeYC 0831	u08 SendFinish; //发送完成标志
//By HeYC 0831 u08 sleep;
 u08 guide;//引导码标志
 //u08 num;    
 u08 NO;         //当前指令号
//By HeYC 0831 u32 timeout;  //计数时间到
 u32 PulseN;   //脉冲数，变量
//By HeYC 0831 u32 PulseLEN; //存储脉冲长度，存在flash中
 u32 Tout;
 u32 timeCounter;//超时计数器
 u08 sending;//报警信号已发送
}ALARM_RF;


//Bgn:By HeYC 0906
typedef struct _CfgParaInFlash
{
	char magicStr[5];		//"topan"
	u08  reserved1;		// reserved for other used
	u08 macAddr[12];
	char serverAddr[20];	//ex:"topansh.wicp.net"
	char serverPort[10];	//"7269"
	char SSIDToJoin[20];
	char SSIDPwd[20];
}CfgParaInFlash;
//End:By HeYC 0906
extern ALARM_RF  alarm;				    //RF报警指令

/////////////////////////////RF//////////////////////////////////////////////

extern volatile              DBit _BitParament0;
#define sys10msFlag 		_BitParament0.Bits.bit0 	/* 10ms标志 			*/
#define sys1msFlag	 	    	_BitParament0.Bits.bit1 	/* CAN数据接收标志 	*/
#define KEYflag		        _BitParament0.Bits.bit2 	/* 按键控制标志   */
#define beepout		        _BitParament0.Bits.bit3 	/* 0输出低，1输出高 */
#define KEYControlState		_BitParament0.Bits.bit4 	/* 控制状态，0：空闲，1：窗帘正在动作  */
#define Nightflag     		_BitParament0.Bits.bit5 	/* 夜晚标志，夜晚降低背景灯 */		  
#define LONGKEYFALG         _BitParament0.Bits.bit6 	/* 长按键标志 */
#define BEEPflag            _BitParament0.Bits.bit7 	/* 0关，1开 */


extern volatile word   	    tickcount;      /* Timer tick count   		*/
extern volatile byte 	    sysTickfor1ms;	/* 10millisecond timer 		*/
extern volatile byte 	    sysTickfor1us;	/* 10millisecond timer 		*/
extern volatile byte            Markfor1us;
extern volatile word 		sysTickfor10ms;	/* 10millisecond timer 		*/
extern volatile byte 		sysTickfor100ms;/* 100millisecond timer 	*/
extern volatile byte 		sysTickfor500ms;   /* 500millisecond timer 	*/
extern volatile byte 		sysTickforRtc1s;   /* 1second timer 	*/
extern volatile byte 		  sysTickfor8s;   /* 8second timer 	*/
extern volatile byte 		  sysTickfor5s;   /* 5second timer 	*/
extern volatile byte 		  sysTickfor6s;   /* 6second timer 	*/
extern volatile u16		uartwifiTimer;	//串口定时器，定时清空缓冲区
extern volatile word         Heartbeattimer5s;
extern volatile byte         Heartbeattimer5s_count;
extern volatile byte         Heartbeattimer5sflag;
extern volatile byte           flagGotAppCfgInfo;

#define WIFILENGTH  2
extern flash  WIFISBUF[WIFILENGTH];	////wifi缓冲区
extern flash  WIFITxMessage;	//WiFi发送数据
extern u08    WIFI_Head;//WiFi环形缓冲区头
extern u08    WIFI_Hail;//WiFi环形缓冲区尾
//by HeYC 0906	extern u08    WIFIENBLE;     // //wifi使能
extern u08    WaitMONITOR_IDELSend; ////等待wifi发送数据完成标志位
extern int smsSMSStep;//WiFi信息发送步奏
//By HeYC 0906	extern char macid[20];//网卡mac地址
extern char macidHEX[6];//网卡mac地址
//by heyc 0906	extern char   revmacid[20];//收到的mac序列号
//BY HeYC 0906	extern char   staClientIP[20];//STA模式远程服务器IP
//By HeYC 0906	extern char   staClientPort[10];//STA式远程服务器端口号
extern char   UDPPort[10];//正常工作时的UDP端口
//by heyc 0906	extern char   revmacid[20];
//By HeYC 0906	extern WORD macpin;//mac密码
//by heyc 0906	extern DWORD  macidlong;//mac转长整形
extern u08  waitsmssend;  //收到等待'>'启动标志位，表示可以发送数据到WIFI
//by heyc 0906	extern u08  smssenddata;
//by heyc 0906	extern u08 keystate;//按键状态
extern u08  WifiBusy; //模块忙，不能回复数据

///rtc
//extern  RTCTIME tm;
//extern  CLOCKTIME clcoks[10];

//extern u08 ENBALE_SET;//取消
//extern u08 ENBALE_PROTECTION;//取消
//By HeYC 0906	extern u08 ENBALE_KEY;//取消
//extern char clock[128];
extern u08 eeprombuf[270];
extern u08 splflage[2];
//extern char IRString[50];//红外序列
//extern u8 IRDatas[25];//红外与数据
//By HeYC 0906	extern u16 delayOffTime;//取消

//by heyc 0906	extern char ssid[20];//路由器账号
//by heyc 0906	extern char ssidPassword[20];//路由器密码
extern char apssid[20];//模块的账号
extern char apssidPassword[20];//模块的密码
//extern current currents;//取消
//extern u08 REALYState;//取消
//By HeYC 0822	extern u08 setSSid;
//By HeYC 0822	extern u08 apsetSSid; 
extern u08 ClientID;

//#define REAL_ANALOG_NUM		1	// 实际的模拟通道数量
//extern word AnalogData[REAL_ANALOG_NUM]; //取消
//extern word delayoffCounter;//取消
extern u08 wifiEnable;//wifi使能
//extern u08  adStart;//取消
extern u08 STAEnable;//sta模式连接路由器成功
extern u08 staClientConnent;//1表示远程连接成功
extern u32 staClientCounter;//
extern u08 staInitMsg;
extern u08 staClientConnentStart;
extern u08 ATCWSAPEnable;
extern AIRCON  air;
extern u08 read_IR315_trig;        //
extern u08 read_IR433_trig;
extern u8 send_flag;
extern u32 TimeOut;
extern u08 CodeBuf[IRCODELENGTH];
extern u08 CodeHead,CodeTail;
extern u32 gAppCfgSuccTmout;
//设防撤防状态
//By HeYC 0906	extern u08 SetOrUnsetState[1];
extern u08	curSecuMode;//by heyc 0906
extern volatile byte curSecuModeShadow;
//By HeYC 	extern char rfInterLock;
extern IDLE_RF_RCV_STAT rfInterLock;//By HeYC
//By HeYC 0830	extern u08 alarmActive;//报警信号对码成功
extern u08 Authenticationflag;
extern u08 CNT;
extern u08 wifiMDInitflag;
extern u08 ClientCounterA;
extern u08 hartconnectflag;
extern byte HFlagWhereAuthSetZero;
extern volatile byte secModChangedFlag;//By HeYC
extern int wifiInitMsg;//By HeYC 
extern CfgParaInFlash   gCfgPara;//By HeYC 0906
/////////////////////////////////////////////////////////////////////

#endif // !defined(_GLOBAL_H_) 
		
/* EOF */
