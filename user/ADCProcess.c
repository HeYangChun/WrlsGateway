/*******************************************************************************
*
*   SW
*   Soft version:   
*   File Name:      ADCProcess.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:DTS 模拟数据采样处理
*   Copyright (C) 
*
********************************************************************************/
#include "ADCProcess.h"
#include "GLOBAL.h"
#include "FifoFilter.h"
#include "stm8s_adc2.h"

#define AD_channel7   (GPIO_PIN_7)

dFIFO ADFilterFifo[REAL_ANALOG_NUM];
u08   adc[REAL_ANALOG_NUM]={0x07};//AD模拟通道号，PB7
volatile u08 ucADSampleIndex=0; 

/*******************************************************************************
Function: 		ADCProcInit()
Description: 	模拟数据采样处理初始化
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void ADCProcInit(void)
{
  u08 i;
  
  GPIO_Init(GPIOB, AD_channel7, GPIO_MODE_IN_FL_NO_IT);  // Ain7
  ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D2);
  
  ADC2_DeInit();
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_7, ADC2_PRESSEL_FCPU_D18,\
  ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL7,DISABLE);
  //ADC1_ITConfig(ADC1_IT_EOCIE,ENABLE);
  ADC2_Cmd(ENABLE);
  ADC2_StartConversion();
  
   
  /////////////////////////////////////////////////////////
  for (i = 0; i < REAL_ANALOG_NUM; i++)
  {
   FlushFifoFilter(&ADFilterFifo[i]);//初始化滤波器
  }
}

/*******************************************************************************
Function: 		GetADCValue()
Description: 	模拟数据采样函数
Calls: 			
Called By: 		no
Input: 			通道号
Output: 		ADC采样值
Return: 		no
Others: 		no
********************************************************************************/
word GetADCValue(void) 
{


  // Start the conversion
 // ADC1_StartConversion();

  // Wait until conversion completion
 // while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

  // Get the conversion value
  return ADC2_GetConversionValue();

}

/*******************************************************************************
Function: 		ADCProcess()
Description: 	ADC任务，100us采集一次
Calls: 			
Called By: 		no
Input: 			void
Output: 		void
Return: 		no
Others: 		no
********************************************************************************/
void ADCProcess(void)
{

   u16 adcdata;
   word currentDate;
   //
          adcdata=GetADCValue();
          if(adcdata>=511)
          {
            currentDate=adcdata-511;
          }
          else
          {
            currentDate=511-adcdata;
          }
	  PushFifoFilter(currentDate,&ADFilterFifo[0]); //存平均值
          ConventADToReal();//数据转换
   
   //

 /*  if (ucADSampleIndex < REAL_ANALOG_NUM) // 循环数据采集
   {		
	  adcdata=GetADCValue();
          if(adcdata>=511)
          {
            currentDate=adcdata-511;
          }
          else
          {
            currentDate=511-adcdata;
          }
	  PushFifoFilter(currentDate,&ADFilterFifo[ucADSampleIndex]); //存平均值
	  ucADSampleIndex++;	
   }
   else // 完成一次完成采集轮询
   {
	  ucADSampleIndex=0;
	  ConventADToReal();//数据转换
   }*/

}
/*******************************************************************************
Function: 		ConventADToReal()
Description: 	将AD采样数据转换成实际应用数据
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void ConventADToReal(void)
{
    u08 i;
    
      
   /* for (i = 0; i < REAL_ANALOG_NUM; i++)
    {
     AnalogData[i]=PopFifoFilter(&ADFilterFifo[i]);
    }*/
    AnalogData[0]=PopFifoFilter(&ADFilterFifo[0]);
    currents.I=(WORD)((AnalogData[0]*1.65*1000/512/0.1)*1.1);//mA 有效值  1.1*IAve
    if(currents.I>=10000)
    {
     currents.I=10000;
    }
    
}	


