#include "stm8s_lib.h"
#include "CD74HC166.h"
#include "TPIC6B595.h"
#include "boxcontrol.h"

// ���������Ϊ1����
#define DOOR_STA_OP   1
#define DOOR_STA_CL   0
// �����������Ϊ1�򿪵�
#define LIGHT_STA_ON  1
#define LIGHT_STA_OFF 0

#define LOCK_OP_DL_ADDR   (FLASH_DATA_START_PHYSICAL_ADDRESS + 0)
#define LIGHT_ON_DL_ADDR  (LOCK_OP_DL_ADDR + 4)
#define REMAP_TABLE_ADDR  (LIGHT_ON_DL_ADDR + 4)

typedef union _boxoutput
{
  u8 all;
  struct
  {
    u8 lock1  : 1;
    u8 light1 : 1;
    u8 lock2  : 1;
    u8 light2 : 1;
    u8 lock3  : 1;
    u8 light3 : 1;
    u8 lock4  : 1;
    u8 light4 : 1;
  }bit;
}tBoxOutput;

typedef union _boxinput
{
  u8 all;
  struct
  {
    u8 door1    : 1;  // ���ż�⣬Ϊ0�����Źر�
    u8 object1  : 1;  // �����⣬Ϊ1��������
    u8 door2    : 1;
    u8 object2  : 1;
    u8 door3    : 1;
    u8 object3  : 1;
    u8 door4    : 1;
    u8 object4  : 1;
  }bit;
}tBoxInput;

// ���ӵ�����״̬��һ���ṹ�����4�����ӵ�����״̬
static tBoxInput  box_input[((BOX_NUM_MAX-1)>>2) + 1];
// ���ӵ����״̬��һ���ṹ�����4�����ӵ����״̬
static tBoxOutput box_output[((BOX_NUM_MAX-1)>>2) + 1];

static u16 lock_op_dl;                // ����ͨ��ʱ�䣬��λΪ0.1��
static u16 lock_op_cnt[BOX_NUM_MAX];  // Ϊ0ʱֹͣ����ͨ��

static u16 light_on_dl;               // ����ʱ�䣬��λΪ0.1��
static u16 light_on_cnt[BOX_NUM_MAX]; // Ϊ0�ǹر�����

static u8 remap_table[BOX_NUM_MAX];

static void Box_Input(void);
static void Box_Output(void);
static void Box_SetLockSta(u8 box_num, u8 sta);
static void Box_SetLightSta(u8 box_num, u8 sta);
static void Box_SaveDelayTime(u32 addr, u16 data);
static u8 Box_ReadDelayTime(u32 addr, u16 *pdata);
static void Box_SaveRemapTable(const u8 * table);
static u8 Box_ReadRemapTabe(u8 * table);

//******************************************************************************
//������  ��Box_Init
//������������ʼ�����Ĵ���Ŀ��ƽӿ�
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_Init(void)
{
  u8 i;
  
  // ��ȡ������Ƭ��EEPROM�еĿ���ʱ�������ʱ��
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  if(!Box_ReadDelayTime(LOCK_OP_DL_ADDR, &lock_op_dl))
  {lock_op_dl = 10;}
  if(!Box_ReadDelayTime(LIGHT_ON_DL_ADDR, &light_on_dl))
  {light_on_dl = 200;}
  if(!Box_ReadRemapTabe(remap_table))
  {
    Box_ResetRemap();
  }
  
  // ��ʼ����ת������ת����оƬ�ӿ�
  HC166_Init();
  HC595_Init();
  
  // ��ʼ�������ӵ����״̬
  for(i=0; i<(((BOX_NUM_MAX-1)>>2) + 1); i++)
  {
    box_output[i].bit.lock1 = DOOR_STA_CL;
    box_output[i].bit.light1 = LIGHT_STA_OFF;
    box_output[i].bit.lock2 = DOOR_STA_CL;
    box_output[i].bit.light2 = LIGHT_STA_OFF;
    box_output[i].bit.lock3 = DOOR_STA_CL;
    box_output[i].bit.light3 = LIGHT_STA_OFF;
    box_output[i].bit.lock4 = DOOR_STA_CL;
    box_output[i].bit.light4 = LIGHT_STA_OFF;
  }
  for(i=0; i<BOX_NUM_MAX; i++)
  {
    lock_op_cnt[i] = 0;
    light_on_cnt[i] = 0;
  }
  
  Box_Output();
  Box_Input();
}

