#include "stm8s_lib.h"
#include "CD74HC166.h"

//******************************************************************************
//������  ��HC166_Init
//������������ʼ����ת��оƬCD74HC166�Ľӿ�
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void HC166_Init(void)
{
  //���������
  GPIO_Init(HC166_PORT, HC166_IN, GPIO_MODE_IN_PU_NO_IT);
  
  //ʱ�������
  GPIO_Init(HC166_PORT, HC166_CLK, GPIO_MODE_OUT_PP_LOW_FAST);
  //�������ݼ���ʹ�������
  GPIO_Init(HC166_PORT, HC166_SH, GPIO_MODE_OUT_PP_LOW_FAST);
}

//******************************************************************************
//������  ��HC166_Read
//������������CD74HC166��ȡ����
//����    ��pdata   ���ݴ�ŵĵ�ַ
//          bits    ��ȡ���ݵ�λ��
//���    ����
//����ֵ  ����
//******************************************************************************
void HC166_Read(u8 *pdata, u8 bits)
{
  u8 i,dly;
  
  if((pdata == 0) || (bits == 0))
  {return;}
  
  // �Ӳ��пڼ�������
  GPIO_WriteLow(HC166_PORT, HC166_SH);
  for(dly=0; dly<3; dly++);
  GPIO_WriteLow(HC166_PORT, HC166_CLK);
  for(dly=0; dly<3; dly++);
  GPIO_WriteHigh(HC166_PORT, HC166_CLK);
  for(dly=0; dly<3; dly++);
  GPIO_WriteHigh(HC166_PORT, HC166_SH);
  for(dly=0; dly<3; dly++);
  
  // ��ȡ����
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
