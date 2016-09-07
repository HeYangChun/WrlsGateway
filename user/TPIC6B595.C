#include "stm8s_lib.h"
#include "TPIC6B595.h"

//******************************************************************************
//函数名  ：HC595_Init
//功能描述：初始化串转并芯片TPIC6B595的接口
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void HC595_Init(void)
{
  // 串行输出口
  GPIO_Init(HC595_PORT, HC595_SER, GPIO_MODE_OUT_PP_LOW_FAST);
  // 数据移位时钟
  GPIO_Init(HC595_PORT, HC595_SRCK, GPIO_MODE_OUT_PP_LOW_FAST);
  // 寄存器输出信号
  GPIO_Init(HC595_PORT, HC595_RCK, GPIO_MODE_OUT_PP_LOW_FAST);
}

//******************************************************************************
//函数名  ：HC595_Write
//功能描述：从TPIC6B595输出并行数据
//输入    ：pdata   数据存放的地址
//          bits    输出数据的位数
//输出    ：无
//返回值  ：无
//******************************************************************************
void HC595_Write(u8 *pdata, u8 bits)
{
  u8 dly,data;
  
  if((pdata == 0) || (bits == 0))
  {return;}
  
  // 串行输出数据
  pdata += ((bits-1) >> 3);
#if OUTPUT_REV
  data = ~((*pdata) << ((8 - (bits&0x07)) & 0x07));
#else
  data = (*pdata) << ((8 - (bits&0x07)) & 0x07);
#endif
  while(bits > 0)
  {
    GPIO_WriteLow(HC595_PORT, HC595_SRCK);
    if((data&0x80) == 0x80)
    {GPIO_WriteHigh(HC595_PORT, HC595_SER);}
    else
    {GPIO_WriteLow(HC595_PORT, HC595_SER);}
    data <<= 1;
    GPIO_WriteHigh(HC595_PORT, HC595_SRCK);
    for(dly=0; dly<3; dly++);
    bits--;
    if((bits&0x07) == 0)
    {
      pdata--;
#if OUTPUT_REV
      data = ~(*pdata);
#else
      data = *pdata;
#endif
    }
  }
  
  // HC595输出并行数据
  GPIO_WriteLow(HC595_PORT, HC595_RCK);
  for(dly=0; dly<3; dly++);
  GPIO_WriteHigh(HC595_PORT, HC595_RCK);
}



