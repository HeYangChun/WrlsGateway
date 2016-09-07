#include "stm8s_lib.h"
#include "CoinControl.h"

#define COIN_PORT  GPIOD
#define COIN_PIN   GPIO_PIN_3

static u8 coinCounter; // 硬币计数器

//******************************************************************************
//函数名  ：COIN_Init
//功能描述：投币器接口初始化
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void COIN_Init(void)
{
  coinCounter = 0;
  // 投币器信号输入口设置
  GPIO_Init(COIN_PORT, COIN_PIN, GPIO_MODE_IN_PU_NO_IT );
}

//******************************************************************************
//函数名  ：COIN_GetCoinCounter
//功能描述：获取投入的硬币数量
//输入    ：无
//输出    ：无
//返回值  ：投入的硬币数量
//******************************************************************************
u8 COIN_GetCoinCounter(void)
{
  return coinCounter;
}

//******************************************************************************
//函数名  ：CION_ClearCoinCounter
//功能描述：清零投入的硬币数量，为防止发生错误，不能在中断中被调用
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void COIN_ClearCoinCounter(void)
{
  coinCounter = 0;
}

//******************************************************************************
//函数名  ：COIN_Scan
//功能描述：投币器信号扫描，每10ms执行一次，为防止发生错误，不能在中断中被调用
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void COIN_Scan(void)
{
  static u8 sta1 = 0;
  static u8 sta2 = 0;
  static u8 tmp1 = 0;
  static u8 tmp2 = 0;
  
  tmp2 = tmp1;
  if(GPIO_ReadInputPin(COIN_PORT, COIN_PIN))
  {tmp1 = 1;}
  else
  {tmp1 = 0;}
  
  sta2 = sta1;
  sta1 = tmp2&tmp1;
  
  if((!sta2) && sta1)
  {
    coinCounter++;
  }
}
