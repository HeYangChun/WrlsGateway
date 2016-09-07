#include "stm8s_lib.h"
#include "cmdFunctions.h"
#include "RS485.h"
#include "protocol.h"

#define  PROTOCOL_RX_SD0        0xED  // 接收命令时的起始位0
#define  PROTOCOL_RX_SD1        0x8C  // 接收命令时的起始位1
#define  PROTOCOL_RX_ED         0x8D  // 接收命令时的结束位

#define  PROTOCOL_TX_SD0        0x8C  // 发送命令时的起始位0
#define  PROTOCOL_TX_SD1        0xED  // 发送命令时的起始位1
#define  PROTOCOL_TX_ED         0x8D  // 发送命令时的结束位

#define  PROTOCOL_ADD_ID        2     // 命令中地址所在的位置
#define  PROTOCOL_LEN_ID        3     // 命令中数据长度LEN所在的位置
#define  PROTOCOL_CMD_ID        4     // 命令中数据所在的位置

#define  RX_STATE_SD0           0     // 接收起始位0状态
#define  RX_STATE_SD1           1     // 接收起始位1状态
#define  RX_STATE_ADDR          2     // 接收控制板地址状态
#define  RX_STATE_LEN           3     // 接收数据长度状态
#define  RX_STATE_DATA          4     // 接收数据状态
#define  RX_STATE_CHKSUM        5     // 接收校验和状态
#define  RX_STATE_ED            6     // 接收结束位状态

tDelayType rs485TxDly = DELAY_DEFAULT;

static u8 DeviceID = 0;   // 控制板地址

static u8 RxID;           // 命令中的控制板地址

static u8 RxState = RX_STATE_SD0; // 接收状态

static u8 RxBuf[PROTOCOL_RX_BUF_SIZE];  // 接收缓存
static u8 RxBufCnt = 0;                 // 接收缓存中数据的长度
static u8 RxRdIx = 0;                   // 读取接收缓存的索引
static u8 RxChkSum = 0;                 // 计算出的校验和
static u8 RxRemainLen = 0;              // 为接收数据的长度

static u8 TxBuf[PROTOCOL_TX_BUF_SIZE];  // 发送缓存
static u8 TxLen = 0;                    // 发送缓存中数据的长度
//static u8 TxRdIx = 0;                   // 读取发送缓存的索引
static u8 TxChkSum = 0;                 // 发送命令的校验和

static CmdFun cmdFun[MAX_CMD_NUM+1];    // 命令功能函数的指针数组

static void RxPkt(void);
static void RxBufWrite(u8 rx_data);
static u8 RxBufRead(void);
static void RxBufClr(void);
static void TxBufClr(void);

//******************************************************************************
//函数名  ：PROTOCOL_Init
//功能描述：协议初始化，主要是初始化命令的功能函数指针
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void PROTOCOL_Init(void)
{
  // 命令功能函数指针为0时，表示无该功能函数
  cmdFun[0] = 0;
  cmdFun[1] = CMD_ConnectTest;
  cmdFun[2] = CMD_OpenDoor;
  cmdFun[3] = CMD_CheckDoorSta;
  cmdFun[4] = CMD_SetLockDly;
  cmdFun[5] = CMD_GetLockDly;
  cmdFun[6] = CMD_LightSet;
  cmdFun[7] = CMD_CheckLightSta;
  cmdFun[8] = CMD_SetLightDly;
  cmdFun[9] = CMD_GetLightDly;
  cmdFun[10] = CMD_CheckObject;
  cmdFun[11] = CMD_CheckAllDoorSta;
  cmdFun[12] = CMD_CheckAllLightSta;
  cmdFun[13] = CMD_CheckAllObjects;
  cmdFun[14] = CMD_OpenDoors;
  cmdFun[15] = CMD_GetCoinCounter;
  cmdFun[16] = CMD_ClearCoinCounter;
  cmdFun[17] = CMD_SetBoxRemap;
  cmdFun[18] = CMD_GetBoxRemapTable;
  cmdFun[19] = CMD_ResetBoxRemapTable;
  
  RxBufClr();
  TxBufClr();
}

//******************************************************************************
//函数名  ：PROTOCOL_RegisterCmdFun
//功能描述：注册命令的功能函数
//输入    ：cmdNum    命令号，范围为0~MAX_CMD_NUM
//          fun       命令功能函数的指针
//输出    ：无
//返回值  ：无
//******************************************************************************
void PROTOCOL_RegisterCmdFun(u8 cmdNum, CmdFun fun)
{
  cmdFun[cmdNum] = fun;
}