//******************************************************************************
//������  ��Box_OpenDoor
//���������������ţ�ͬʱ������
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_OpenDoor(u8 box_num)
{
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  lock_op_cnt[box_num-1] = lock_op_dl + 1;
  Box_SetLockSta(box_num, DOOR_STA_OP);
  light_on_cnt[box_num-1] = light_on_dl + 1;
  Box_SetLightSta(box_num, LIGHT_STA_ON);
  Box_Output();
}

//******************************************************************************
//������  ��Box_OpenDoors
//������������ָ�������ţ�ͬʱ������
//����    ��box_nums[]   Ҫ�򿪵����ӱ��
//          len          ���ӵĸ���
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_OpenDoors(u8 box_nums[], u8 len)
{
  u8 i,box_num;
  
  if(len > 2)
  {len = 2;}
  for(i=0; i<len; i++)
  {
    box_num = box_nums[i];
    if((box_num > BOX_NUM_MAX) || (box_num == 0))
    {continue;}
    else
    {
      lock_op_cnt[box_num-1] = lock_op_dl + 1;
      Box_SetLockSta(box_num, DOOR_STA_OP);
      light_on_cnt[box_num-1] = light_on_dl + 1;
      Box_SetLightSta(box_num, LIGHT_STA_ON);
    }
  }
  Box_Output();
}

//******************************************************************************
//������  ��Box_SetLockDly
//�������������ÿ���ͨ��ʱ��
//����    ��dly   ͨ��ʱ�䣬��λ��
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_SetLockDly(u8 dly)
{
  if(lock_op_dl != dly)
  {
    lock_op_dl = dly;
    Box_SaveDelayTime(LOCK_OP_DL_ADDR, lock_op_dl);
  }
}

//******************************************************************************
//������  ��Box_GetLockDly
//������������ȡ����ͨ��ʱ��
//����    ����
//���    ����
//����ֵ  ��ͨ��ʱ�䣬��λ��
//******************************************************************************
u8 Box_GetLockDly(void)
{
  return (lock_op_dl);
}

//******************************************************************************
//������  ��Box_OpenLight
//���������������������Ƶ�״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_OpenLight(u8 box_num)
{
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  light_on_cnt[box_num-1] = light_on_dl + 1;
  Box_SetLightSta(box_num, LIGHT_STA_ON);
  Box_Output();
}

//******************************************************************************
//������  ��Box_CloseLight
//�����������ر����������Ƶ�״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_CloseLight(u8 box_num)
{
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  light_on_cnt[box_num-1] = 0;
  Box_SetLightSta(box_num, LIGHT_STA_OFF);
  Box_Output();
}

//******************************************************************************
//������  ��Box_SetLightDly
//������������������ʱ��
//����    ��dly   ����ʱ�䣬��λ��
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_SetLightDly(u8 dly)
{
  u16 tmp;
  
  tmp = dly * 10;
  if(light_on_dl != tmp)
  {
    light_on_dl = tmp;
    Box_SaveDelayTime(LIGHT_ON_DL_ADDR, light_on_dl);
  }
}

//******************************************************************************
//������  ��Box_GetLightDly
//������������ȡ����ʱ��
//����    ����
//���    ����
//����ֵ  ������ʱ�䣬��λ��
//******************************************************************************
u8 Box_GetLightDly(void)
{
  return (light_on_dl / 10);
}

//******************************************************************************
//������  ��Box_IsDoorClosed
//����������������ŵ�״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ��0�����Ӵ�  1�����ӹر�
//******************************************************************************
u8 Box_IsDoorClosed(u8 box_num)
{
  u8 index,sta;
  tBoxInput inputTemp;
  
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return 0;}
  
  Box_Input();
  box_num = remap_table[box_num - 1];
  index = (box_num-1) >> 2;
  inputTemp.all = ~box_input[index].all;
  switch((box_num-1) & 0x03)
  {
  case 0:
    sta = inputTemp.bit.door1;
    break;
  case 1:
    sta = inputTemp.bit.door2;
    break;
  case 2:
    sta = inputTemp.bit.door3;
    break;
  case 3:
    sta = inputTemp.bit.door4;
    break;
  default:
    break;
  }
  
  return sta;
}

