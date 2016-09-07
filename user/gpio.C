#include "stm8s_lib.h"
#include "gpio.h"

//******************************************************************************
//函数名  ：GPIO_Configuration
//功能描述：IO口初始化
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
void GPIO_Configuration(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD); //复位GPIOD  PD端口的设置恢复为初始值
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOG);
  
  // 运行灯输出口
  GPIO_Init(LED_RUN_PORT, LED_RUN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
  // 按键输入
  GPIO_Init(KEY_PORT, KEY_PIN, GPIO_MODE_IN_PU_NO_IT );
  
  // 继电器输出口
  //GPIO_Init(REALY_PORT, REALY_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  
  // WIFI电源输出口
  GPIO_Init(WIFI_POWER_PORT, WIFI_POWER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  

  // I2C接口
 // GPIO_Init(HXD_I2C_PORT, HXD_BUSY_PIN, GPIO_MODE_IN_PU_NO_IT );
  GPIO_Init(GPIOE, HXD_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
  GPIO_Init(GPIOC, HXD_SCL_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
  
  //RF 接口
  GPIO_Init(RF_TX_PORT, RF315_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST );
  GPIO_Init(RF_TX_PORT, RF433_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST );
  
  GPIO_Init(GPIOE, RF315_RX_PIN, GPIO_MODE_IN_PU_IT );//
  GPIO_Init(RF_RX_PORT, RF433_RX_PIN, GPIO_MODE_IN_PU_IT );//先处理433中断，防止2路互相影响，硬件需改为不在同一GPIO组
  
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
 enableInterrupts();     /* 开中断 */
}
void EXTI_UN(void)
{
 disableInterrupts(); /* 关中断 */ 
}