//******************************************************************************
//函数名  ：PROTOCOL_UnregisterCmdFun
//功能描述：注销命令的功能函数
//输入    ：cmdNum    命令号，范围为0~MAX_CMD_NUM
//输出    ：无
//返回值  ：无
//******************************************************************************
void PROTOCOL_UnregisterCmdFun(u8 cmdNum)
{
  cmdFun[cmdNum] = 0;
}

//******************************************************************************
//函数名  ：PROTOCOL_SetDeviceID
//功能描述：设置协议中控制板地址
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void PROTOCOL_SetDeviceID(u8 id)
{
  DeviceID = id;
}

//******************************************************************************
//函数名  ：PROTOCOL_RxHandler
//功能描述：接收到数据时调用该函数
//输入    ：rx_data    接收到的数据
//输出    ：无
//返回值  ：无
//******************************************************************************
void PROTOCOL_RxHandler(u8 rx_data)
{
  switch(RxState)
  {
  case RX_STATE_SD0:  // 等待起始位0
    if(rx_data == PROTOCOL_RX_SD0)
    {
      RxState = RX_STATE_SD1;
      RxBufClr();     // 清除接收缓存
    }
    break;
  case RX_STATE_SD1:  // 等待起始位1
    if(rx_data == PROTOCOL_RX_SD1)
    {RxState = RX_STATE_ADDR;}
    else
    {RxState = RX_STATE_SD0;}
    break;
  case RX_STATE_ADDR: // 等待器件地址
    RxID = rx_data;
    RxChkSum = rx_data;
    RxState = RX_STATE_LEN;
    break;
  case RX_STATE_LEN:  // 等待数据长度
    if((rx_data == 0x00) || (rx_data > PROTOCOL_RX_BUF_SIZE))
    {RxState = RX_STATE_SD0;} // 数据长度为0或超过接收缓存大小，则从新开始等待命令
    else
    {
      RxChkSum += rx_data;
      RxRemainLen = rx_data;  // 还需接收的数据长度
      RxState = RX_STATE_DATA;
    }
    break;
  case RX_STATE_DATA:
    RxBufWrite(rx_data);
    RxChkSum += rx_data;
    if(--RxRemainLen == 0)
    {RxState = RX_STATE_CHKSUM;}
    break;
  case RX_STATE_CHKSUM:       // 等待校验和
    RxChkSum += rx_data;
    if((RxChkSum & 0xFF) == 0x00)
    {RxState = RX_STATE_ED;}
    else
    {RxState = RX_STATE_SD0;}
    break;
  case RX_STATE_ED:           // 等待结束位
    if((rx_data == PROTOCOL_RX_ED) && (RxID == DeviceID))
    {RxPkt();}  // 收到正确命令且器件地址相同，则执行命令
    RxState = RX_STATE_SD0;
    break;
  default:
    RxState = RX_STATE_SD0;
    break;
  }
}

//******************************************************************************
//函数名  ：RxPkt
//功能描述：解包命令，执行命令功能函数
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
static void RxPkt(void)
{
  u8 i,cmd,len;
  
  TxLen = 0;
  
  cmd = RxBufRead();  // 读取命令
  if((cmd <= MAX_CMD_NUM) && (cmdFun[cmd] != 0))
  { // 命令有效且存在对应的功能函数，则执行命令
    TxBuf[TxLen++] = PROTOCOL_TX_SD0;
    TxBuf[TxLen++] = PROTOCOL_TX_SD1;
    TxBuf[TxLen++] = DeviceID;
    TxLen++;  // 为LEN预留，存放数据的长度
    TxBuf[TxLen++] = cmd;
    // 执行功能函数
    len = cmdFun[cmd](&RxBuf[RxRdIx], RxBufCnt-1, &TxBuf[TxLen]);
    TxLen += len;
    TxBuf[PROTOCOL_LEN_ID] = len + 1;
    // 计算校验和
    for(i=PROTOCOL_ADD_ID,TxChkSum=0; i<TxLen; i++)
    {TxChkSum += TxBuf[i];}
    TxBuf[TxLen++] = TxChkSum;
    TxBuf[TxLen++] = PROTOCOL_TX_ED;
    Delay10MS(&rs485TxDly, 1, TX_DLY_TIME);
    //RS485_Write(TxBuf, TxLen); // 开始发送数据
  }
}

void PROTOCOL_SendResp(void)
{
  RS485_Write(TxBuf, TxLen); // 开始发送数据
}

static void RxBufWrite(u8 rx_data)
{
  if(RxBufCnt < PROTOCOL_RX_BUF_SIZE)
  {RxBuf[RxBufCnt++] = rx_data;}
}

static u8 RxBufRead(void)
{
  return (RxBuf[RxRdIx++]);
}

static void RxBufClr(void)
{
  RxBufCnt = 0;
  RxRdIx = 0;
}

static void TxBufClr(void)
{
  TxLen = 0;
//  TxRdIx = 0;
}