//******************************************************************************
//������  ��Box_IsAllDoorClosed
//��������������������ӵ�����״̬
//����    ����
//���    ��sta   ������ӵ�����״̬
//                STA[0]��1~8�����ӵ�����״̬��bit0��ʾ1�����ӵ�����״̬��
//                bit7��ʾ8�����ӵ�����״̬��0:���Ŵ�  1:���Źر�
//                STA[N]��8*N+1~8*(N+1)�����ӵ�����״̬����������ͬ��
//����ֵ  ��sta�е����ݸ���
//******************************************************************************
u8 Box_IsAllDoorClosed(u8 sta[])
{
  u8 stas[BOX_NUM_MAX + 3];
  u8 index;
  tBoxInput inputTemp;
  
  Box_Input();
  for(index=0; index<(((BOX_NUM_MAX-1)>>2) + 1); index++)
  {
    inputTemp.all = ~box_input[index].all;
    if(inputTemp.bit.door1)
    {stas[(index<<2)] = 1;}
    else
    {stas[(index<<2)] = 0;}
    if(inputTemp.bit.door2)
    {stas[(index<<2) + 1] = 1;}
    else
    {stas[(index<<2) + 1] = 0;}
    if(inputTemp.bit.door3)
    {stas[(index<<2) + 2] = 1;}
    else
    {stas[(index<<2) + 2] = 0;}
    if(inputTemp.bit.door4)
    {stas[(index<<2) + 3] = 1;}
    else
    {stas[(index<<2) + 3] = 0;}
  }
  for(index=0; index<BOX_NUM_MAX; index++)
  {
    if(stas[remap_table[index] - 1])
    {sta[index>>3] |= 1<<(index&0x07);}
    else
    {sta[index>>3] &= ~(1<<(index&0x07));}
  }
  
  return (((BOX_NUM_MAX-1)>>3) + 1);
}

//******************************************************************************
//������  ��Box_IsLightOn
//���������������������״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ��0�������ر�  1��������
//******************************************************************************
u8 Box_IsLightOn(u8 box_num)
{
  u8 index,sta;
  
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return 0;}
  
  box_num = remap_table[box_num - 1];
  index = (box_num-1) >> 2;
  switch((box_num-1) & 0x03)
  {
  case 0:
    sta = box_output[index].bit.light1;
    break;
  case 1:
    sta = box_output[index].bit.light2;
    break;
  case 2:
    sta = box_output[index].bit.light3;
    break;
  case 3:
    sta = box_output[index].bit.light4;
    break;
  default:
    sta = 0;
    break;
  }
  
  return sta;
}

//******************************************************************************
//������  ��Box_IsAllLightOn
//��������������������ӵ�����״̬
//����    ����
//���    ��sta   ������ӵ�����״̬
//                STA[0]��1~8�����ӵ�����״̬��bit0��ʾ1�����ӵ�����״̬��
//                bit7��ʾ8�����ӵ�����״̬��0�������ر�  1��������
//                STA[N]��8*N+1~8*(N+1)�����ӵ�����״̬����������ͬ��
//����ֵ  ��sta�е����ݸ���
//******************************************************************************
u8 Box_IsAllLightOn(u8 sta[])
{
  u8 index;
  u8 stas[BOX_NUM_MAX + 3];
  
  for(index=0; index<(((BOX_NUM_MAX-1)>>2) + 1); index++)
  {
    if(box_output[index].bit.light1)
    {stas[(index<<2)] = 1;}
    else
    {stas[(index<<2)] = 0;}
    if(box_output[index].bit.light2)
    {stas[(index<<2) + 1] = 1;}
    else
    {stas[(index<<2) + 1] = 0;}
    if(box_output[index].bit.light3)
    {stas[(index<<2) + 2] = 1;}
    else
    {stas[(index<<2) + 2] = 0;}
    if(box_output[index].bit.light4)
    {stas[(index<<2) + 3] = 1;}
    else
    {stas[(index<<2) + 3] = 0;}
  }
  for(index=0; index<BOX_NUM_MAX; index++)
  {
    if(stas[remap_table[index] - 1])
    {sta[index>>3] |= 1<<(index&0x07);}
    else
    {sta[index>>3] &= ~(1<<(index&0x07));}
  }
  
  return (((BOX_NUM_MAX-1)>>3) + 1);
}

//******************************************************************************
//������  ��Box_HaveObject
//������������������Ƿ�������
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ��0������������  1������������
//******************************************************************************
u8 Box_HaveObject(u8 box_num)
{
  u8 index,sta;
  
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return 0;}
  
  Box_Input();
  box_num = remap_table[box_num - 1];
  index = (box_num-1) >> 2;
  switch((box_num-1) & 0x03)
  {
  case 0:
    sta = box_input[index].bit.object1;
    break;
  case 1:
    sta = box_input[index].bit.object2;
    break;
  case 2:
    sta = box_input[index].bit.object3;
    break;
  case 3:
    sta = box_input[index].bit.object4;
    break;
  default:
    break;
  }
  
  return sta;
}

