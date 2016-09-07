#ifndef GPIO_H
#define GPIO_H

#include "GLOBAL.h"
#include "stm8s_lib.h"
#include "stm8s_gpio.h"

// 运行灯输出口
#define LED_RUN_PORT    GPIOE
#define LED_RUN_PIN     GPIO_PIN_3

//运行指示灯
#define RUN_ON()	    GPIO_WriteLow(LED_RUN_PORT, LED_RUN_PIN)
#define RUN_OFF()	    GPIO_WriteHigh(LED_RUN_PORT, LED_RUN_PIN)
#define RunLedOn()          RUN_ON()
#define RunLedOff()         RUN_OFF()

// 按键输入
#define KEY_PORT    GPIOA
#define KEY_PIN     GPIO_PIN_6
#define READ_KEY    GPIO_ReadInputPin(KEY_PORT, KEY_PIN)

// WiFi电源控制
#define WIFI_POWER_PORT       GPIOE
#define WIFI_POWER_PIN        GPIO_PIN_6
#define WIFI_POWER_ON()	      GPIO_WriteHigh(WIFI_POWER_PORT, WIFI_POWER_PIN)
#define WIFI_POWER_OFF()      GPIO_WriteLow(WIFI_POWER_PORT, WIFI_POWER_PIN)

//I2C接口

#define HXD_A0_PIN        GPIO_PIN_3
#define HXD_A1_PIN        GPIO_PIN_2

#define HXD_A0_LO      GPIO_WriteLow(GPIOC, HXD_A0_PIN)
#define HXD_A1_LO      GPIO_WriteLow(GPIOC, HXD_A1_PIN)

#define HXD_SCL_PIN        GPIO_PIN_1
//#define HXD_BUSY_PIN       GPIO_PIN_6
#define HXD_SDA_PIN        GPIO_PIN_5

//#define READ_BUSY       GPIO_ReadInputPin(HXD_I2C_PORT, HXD_BUSY_PIN)
#define HXD_SDA_HI      GPIO_WriteHigh(GPIOE, HXD_SDA_PIN)
#define HXD_SDA_LO      GPIO_WriteLow(GPIOE, HXD_SDA_PIN)
#define HXD_SCL_HI      GPIO_WriteHigh(GPIOC, HXD_SCL_PIN)
#define HXD_SCL_LO      GPIO_WriteLow(GPIOC, HXD_SCL_PIN)

#define  SET_AT24SDA_OUT GPIO_Init(GPIOE, HXD_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST )
#define  SET_AT24SDA_IN  GPIO_Init(GPIOE, HXD_SDA_PIN, GPIO_MODE_IN_PU_IT )
#define  AT24_GET_SDA  GPIO_ReadInputPin(GPIOE, HXD_SDA_PIN)

//RF模块引脚定义
#define RF_RX_PORT     GPIOD
#define RF315_RX_PIN   GPIO_PIN_0
#define RF433_RX_PIN   GPIO_PIN_4
#define RF_TX_PORT     GPIOB
#define RF315_TX_PIN   GPIO_PIN_7
#define RF433_TX_PIN   GPIO_PIN_6

#define RF315_TX_HI   GPIO_WriteHigh(RF_TX_PORT, RF315_TX_PIN)
#define RF433_TX_HI   GPIO_WriteHigh(RF_TX_PORT, RF433_TX_PIN)
#define RF315_TX_LO   GPIO_WriteLow(RF_TX_PORT, RF315_TX_PIN)
#define RF433_TX_LO   GPIO_WriteLow(RF_TX_PORT, RF433_TX_PIN)


// 翻转运行灯的状态
//#define LED_RUN_Flash()	LED_RUN_PORT->ODR ^= LED_RUN_PIN


//******************************************************************************
//函数名  ：GPIO_Configuration
//功能描述：IO口初始化
//输入    ：无
//输出    ：无
//返回值  ：无
//******************************************************************************
extern void GPIO_Configuration(void);
extern void EXTI_Configuration(void);
extern void EXTI_EN(void);
extern void EXTI_UN(void);

#endif