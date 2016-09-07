#include "stm8s_lib.h"
#include "cmdFunctions.h"
#include "RS485.h"
#include "protocol.h"

#define  PROTOCOL_RX_SD0        0xED  // ��������ʱ����ʼλ0
#define  PROTOCOL_RX_SD1        0x8C  // ��������ʱ����ʼλ1
#define  PROTOCOL_RX_ED         0x8D  // ��������ʱ�Ľ���λ

#define  PROTOCOL_TX_SD0        0x8C  // ��������ʱ����ʼλ0
#define  PROTOCOL_TX_SD1        0xED  // ��������ʱ����ʼλ1
#define  PROTOCOL_TX_ED         0x8D  // ��������ʱ�Ľ���λ

#define  PROTOCOL_ADD_ID        2     // �����е�ַ���ڵ�λ��
#define  PROTOCOL_LEN_ID        3     // ���������ݳ���LEN���ڵ�λ��
#define  PROTOCOL_CMD_ID        4     // �������������ڵ�λ��

#define  RX_STATE_SD0           0     // ������ʼλ0״̬
#define  RX_STATE_SD1           1     // ������ʼλ1״̬
#define  RX_STATE_ADDR          2     // ���տ��ư��ַ״̬
#define  RX_STATE_LEN           3     // �������ݳ���״̬
#define  RX_STATE_DATA          4     // ��������״̬
#define  RX_STATE_CHKSUM        5     // ����У���״̬
#define  RX_STATE_ED            6     // ���ս���λ״̬

tDelayType rs485TxDly = DELAY_DEFAULT;

static u8 DeviceID = 0;   // ���ư��ַ

static u8 RxID;           // �����еĿ��ư��ַ

static u8 RxState = RX_STATE_SD0; // ����״̬

static u8 RxBuf[PROTOCOL_RX_BUF_SIZE];  // ���ջ���
static u8 RxBufCnt = 0;                 // ���ջ��������ݵĳ���
static u8 RxRdIx = 0;                   // ��ȡ���ջ��������
static u8 RxChkSum = 0;                 // �������У���
static u8 RxRemainLen = 0;              // Ϊ�������ݵĳ���

static u8 TxBuf[PROTOCOL_TX_BUF_SIZE];  // ���ͻ���
static u8 TxLen = 0;                    // ���ͻ��������ݵĳ���
//static u8 TxRdIx = 0;                   // ��ȡ���ͻ��������
static u8 TxChkSum = 0;                 // ���������У���

static CmdFun cmdFun[MAX_CMD_NUM+1];    // ����ܺ�����ָ������

static void RxPkt(void);
static void RxBufWrite(u8 rx_data);
static u8 RxBufRead(void);
static void RxBufClr(void);
static void TxBufClr(void);