//******************************************************************************
//������  ��Box_IsAllHaveObject
//����������������������Ƿ�������
//����    ����
//���    ��sta   ������ӵ�����״̬
//                STA[0]��1~8�����ӵ�����״̬��bit0��ʾ1�����ӵ�����״̬��
//                bit7��ʾ8�����ӵ�����״̬��0������������  1������������
//                STA[N]��8*N+1~8*(N+1)�����ӵ�����״̬����������ͬ��
//����ֵ  ��sta�е����ݸ���
//******************************************************************************
u8 Box_IsAllHaveObject(u8 sta[])
{
  u8 index;
  u8 stas[BOX_NUM_MAX + 3];
  
  Box_Input();
  for(index=0; index<(((BOX_NUM_MAX-1)>>2) + 1); index++)
  {
    if(box_input[index].bit.object1)
    {stas[(index<<2)] = 1;}
    else
    {stas[(index<<2)] = 0;}
    if(box_input[index].bit.object2)
    {stas[(index<<2) + 1] = 1;}
    else
    {stas[(index<<2) + 1] = 0;}
    if(box_input[index].bit.object3)
    {stas[(index<<2) + 2] = 1;}
    else
    {stas[(index<<2) + 2] = 0;}
    if(box_input[index].bit.object4)
    {stas[(index<<2) + 3] = 1;}
    else
    {stas[(index<<2) + 3] = 0;}
  }
  for(index=0; index<BOX_NUM_MAX; index++)
  {
    if(stas[remap_table[index] - 1])
    {sta[index>>3] |= 1<<(index&0x07);}
    else
    {sta[index>>3] &= ~(1<<(index&0x07));}
  }
  
  return (((BOX_NUM_MAX-1)>>3) + 1);
}

//******************************************************************************
//������  ��Box_SetBoxRemap
//�����������������ӵ���ӳ��
//����    ��box_num   ���ӱ��
//          remap     ��ӳ��ı��
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_SetBoxRemap(u8 box_num, u8 remap)
{
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  remap_table[box_num - 1] = remap;
  Box_SaveRemapTable(remap_table);
}

//******************************************************************************
//������  ��Box_GetRemapTable
//������������ȡ���ӵ�ӳ���
//����    ����
//���    ��table   ��ȡ����ӳ���
//����ֵ  ��table�����ݵĸ���
//******************************************************************************
u8 Box_GetRemapTable(u8 table[])
{
  u8 i;
  
  for(i=0; i<BOX_NUM_MAX; i++)
  {table[i] = remap_table[i];}
  
  return BOX_NUM_MAX;
}

//******************************************************************************
//������  ��Box_ResetRemap
//�����������������ӵ�ӳ���
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_ResetRemap(void)
{
  u8 i;
  
  for(i=0; i<BOX_NUM_MAX; i++)
  {remap_table[i] = i + 1;}
  Box_SaveRemapTable(remap_table);
}

//******************************************************************************
//������  ��Box_RunPer100MS
//������������Ȧͨ�缰�����ļ�ʱ������ÿ0.1��ִ��һ��
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void Box_RunPer100MS(void)
{
  u8 i;
  
  for(i=0; i<BOX_NUM_MAX; i++)
  {
    if(lock_op_cnt[i] == 1)
    {
      Box_SetLockSta(i+1, DOOR_STA_CL);
      Box_Output();
    }
    if(light_on_cnt[i] == 1)
    {
      Box_SetLightSta(i+1, LIGHT_STA_OFF);
      Box_Output();
    }
    
    if(lock_op_cnt[i] > 0)
    {lock_op_cnt[i]--;}
    if(light_on_cnt[i] > 0)
    {light_on_cnt[i]--;}
  }
}


//******************************************************************************
//������  ��Box_SetLockSta
//��������������������״̬
//����    ��box_num   ���ӱ��
//          sta       ����״̬
//���    ����
//����ֵ  ����
//******************************************************************************
static void Box_SetLockSta(u8 box_num, u8 sta)
{
  u8 index;
  
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  if(sta > 0)
  {sta = 1;}
  box_num = remap_table[box_num - 1];
  index = (box_num-1) >> 2;
  switch((box_num-1) & 0x03)
  {
  case 0:
    box_output[index].bit.lock1 = sta;
    break;
  case 1:
    box_output[index].bit.lock2 = sta;
    break;
  case 2:
    box_output[index].bit.lock3 = sta;
    break;
  case 3:
    box_output[index].bit.lock4 = sta;
    break;
  default:
    break;
  }
}

