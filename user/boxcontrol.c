#include "stm8s_lib.h"
#include "CD74HC166.h"
#include "TPIC6B595.h"
#include "boxcontrol.h"

// 门锁输出，为1则开门
#define DOOR_STA_OP   1
#define DOOR_STA_CL   0
// 照明灯输出，为1则开灯
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
    u8 door1    : 1;  // 开门检测，为0则箱门关闭
    u8 object1  : 1;  // 物体检测，为1则有物体
    u8 door2    : 1;
    u8 object2  : 1;
    u8 door3    : 1;
    u8 object3  : 1;
    u8 door4    : 1;
    u8 object4  : 1;
  }bit;
}tBoxInput;

// 箱子的输入状态，一个结构体包含4个箱子的输入状态
static tBoxInput  box_input[((BOX_NUM_MAX-1)>>2) + 1];
// 箱子的输出状态，一个结构体包含4个箱子的输出状态
static tBoxOutput box_output[((BOX_NUM_MAX-1)>>2) + 1];

static u16 lock_op_dl;                // 开锁通电时间，单位为0.1秒
static u16 lock_op_cnt[BOX_NUM_MAX];  // 为0时停止给锁通电

static u16 light_on_dl;               // 照明时间，单位为0.1秒
static u16 light_on_cnt[BOX_NUM_MAX]; // 为0是关闭照明

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
//函数名  ：Box_Init
//功能描述：初始化串寄存箱的控制接口
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void Box_Init(void)
{
  u8 i;
  
  // 读取保存在片内EEPROM中的开锁时间和照明时间
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  if(!Box_ReadDelayTime(LOCK_OP_DL_ADDR, &lock_op_dl))
  {lock_op_dl = 10;}
  if(!Box_ReadDelayTime(LIGHT_ON_DL_ADDR, &light_on_dl))
  {light_on_dl = 200;}
  if(!Box_ReadRemapTabe(remap_table))
  {
    Box_ResetRemap();
  }
  
  // 初始化串转并、并转串的芯片接口
  HC166_Init();
  HC595_Init();
  
  // 初始化各箱子的输出状态
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
//函数名  ：Box_OpenDoor
//功能描述：打开箱门，同时打开照明
//输入    ：box_num   箱子编号
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_OpenDoors
//功能描述：打开指定的箱门，同时打开照明
//输入    ：box_nums[]   要打开的箱子编号
//          len          箱子的个数
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_SetLockDly
//功能描述：设置开锁通电时间
//输入    ：dly   通电时间，单位秒
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_GetLockDly
//功能描述：获取开锁通电时间
//输入    ：无
//输出    ：无
//返回值  ：通电时间，单位秒
//******************************************************************************
u8 Box_GetLockDly(void)
{
  return (lock_op_dl);
}

//******************************************************************************
//函数名  ：Box_OpenLight
//功能描述：打开箱子照明灯的状态
//输入    ：box_num   箱子编号
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_CloseLight
//功能描述：关闭箱子照明灯的状态
//输入    ：box_num   箱子编号
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_SetLightDly
//功能描述：设置照明时间
//输入    ：dly   照明时间，单位秒
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_GetLightDly
//功能描述：获取照明时间
//输入    ：无
//输出    ：无
//返回值  ：照明时间，单位秒
//******************************************************************************
u8 Box_GetLightDly(void)
{
  return (light_on_dl / 10);
}

//******************************************************************************
//函数名  ：Box_IsDoorClosed
//功能描述：检测箱门的状态
//输入    ：box_num   箱子编号
//输出    ：无
//返回值  ：0：箱子打开  1：箱子关闭
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
//函数名  ：Box_IsAllDoorClosed
//功能描述：检测所有箱子的箱门状态
//输入    ：无
//输出    ：sta   存放箱子的箱门状态
//                STA[0]：1~8号箱子的箱门状态，bit0表示1号箱子的箱门状态，
//                bit7表示8号箱子的箱门状态：0:箱门打开  1:箱门关闭
//                STA[N]：8*N+1~8*(N+1)号箱子的箱门状态，具体意义同上
//返回值  ：sta中的数据个数
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
//函数名  ：Box_IsLightOn
//功能描述：检测箱子照明状态
//输入    ：box_num   箱子编号
//输出    ：无
//返回值  ：0：照明关闭  1：照明打开
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
//函数名  ：Box_IsAllLightOn
//功能描述：检测所有箱子的照明状态
//输入    ：无
//输出    ：sta   存放箱子的照明状态
//                STA[0]：1~8号箱子的照明状态，bit0表示1号箱子的照明状态，
//                bit7表示8号箱子的照明状态：0：照明关闭  1：照明打开
//                STA[N]：8*N+1~8*(N+1)号箱子的照明状态，具体意义同上
//返回值  ：sta中的数据个数
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
//函数名  ：Box_HaveObject
//功能描述：检测箱内是否有物体
//输入    ：box_num   箱子编号
//输出    ：无
//返回值  ：0：箱内无物体  1：箱内有物体
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
//函数名  ：Box_IsAllHaveObject
//功能描述：检测所有箱子是否有物体
//输入    ：无
//输出    ：sta   存放箱子的物体状态
//                STA[0]：1~8号箱子的物体状态，bit0表示1号箱子的物体状态，
//                bit7表示8号箱子的物体状态：0：箱内无物体  1：箱内有物体
//                STA[N]：8*N+1~8*(N+1)号箱子的物体状态，具体意义同上
//返回值  ：sta中的数据个数
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
//函数名  ：Box_SetBoxRemap
//功能描述：设置箱子的重映射
//输入    ：box_num   箱子编号
//          remap     重映射的编号
//输出    ：无
//返回值  ：无
//******************************************************************************
void Box_SetBoxRemap(u8 box_num, u8 remap)
{
  if((box_num > BOX_NUM_MAX) || (box_num == 0))
  {return;}
  
  remap_table[box_num - 1] = remap;
  Box_SaveRemapTable(remap_table);
}

//******************************************************************************
//函数名  ：Box_GetRemapTable
//功能描述：获取箱子的映射表
//输入    ：无
//输出    ：table   获取到的映射表
//返回值  ：table中数据的个数
//******************************************************************************
u8 Box_GetRemapTable(u8 table[])
{
  u8 i;
  
  for(i=0; i<BOX_NUM_MAX; i++)
  {table[i] = remap_table[i];}
  
  return BOX_NUM_MAX;
}

//******************************************************************************
//函数名  ：Box_ResetRemap
//功能描述：重置箱子的映射表
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void Box_ResetRemap(void)
{
  u8 i;
  
  for(i=0; i<BOX_NUM_MAX; i++)
  {remap_table[i] = i + 1;}
  Box_SaveRemapTable(remap_table);
}

//******************************************************************************
//函数名  ：Box_RunPer100MS
//功能描述：线圈通电及照明的计时函数，每0.1秒执行一次
//输入    ：无
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_SetLockSta
//功能描述：设置门锁的状态
//输入    ：box_num   箱子编号
//          sta       门锁状态
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_SetLightSta
//功能描述：设置箱子照明灯的状态
//输入    ：box_num   箱子编号
//          sta       照明灯状态
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_SaveDelayTime
//功能描述：保存延时时间至片内EEPROM
//输入    ：addr      延时时间保存在EEPROM中的位置
//          data      延时时间
//输出    ：无
//返回值  ：无
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
//函数名  ：Box_ReadDelayTime
//功能描述：从EEPROM读取延时时间
//输入    ：addr      延时时间保存在EEPROM中的位置
//输出    ：pdata     延时时间的指针
//返回值  ：0：读取错误   1：读取成功
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
//函数名  ：Box_SaveRemapTable
//功能描述：保存映射表到EEPROM
//输入    ：table      内存中映射表地址
//输出    ：无
//返回值  ：无
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
    IWDG_ReloadCounter();	// 踢看门狗
  }
  sum = ~sum;
  FLASH_ProgramByte(addr, sum);
  
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

//******************************************************************************
//函数名  ：Box_ReadRemapTabe
//功能描述：从EEPROM读取映射表
//输入    ：无
//输出    ：table     内存中映射表存放的位置
//返回值  ：0：读取错误   1：读取成功
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
    IWDG_ReloadCounter();	// 踢看门狗
  }
  sum += FLASH_ReadByte(addr);
  if(sum == 0xff)
  {return 1;}
  else
  {return 0;}
}

//******************************************************************************
//函数名  ：Box_Input
//功能描述：读取箱子的状态
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
static void Box_Input(void)
{
  HC166_Read((u8 *)box_input, BOX_NUM_MAX<<1);
}

//******************************************************************************
//函数名  ：Box_Output
//功能描述：输出箱子的状态
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
static void Box_Output(void)
{
  HC595_Write((u8 *)box_output, BOX_NUM_MAX<<1);
}