//******************************************************************************
//������  ��PROTOCOL_Init
//����������Э���ʼ������Ҫ�ǳ�ʼ������Ĺ��ܺ���ָ��
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void PROTOCOL_Init(void)
{
  // ����ܺ���ָ��Ϊ0ʱ����ʾ�޸ù��ܺ���
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
//������  ��PROTOCOL_RegisterCmdFun
//����������ע������Ĺ��ܺ���
//����    ��cmdNum    ����ţ���ΧΪ0~MAX_CMD_NUM
//          fun       ����ܺ�����ָ��
//���    ����
//����ֵ  ����
//******************************************************************************
void PROTOCOL_RegisterCmdFun(u8 cmdNum, CmdFun fun)
{
  cmdFun[cmdNum] = fun;
}

//******************************************************************************
//������  ��PROTOCOL_UnregisterCmdFun
//����������ע������Ĺ��ܺ���
//����    ��cmdNum    ����ţ���ΧΪ0~MAX_CMD_NUM
//���    ����
//����ֵ  ����
//******************************************************************************
void PROTOCOL_UnregisterCmdFun(u8 cmdNum)
{
  cmdFun[cmdNum] = 0;
}

//******************************************************************************
//������  ��PROTOCOL_SetDeviceID
//��������������Э���п��ư��ַ
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void PROTOCOL_SetDeviceID(u8 id)
{
  DeviceID = id;
}

//******************************************************************************
//������  ��PROTOCOL_RxHandler
//�������������յ�����ʱ���øú���
//����    ��rx_data    ���յ�������
//���    ����
//����ֵ  ����
//******************************************************************************
void PROTOCOL_RxHandler(u8 rx_data)
{
  switch(RxState)
  {
  case RX_STATE_SD0:  // �ȴ���ʼλ0
    if(rx_data == PROTOCOL_RX_SD0)
    {
      RxState = RX_STATE_SD1;
      RxBufClr();     // ������ջ���
    }
    break;
  case RX_STATE_SD1:  // �ȴ���ʼλ1
    if(rx_data == PROTOCOL_RX_SD1)
    {RxState = RX_STATE_ADDR;}
    else
    {RxState = RX_STATE_SD0;}
    break;
  case RX_STATE_ADDR: // �ȴ�������ַ
    RxID = rx_data;
    RxChkSum = rx_data;
    RxState = RX_STATE_LEN;
    break;
  case RX_STATE_LEN:  // �ȴ����ݳ���
    if((rx_data == 0x00) || (rx_data > PROTOCOL_RX_BUF_SIZE))
    {RxState = RX_STATE_SD0;} // ���ݳ���Ϊ0�򳬹����ջ����С������¿�ʼ�ȴ�����
    else
    {
      RxChkSum += rx_data;
      RxRemainLen = rx_data;  // ������յ����ݳ���
      RxState = RX_STATE_DATA;
    }
    break;
  case RX_STATE_DATA:
    RxBufWrite(rx_data);
    RxChkSum += rx_data;
    if(--RxRemainLen == 0)
    {RxState = RX_STATE_CHKSUM;}
    break;
  case RX_STATE_CHKSUM:       // �ȴ�У���
    RxChkSum += rx_data;
    if((RxChkSum & 0xFF) == 0x00)
    {RxState = RX_STATE_ED;}
    else
    {RxState = RX_STATE_SD0;}
    break;
  case RX_STATE_ED:           // �ȴ�����λ
    if((rx_data == PROTOCOL_RX_ED) && (RxID == DeviceID))
    {RxPkt();}  // �յ���ȷ������������ַ��ͬ����ִ������
    RxState = RX_STATE_SD0;
    break;
  default:
    RxState = RX_STATE_SD0;
    break;
  }
}

//******************************************************************************
//������  ��RxPkt
//����������������ִ������ܺ���
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
static void RxPkt(void)
{
  u8 i,cmd,len;
  
  TxLen = 0;
  
  cmd = RxBufRead();  // ��ȡ����
  if((cmd <= MAX_CMD_NUM) && (cmdFun[cmd] != 0))
  { // ������Ч�Ҵ��ڶ�Ӧ�Ĺ��ܺ�������ִ������
    TxBuf[TxLen++] = PROTOCOL_TX_SD0;
    TxBuf[TxLen++] = PROTOCOL_TX_SD1;
    TxBuf[TxLen++] = DeviceID;
    TxLen++;  // ΪLENԤ����������ݵĳ���
    TxBuf[TxLen++] = cmd;
    // ִ�й��ܺ���
    len = cmdFun[cmd](&RxBuf[RxRdIx], RxBufCnt-1, &TxBuf[TxLen]);
    TxLen += len;
    TxBuf[PROTOCOL_LEN_ID] = len + 1;
    // ����У���
    for(i=PROTOCOL_ADD_ID,TxChkSum=0; i<TxLen; i++)
    {TxChkSum += TxBuf[i];}
    TxBuf[TxLen++] = TxChkSum;
    TxBuf[TxLen++] = PROTOCOL_TX_ED;
    Delay10MS(&rs485TxDly, 1, TX_DLY_TIME);
    //RS485_Write(TxBuf, TxLen); // ��ʼ��������
  }
}

void PROTOCOL_SendResp(void)
{
  RS485_Write(TxBuf, TxLen); // ��ʼ��������
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