//******************************************************************************
//������  ��Box_SetLightSta
//�����������������������Ƶ�״̬
//����    ��box_num   ���ӱ��
//          sta       ������״̬
//���    ����
//����ֵ  ����
//******************************************************************************
static void Box_SetLightSta(u8 box_num, u8 sta)
{
  u8 index;
  
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  if(sta > 0)
  {sta = 1;}
  box_num = remap_table[box_num - 1];
  index = (box_num-1) >> 2;
  switch((box_num-1) & 0x03)
  {
  case 0:
    box_output[index].bit.light1 = sta;
    break;
  case 1:
    box_output[index].bit.light2 = sta;
    break;
  case 2:
    box_output[index].bit.light3 = sta;
    break;
  case 3:
    box_output[index].bit.light4 = sta;
    break;
  default:
    break;
  }
}

//******************************************************************************
//������  ��Box_SaveDelayTime
//����������������ʱʱ����Ƭ��EEPROM
//����    ��addr      ��ʱʱ�䱣����EEPROM�е�λ��
//          data      ��ʱʱ��
//���    ����
//����ֵ  ����
//******************************************************************************
static void Box_SaveDelayTime(u32 addr, u16 data)
{
  u16 tmp;
  
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  FLASH_ProgramByte(addr, data & 0xff);
  FLASH_ProgramByte(addr+1, data >> 8);
  tmp = ~data;
  FLASH_ProgramByte(addr+2, tmp & 0xff);
  FLASH_ProgramByte(addr+3, tmp >> 8);
  
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

//******************************************************************************
//������  ��Box_ReadDelayTime
//������������EEPROM��ȡ��ʱʱ��
//����    ��addr      ��ʱʱ�䱣����EEPROM�е�λ��
//���    ��pdata     ��ʱʱ���ָ��
//����ֵ  ��0����ȡ����   1����ȡ�ɹ�
//******************************************************************************
static u8 Box_ReadDelayTime(u32 addr, u16 *pdata)
{
  u8 ret;
  u16 tmp1,tmp2;
  
//  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  tmp1 = ((u16)(FLASH_ReadByte(addr))) | (((u16)(FLASH_ReadByte(addr + 1))) << 8);
  tmp2 = ((u16)(FLASH_ReadByte(addr + 2))) | (((u16)(FLASH_ReadByte(addr + 3))) << 8);
  if((tmp1 + tmp2) == 0xffff)
  {
    *pdata = tmp1;
    ret = 1;
  }
  else
  {ret = 0;}
  
//  FLASH_Lock(FLASH_MEMTYPE_DATA);
  
  return ret;
}

//******************************************************************************
//������  ��Box_SaveRemapTable
//��������������ӳ���EEPROM
//����    ��table      �ڴ���ӳ����ַ
//���    ����
//����ֵ  ����
//******************************************************************************
static void Box_SaveRemapTable(const u8 * table)
{
  u8 i,sum;
  u32 addr;
  
  sum = 0;
  addr = REMAP_TABLE_ADDR;
  
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  for(i=0; i<BOX_NUM_MAX; i++)
  {
    sum += remap_table[i];
    FLASH_ProgramByte(addr++, remap_table[i]);
    IWDG_ReloadCounter();	// �߿��Ź�
  }
  sum = ~sum;
  FLASH_ProgramByte(addr, sum);
  
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

//******************************************************************************
//������  ��Box_ReadRemapTabe
//������������EEPROM��ȡӳ���
//����    ����
//���    ��table     �ڴ���ӳ����ŵ�λ��
//����ֵ  ��0����ȡ����   1����ȡ�ɹ�
//******************************************************************************
static u8 Box_ReadRemapTabe(u8 * table)
{
  u8 i,sum;
  u32 addr;
  
  sum = 0;
  addr = REMAP_TABLE_ADDR;
  for(i=0; i<BOX_NUM_MAX; i++)
  {
    remap_table[i] = FLASH_ReadByte(addr++);
    sum += remap_table[i];
    IWDG_ReloadCounter();	// �߿��Ź�
  }
  sum += FLASH_ReadByte(addr);
  if(sum == 0xff)
  {return 1;}
  else
  {return 0;}
}

//******************************************************************************
//������  ��Box_Input
//������������ȡ���ӵ�״̬
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
static void Box_Input(void)
{
  HC166_Read((u8 *)box_input, BOX_NUM_MAX<<1);
}

//******************************************************************************
//������  ��Box_Output
//����������������ӵ�״̬
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
static void Box_Output(void)
{
  HC595_Write((u8 *)box_output, BOX_NUM_MAX<<1);
}
