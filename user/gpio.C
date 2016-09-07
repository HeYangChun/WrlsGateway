#include "stm8s_lib.h"
#include "gpio.h"

//******************************************************************************
//������  ��GPIO_Configuration
//����������IO�ڳ�ʼ��
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
void GPIO_Configuration(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD); //��λGPIOD  PD�˿ڵ����ûָ�Ϊ��ʼֵ
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOG);
  
  // ���е������
  GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
  // ��������
  GPIO_Init(KEY_PORT, KEY_PIN, GPIO_MODE_IN_PU_NO_IT );
  
  // �̵��������
  //GPIO_Init(REALY_PORT, REALY_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  
  // WIFI��Դ�����
  GPIO_Init(WIFI_POWER_PORT, WIFI_POWER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  

  // I2C�ӿ�
 // GPIO_Init(HXD_I2C_PORT, HXD_BUSY_PIN, GPIO_MODE_IN_PU_NO_IT );
  GPIO_Init(GPIOE, HXD_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
  GPIO_Init(GPIOC, HXD_SCL_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
  
  //RF �ӿ�
  GPIO_Init(RF_TX_PORT, RF315_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST );
  GPIO_Init(RF_TX_PORT, RF433_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST );
  
  GPIO_Init(GPIOE, RF315_RX_PIN, GPIO_MODE_IN_PU_IT );//
  GPIO_Init(RF_RX_PORT, RF433_RX_PIN, GPIO_MODE_IN_PU_IT );//�ȴ���433�жϣ���ֹ2·����Ӱ�죬Ӳ�����Ϊ����ͬһGPIO��
  
  RF433_TX_LO;
  RF315_TX_LO;
  WIFI_POWER_ON();
  //REALY_OFF();
}


/*void REALY_ON(void)
{
 REALY_H;
 REALYState=1;
 delayoffCounter=0;
}

void REALY_OFF(void)
{
 REALY_L;
 REALYState=0;
}*/


/*******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void EXTI_Configuration(void)
{
  EXTI_DeInit();
  
  EXTI_SetExtIntSensitivity((EXTI_PORT_GPIOD),EXTI_SENSITIVITY_RISE_FALL);
  EXTI_SetExtIntSensitivity((EXTI_PORT_GPIOE),EXTI_SENSITIVITY_RISE_FALL);
  
}

void EXTI_EN(void)
{
 enableInterrupts();     /* ���ж� */
}
void EXTI_UN(void)
{
 disableInterrupts(); /* ���ж� */ 
}
