#include "stm8s_lib.h"
#include "CD74HC166.h"

//******************************************************************************
//函数名  ：HC166_Init
//功能描述：初始化并转串芯片CD74HC166的接口
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void HC166_Init(void)
{
  //串行输入口
  GPIO_Init(HC166_PORT, HC166_IN, GPIO_MODE_IN_PU_NO_IT);
  
  //时钟输出口
  GPIO_Init(HC166_PORT, HC166_CLK, GPIO_MODE_OUT_PP_LOW_FAST);
  //并行数据加载使能输出口
  GPIO_Init(HC166_PORT, HC166_SH, GPIO_MODE_OUT_PP_LOW_FAST);
}

//******************************************************************************
//函数名  ：HC166_Read
//功能描述：从CD74HC166读取数据
//输入    ：pdata   数据存放的地址
//          bits    读取数据的位数
//输出    ：无
//返回值  ：无
//******************************************************************************
void HC166_Read(u8 *pdata, u8 bits)
{
  u8 i,dly;
  
  if((pdata == 0) || (bits == 0))
  {return;}
  
  // 从并行口加载数据
  GPIO_WriteLow(HC166_PORT, HC166_SH);
  for(dly=0; dly<3; dly++);
  GPIO_WriteLow(HC166_PORT, HC166_CLK);
  for(dly=0; dly<3; dly++);
  GPIO_WriteHigh(HC166_PORT, HC166_CLK);
  for(dly=0; dly<3; dly++);
  GPIO_WriteHigh(HC166_PORT, HC166_SH);
  for(dly=0; dly<3; dly++);
  
  // 读取数据
  i=0;
  while(i < bits)
  {
    GPIO_WriteLow(HC166_PORT, HC166_CLK);
    *pdata <<= 1;
#if INPUT_REV
    if(!GPIO_ReadInputPin(HC166_PORT, HC166_IN))
#else
    if(GPIO_ReadInputPin(HC166_PORT, HC166_IN))
#endif
    {*pdata |= 1;}
    else
    {*pdata &= 0xfe;}
    GPIO_WriteHigh(HC166_PORT, HC166_CLK);
    for(dly=0; dly<3; dly++);
    i++;
    if((i&0x07) == 0)
    {pdata++;}
  }
}
