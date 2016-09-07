/*******************************************************************************
*
*   SW
*   Soft version:   
*   File Name:      ADCProcess.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:DTS ģ�����ݲ�������
*   Copyright (C) 
*
********************************************************************************/
#include "ADCProcess.h"
#include "GLOBAL.h"
#include "FifoFilter.h"
#include "stm8s_adc2.h"

#define AD_channel7   (GPIO_PIN_7)

dFIFO ADFilterFifo[REAL_ANALOG_NUM];
u08   adc[REAL_ANALOG_NUM]={0x07};//ADģ��ͨ���ţ�PB7
volatile u08 ucADSampleIndex=0; 

/*******************************************************************************
Function: 		ADCProcInit()
Description: 	ģ�����ݲ��������ʼ��
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
   FlushFifoFilter(&ADFilterFifo[i]);//��ʼ���˲���
  }
}

/*******************************************************************************
Function: 		GetADCValue()
Description: 	ģ�����ݲ�������
Calls: 			
Called By: 		no
Input: 			ͨ����
Output: 		ADC����ֵ
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
Description: 	ADC����100us�ɼ�һ��
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
	  PushFifoFilter(currentDate,&ADFilterFifo[0]); //��ƽ��ֵ
          ConventADToReal();//����ת��
   
   //

 /*  if (ucADSampleIndex < REAL_ANALOG_NUM) // ѭ�����ݲɼ�
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
	  PushFifoFilter(currentDate,&ADFilterFifo[ucADSampleIndex]); //��ƽ��ֵ
	  ucADSampleIndex++;	
   }
   else // ���һ����ɲɼ���ѯ
   {
	  ucADSampleIndex=0;
	  ConventADToReal();//����ת��
   }*/

}
/*******************************************************************************
Function: 		ConventADToReal()
Description: 	��AD��������ת����ʵ��Ӧ������
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
    currents.I=(WORD)((AnalogData[0]*1.65*1000/512/0.1)*1.1);//mA ��Чֵ  1.1*IAve
    if(currents.I>=10000)
    {
     currents.I=10000;
    }
    
}	


