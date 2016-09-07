/**
  ******************************************************************************
  * @file stm8s_it.c
  * @brief This file contains all the interrupt routines, for Cosmic compiler.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 05/06/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "uart.h"
#include "Delay.h"
#include "ADCProcess.h"
#include "wifi.h"
#include "GLOBAL.h"
#include "gpio.h"
#include "init.h"
#include "sTimeout.h"
#include "IR.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 extern bool MicroSD_Plugged;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
uint iIntPortDCnt=0;//By HeYC
uint iIntPortECnt=0;//By HeYC
/** @addtogroup IT_Functions
  * @{
  */
#define IS_PLUSE_IN_RANGE(X,min1,max1,min2,max2)		( ( (X>=min1) && (X<=max1) ) || ((X>=min2) && (X<=max2) ) )

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
@far @interrupt void NonHandledInterrupt(void)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
@far @interrupt void TRAP_IRQHandler(void)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

#ifdef _RAISONANCE_

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
void TRAP_IRQHandler(void) trap
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

#ifdef _IARSYSTEMS_
/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
#pragma vector=1
__interrupt void TRAP_IRQHandler(void)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
  * @brief Top Level Interrupt Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TLI_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TLI_IRQHandler(void) interrupt 0
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=2
__interrupt void TLI_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void AWU_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void AWU_IRQHandler(void) interrupt 1
#endif	

#ifdef _IAR_SYSTEMS_
#pragma vector=3
__interrupt void AWU_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void CLK_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void CLK_IRQHandler(void) interrupt 2
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=4
__interrupt void CLK_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void EXTI_PORTA_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EXTI_PORTA_IRQHandler(void) interrupt 3
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=5
__interrupt void EXTI_PORTA_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
*/
#ifdef _COSMIC_
@far @interrupt void EXTI_PORTB_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EXTI_PORTB_IRQHandler(void) interrupt 4
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void EXTI_PORTC_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EXTI_PORTC_IRQHandler(void) interrupt 5
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTD Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void EXTI_PORTD_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EXTI_PORTD_IRQHandler(void) interrupt 6
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  	u08 i=0;
	  u08 hTest=0;
	//u08 READBPIO;

        
        //STM8S没有外部中断标志位，STM8L才有外部中断标志位。
        /* Clear the Key Button EXTI line pending bit */
        disableInterrupts();
        read_IR433_trig = GPIO_ReadInputData(GPIOD);//

		//By HeYC
if(read_IR433_trig!=hTest){
	hTest=read_IR433_trig;
	       iIntPortDCnt++;
}
//End:By HeYC
		
	if(IR.REVFLAG==RF_TYPE_433)    //433M
	{
           switch  (IR.DECODESTEP)
	   {
	         case IR_GUIDE:
                
			  if((read_IR433_trig & 0x10)==0x00)
			  {
			    
                            IR.Rcounter=(TIM2->CNTRH)*256;
			       IR.Rcounter+=TIM2->CNTRL;//
			       TIM2->CNTRH=0;
                            TIM2->CNTRL=0;
                            if(	( (IR.Rcounter>=370)&&(IR.Rcounter<=1200) )	||
					( (IR.Rcounter>=2500)&&(IR.Rcounter<=2700) )	||
					( (IR.Rcounter>=100)&&(IR.Rcounter<=200) )		)
                            {
                               IR.cmd[1]=IR.Rcounter;
                               IR.PulseN=1;
                               IR.guide=1;                              
                            }
			  }
			  else if((read_IR433_trig & 0x10)==0x10)
			  {
 	                    if(IR.guide==1)
                           {
                              	IR.Rcounter=(TIM2->CNTRH)*256;
			              IR.Rcounter+=TIM2->CNTRL;//
			              TIM2->CNTRH=0;
                              	TIM2->CNTRL=0;
                              	if(	( (IR.Rcounter>=10000)&&(IR.Rcounter<=13000) )	||
						( (IR.Rcounter>=2400)&&(IR.Rcounter<=2700) )	||
						( (IR.Rcounter>=4000)&&(IR.Rcounter<=5000) )	)
                              	{
                                		IR.cmd[2]=IR.Rcounter;
                                		IR.DECODESTEP=IR_DECODE;
                                		IR.PulseN=2;//2
                                		IR.rfRcvTimer=0;
                              	}
                              	else
                              	{
                                		IR.guide=0;
                              	}
                           }
                           else
                           {
                             	TIM2->CNTRH=0;
                             	TIM2->CNTRL=0;
                             	IR.PulseN=0;
                           }			   
			  }
			  else
			  {
			  }
		      break;
		   case IR_DECODE:
			  IR.PulseN++;//3,4,5...
			  IR.Rcounter=(TIM2->CNTRH)*256;
			  IR.Rcounter+=TIM2->CNTRL;//
			  TIM2->CNTRH=0;
              	  TIM2->CNTRL=0;
			  IR.cmd[IR.PulseN]=IR.Rcounter;//0-长度，1-启动H, 2-启动低，3...
			  if((IR.PulseN>=20)&&(IR.PulseN<21))
              	  {
              	  	//He:fuck, it means IR.PulseN==20??
                		//By HeYC 0901	if(	(	( ( (IR.cmd[13]>=255)&&(IR.cmd[13]<=600))||((IR.cmd[13]>=800)&&(IR.cmd[13]<=1300 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[15]>=255)&&(IR.cmd[15]<=600))||((IR.cmd[15]>=800)&&(IR.cmd[15]<=1300 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[17]>=255)&&(IR.cmd[17]<=600))||((IR.cmd[17]>=800)&&(IR.cmd[17]<=1300 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[19]>=255)&&(IR.cmd[19]<=600))||((IR.cmd[19]>=800)&&(IR.cmd[19]<=1300 ) ) )	)||
				//By HeYC 0901			
				//By HeYC 0901		(	( ( (IR.cmd[13]>=100)&&(IR.cmd[13]<=250))||((IR.cmd[13]>=450)&&(IR.cmd[13]<=800 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[15]>=100)&&(IR.cmd[15]<=250))||((IR.cmd[15]>=450)&&(IR.cmd[15]<=800 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[17]>=100)&&(IR.cmd[17]<=250))||((IR.cmd[17]>=450)&&(IR.cmd[17]<=800 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[19]>=100)&&(IR.cmd[19]<=250))||((IR.cmd[19]>=450)&&(IR.cmd[19]<=800 ) ) )	)||
				//By HeYC 0901			
				//By HeYC 0901		(	( ( (IR.cmd[13]>=100)&&(IR.cmd[13]<=200))||((IR.cmd[13]>=400)&&(IR.cmd[13]<=500 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[15]>=100)&&(IR.cmd[15]<=200))||((IR.cmd[15]>=400)&&(IR.cmd[15]<=500 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[17]>=100)&&(IR.cmd[17]<=200))||((IR.cmd[17]>=400)&&(IR.cmd[17]<=500 ) ) )&&
				//By HeYC 0901			( ( (IR.cmd[19]>=100)&&(IR.cmd[19]<=200))||((IR.cmd[19]>=400)&&(IR.cmd[19]<=500 ) ) )	)	)

				if(	(IS_PLUSE_IN_RANGE(IR.cmd[13],	255,600,	800,1300) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[15],	255,600,	800,1300) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[17],	255,600,	800,1300) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[19],	255,600,	800,1300) ) ||
							 
					(IS_PLUSE_IN_RANGE(IR.cmd[13],	100,250,	450,800) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[15],	100,250,	450,800) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[17],	100,250,	450,800) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[19],	100,250,	450,800) ) ||

					(IS_PLUSE_IN_RANGE(IR.cmd[13],	100,200,	400,500) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[15],	100,200,	400,500) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[17],	100,200,	400,500) &&
					 IS_PLUSE_IN_RANGE(IR.cmd[19],	100,200,	400,500) ) )
				{
                  			IR.DECODESTEP=IR_DECODE;
                		}
                		else
                		{
                  			IR.DECODESTEP=IR_GUIDE;
                  			IR.Learnflag=0;
			      		IR.REVFLAG=RF_TYPE_433;
                  			IR.PulseN=0;
                  			IR.cmd[0]=0;
                  			IR.guide=0;
                  			return;
                		}
              	}
              
              	if(IR.PulseN>=21)
             		{
                		if(IR.cmd[IR.PulseN]>9000)
                		{ 
                  			for(i=3;i<IR.PulseN;i++)
                  			{
                    				if(	 ( (IR.cmd[i]>=255)&&(IR.cmd[i]<=600) )		||
							 ( (IR.cmd[i]>=800)&&(IR.cmd[i]<=1300) )	)
                    				{
                      				IR.Learnflag=1;
			         			IR.REVFLAG=RF_TYPE_NULL;
			          			IR.cmd[0]=IR.PulseN;	//脉冲长度
                    				}
                    				else
                    				{
                      				IR.DECODESTEP=IR_GUIDE;
                      				IR.Learnflag=0;
			          			IR.REVFLAG=RF_TYPE_433;
                      				IR.PulseN=0;
                      				IR.cmd[0]=0;
                      				IR.guide=0;
                      				return;
                    				}
                  			}
                  
                 			IR.PulseN=IR.cmd[0];
                		}
              	}
              
			 if((IR.rfRcvTimer>=IR_END)||(IR.PulseN>=IR_MAX_LEN))
			 {
			   	IR.Learnflag=0xff;//异常退出
			   	IR.REVFLAG=RF_TYPE_NULL;
			   	IR.cmd[0]=IR.PulseN;	//脉冲长度
			  }

			  IR.rfRcvTimer=0;
		      break;
			  
		   case IR_STOP:
		   	  //IR.Learnflag=1;
			 // IR.REVFLAG=0;
		      break;
		   default:
		      IR.REVFLAG=RF_TYPE_NULL;
		      break;
	      }
		
	} 
	//By HeYC	else //学习时不进行探头数据读取
	else if(IR.REVFLAG==RF_TYPE_NULL)
	{
		//He:check if this is an alarm message from alarm sensor
       	//He0830	if( 	(rfInterLock!=1) &&
		//He0830		(rfInterLock!=3) &&
		//He0830		(rfInterLock!=4) &&
		//He0830		(IR.REVFLAG==0) &&
		//He0830		(rfs.type==0)		)//433是否有报警信号
		if( 	( (rfInterLock==IRRS_IDLE) || (rfInterLock==IRRS_433_GUIDE_RCVD) ) &&
			(rfs.type==0)		)
          	{
            		switch  (alarm.DECODESTEP)
	        	{
	         		case IR_GUIDE:
                
			  		if((read_IR433_trig & 0x10)==0x00)
			  		{			    
                            		alarm.Rcounter=(TIM2->CNTRH)*256;
			                	alarm.Rcounter+=TIM2->CNTRL;//
			                	TIM2->CNTRH=0;
                            		TIM2->CNTRL=0;
                            		if(	( (alarm.Rcounter>=370)&&(alarm.Rcounter<=1200) )	||
							( (alarm.Rcounter>=2500)&&(alarm.Rcounter<=2700) )	||
							( (alarm.Rcounter>=100)&&(alarm.Rcounter<=200) )	)
                            		{
                               			IR.cmd[1]=alarm.Rcounter;
                               			alarm.PulseN=1;
                               			alarm.guide=1;
                               			rfInterLock=IRRS_433_GUIDE_RCVD;//读取到引导码，433被锁定，不可去读探头报警
                              
                            		}
			  		}
			  		else if((read_IR433_trig & 0x10)==0x10)
			  		{
			               	if(alarm.guide==1)
                           			{
                              			alarm.Rcounter=(TIM2->CNTRH)*256;
			                  		alarm.Rcounter+=TIM2->CNTRL;//
			                  		TIM2->CNTRH=0;
                              			TIM2->CNTRL=0;
                              			if(	( (alarm.Rcounter>=10000)&&(alarm.Rcounter<=13000) )	||
								( (alarm.Rcounter>=2400)&&(alarm.Rcounter<=2700) )		||
								( (alarm.Rcounter>=4000)&&(alarm.Rcounter<=5000) )		)
                              			{
                                				IR.cmd[2]=alarm.Rcounter;
                                				alarm.DECODESTEP=IR_DECODE;
                                				alarm.PulseN=2;//2
                                				alarm.Tout=0;
                              			}
                              			else
                              			{
                                				alarm.guide=0;
                                				rfInterLock=IRRS_IDLE;//解除锁定
                              			}
                           			}
                           			else
                           			{
                             			TIM2->CNTRH=0;
                             			TIM2->CNTRL=0;
                             			alarm.PulseN=0;
                           			}
			   
			  		}
              			else
			              {
			              }
		      			break;
		   		case IR_DECODE:
			  		alarm.PulseN++;//3,4,5...
			  		alarm.Rcounter=(TIM2->CNTRH)*256;
			  		alarm.Rcounter+=TIM2->CNTRL;//
			  		TIM2->CNTRH=0;
              			TIM2->CNTRL=0;
			  		IR.cmd[alarm.PulseN]=alarm.Rcounter;//0-长度，1-启动H, 2-启动低，3...
			  		if((alarm.PulseN>=20)&&(alarm.PulseN<21))
              			{
			               	//By HeYC 0901	if( 	( 	( ( (IR.cmd[13]>=255)&&(IR.cmd[13]<=600) ) || ( (IR.cmd[13]>=800)&&(IR.cmd[13]<=1300) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[15]>=255)&&(IR.cmd[15]<=600) ) || ( (IR.cmd[15]>=800)&&(IR.cmd[15]<=1300) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[17]>=255)&&(IR.cmd[17]<=600) ) || ( (IR.cmd[17]>=800)&&(IR.cmd[17]<=1300) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[19]>=255)&&(IR.cmd[19]<=600) ) || ( (IR.cmd[19]>=800)&&(IR.cmd[19]<=1300) ) ) 	) ||
						//By HeYC 0901		 	
						//By HeYC 0901		 (	( ( (IR.cmd[13]>=100)&&(IR.cmd[13]<=250) ) || ( (IR.cmd[13]>=450)&&(IR.cmd[13]<=800) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[15]>=100)&&(IR.cmd[15]<=250) ) || ( (IR.cmd[15]>=450)&&(IR.cmd[15]<=800) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[17]>=100)&&(IR.cmd[17]<=250) ) || ( (IR.cmd[17]>=450)&&(IR.cmd[17]<=800) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[19]>=100)&&(IR.cmd[19]<=250) ) || ( (IR.cmd[19]>=450)&&(IR.cmd[19]<=800) ) )		)||
						//By HeYC 0901		 	
						//By HeYC 0901		 (	( ( (IR.cmd[13]>=100)&&(IR.cmd[13]<=200) ) || ( (IR.cmd[13]>=400)&&(IR.cmd[13]<=500) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[15]>=100)&&(IR.cmd[15]<=200) ) || ( (IR.cmd[15]>=400)&&(IR.cmd[15]<=500) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[17]>=100)&&(IR.cmd[17]<=200) ) || ( (IR.cmd[17]>=400)&&(IR.cmd[17]<=500) ) ) &&
						//By HeYC 0901		 	( ( (IR.cmd[19]>=100)&&(IR.cmd[19]<=200) ) || ( (IR.cmd[19]>=400)&&(IR.cmd[19]<=500 ) ) )		)	)

						//#define IS_PLUSE_IN_RANGE(X,min1,max1,min2,max2)		( ( (X>=min1) && (X<=max1) ) || ((X>=min2) && (X<=max2) ) )
						if(	(IS_PLUSE_IN_RANGE(IR.cmd[13],	255,600,	800,1300) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[15],	255,600,	800,1300) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[17],	255,600,	800,1300) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[19],	255,600,	800,1300) ) ||
							 
							(IS_PLUSE_IN_RANGE(IR.cmd[13],	100,250,	450,800) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[15],	100,250,	450,800) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[17],	100,250,	450,800) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[19],	100,250,	450,800) ) ||

							(IS_PLUSE_IN_RANGE(IR.cmd[13],	100,200,	400,500) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[15],	100,200,	400,500) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[17],	100,200,	400,500) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[19],	100,200,	400,500) ) )
                				{
                				
                					alarm.DECODESTEP=IR_DECODE;
                				}
                				else
                				{
				                  	alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                  					alarm.Learnflag=0;
			     				// alarm.REVFLAG=2;
                  					alarm.PulseN=0;
                  					IR.cmd[0]=0;
                  					alarm.guide=0;
                  					rfInterLock=IRRS_IDLE;//解除锁定
                  					return;
                				}
              			}
              
			              if(alarm.PulseN>=21)
              			{
			               	if(IR.cmd[alarm.PulseN]>9000)
                				{ 
				              	for(i=3;i<alarm.PulseN;i++)
                  					{
					              	if(	( (IR.cmd[i]>=255)&&(IR.cmd[i]<=600) )		||
									( (IR.cmd[i]>=800)&&(IR.cmd[i]<=1300) )	)
                    						{
//By HeYC0901                      						alarm.Learnflag=1;
			         					// alarm.REVFLAG=0;
			          					IR.cmd[0]=alarm.PulseN;	//脉冲长度
                      						rfInterLock=IRRS_433_RCV_SUCC;//锁定
                    						}
                    						else
                    						{
                    							alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                      						alarm.Learnflag=0;
			          					//alarm.REVFLAG=2;
                      						alarm.PulseN=0;
                      						IR.cmd[0]=0;
                      						alarm.guide=0;
                      						rfInterLock=IRRS_IDLE;//解除锁定
                      						return;
                    						}
                  					}
                  
				                 	alarm.PulseN=IR.cmd[0];
						}
					}

					if(alarm.Tout>=ALARM_TIMEOUT)//超时退出
              			{
               				alarm.Tout=0;
               				alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                      			alarm.Learnflag=0;
			         		// alarm.REVFLAG=2;
                     			alarm.PulseN=0;
                      			IR.cmd[0]=0;
                      			alarm.guide=0;
				              rfInterLock=IRRS_IDLE;//解除锁定
				       }
		      			break;

		   		default:
		      			alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                      		alarm.Learnflag=0;
			          	//alarm.REVFLAG=2;
                      		alarm.PulseN=0;
                      		IR.cmd[0]=0;
                      		alarm.guide=0;
                      		rfInterLock=IRRS_IDLE;//解除锁定
		      			break;
	      		}
		}        
	}
   
    enableInterrupts();   
}

/**
  * @brief External Interrupt PORTE Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void EXTI_PORTE_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EXTI_PORTE_IRQHandler(void) interrupt 7
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void)
#endif
{
  u08 i=0;
	//u08 READBPIO;
        iIntPortECnt++;
        //STM8S没有外部中断标志位，STM8L才有外部中断标志位。
        /* Clear the Key Button EXTI line pending bit */
        disableInterrupts();
        read_IR315_trig = GPIO_ReadInputData(GPIOE);//
        
       /* for(i=0;i<10;i++)
        {
          READBPIO= GPIO_ReadInputData(GPIOD);
          if(READBPIO!=read_IR315_trig)
          {
	         return;
          }
        } */ 
	if(IR.REVFLAG==RF_TYPE_315)    //315M
	{
		switch  (IR.DECODESTEP)
		{
			case IR_GUIDE:

				if((read_IR315_trig & 0x01)==0x00)
			  	{
			    
                            	IR.Rcounter=(TIM2->CNTRH)*256;
			              IR.Rcounter+=TIM2->CNTRL;//
			              TIM2->CNTRH=0;
                            	TIM2->CNTRL=0;
                            	if( 	( (IR.Rcounter>=370)&&(IR.Rcounter<=1200) ) ||
						( (IR.Rcounter>=280)&&(IR.Rcounter<=400)   ) ||
						( (IR.Rcounter>=200)&&(IR.Rcounter<=300)   )    )

					{
                               		IR.cmd[1]=IR.Rcounter;
                               		IR.PulseN=1;
                               		IR.guide=1;
                            	}
			  	}
			  	else if((read_IR315_trig & 0x01)==0x01)
			  	{
			              if(IR.guide==1)
                           		{
                            		IR.Rcounter=(TIM2->CNTRH)*256;
			                	IR.Rcounter+=TIM2->CNTRL;//
			                	TIM2->CNTRH=0;
                            		TIM2->CNTRL=0;
                            		if(	( (IR.Rcounter>=10000)&&(IR.Rcounter<=12000) )	||
							( (IR.Rcounter>=6500)&&(IR.Rcounter<=8000) )	||
							( (IR.Rcounter>=4500)&&(IR.Rcounter<=5500) )	)

                            		{
		                            	IR.cmd[2]=IR.Rcounter;
                             			IR.DECODESTEP=IR_DECODE;
                             			IR.PulseN=2;//2
                             			IR.rfRcvTimer=0;
                            		}
                            		else
                              		{
                                			IR.guide=0;
                              		}
                           		}
                           		else
                           		{
                             		TIM2->CNTRH=0;
                             		TIM2->CNTRL=0;
                             		IR.PulseN=0;
                           		}
			   
			  	}
              
              		else
              		{
              		}
		      		break;

			case IR_DECODE:
				IR.PulseN++;//3,4,5...
			  	IR.Rcounter=(TIM2->CNTRH)*256;
			  	IR.Rcounter+=TIM2->CNTRL;//
			  	TIM2->CNTRH=0;
              		TIM2->CNTRL=0;
			  	IR.cmd[IR.PulseN]=IR.Rcounter;//0-长度，1-启动H, 2-启动低，3...
			  	if((IR.PulseN>=20)&&(IR.PulseN<21))
              		{
                			//By HeYC 0901	if(	( 	( ( (IR.cmd[13]>=255)&&(IR.cmd[13]<=600) ) ||( (IR.cmd[13]>=800)&&(IR.cmd[13]<=1300) ) )&&
					//By HeYC 0901			( ( (IR.cmd[15]>=255)&&(IR.cmd[15]<=600) ) ||( (IR.cmd[15]>=800)&&(IR.cmd[15]<=1300) ) )&&
					//By HeYC 0901			( ( (IR.cmd[17]>=255)&&(IR.cmd[17]<=600) ) ||( (IR.cmd[17]>=800)&&(IR.cmd[17]<=1300) ) )&&
					//By HeYC 0901			( ( (IR.cmd[19]>=255)&&(IR.cmd[19]<=600) ) ||( (IR.cmd[19]>=800)&&(IR.cmd[19]<=1300) ) )		) ||
					//By HeYC 0901			
					//By HeYC 0901		(	( ( (IR.cmd[13]>=100)&&(IR.cmd[13]<=310) ) ||( (IR.cmd[13]>=400)&&(IR.cmd[13]<=800) ) )&&
					//By HeYC 0901			( ( (IR.cmd[15]>=100)&&(IR.cmd[15]<=310) ) ||( (IR.cmd[15]>=400)&&(IR.cmd[15]<=800) ) )&&
					//By HeYC 0901			( ( (IR.cmd[17]>=100)&&(IR.cmd[17]<=310) ) ||( (IR.cmd[17]>=400)&&(IR.cmd[17]<=800) ) )&&
					//By HeYC 0901			( ( (IR.cmd[19]>=100)&&(IR.cmd[19]<=310) ) ||( (IR.cmd[19]>=400)&&(IR.cmd[19]<=800) ) )		)	)

					if(		(IS_PLUSE_IN_RANGE(IR.cmd[13],	255,600,	800,1300) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[15],	255,600,	800,1300) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[17],	255,600,	800,1300) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[19],	255,600,	800,1300) ) ||
							 
							(IS_PLUSE_IN_RANGE(IR.cmd[13],	100,310,	400,800) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[15],	100,310,	400,800) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[17],	100,310,	400,800) &&
							 IS_PLUSE_IN_RANGE(IR.cmd[19],	100,310,	400,800) )  )
					{
						IR.DECODESTEP=IR_DECODE;
					}
					else
                			{
                  				IR.DECODESTEP=IR_GUIDE;
                  				IR.Learnflag=0;
						IR.REVFLAG=RF_TYPE_315;
                  				IR.PulseN=0;
                  				IR.cmd[0]=0;
                  				IR.guide=0;
                  				return;
                			}
              		}
              
              		if(IR.PulseN>=21)
              		{
                			if(IR.cmd[IR.PulseN]>5000)
                			{ 
                  				for(i=3;i<IR.PulseN;i++)
                  				{
                    					if( 	( (IR.cmd[i]>=255)&&(IR.cmd[i]<=600) )	||
								( (IR.cmd[i]>=800)&&(IR.cmd[i]<=1300) )||
								( (IR.cmd[i]>=100)&&(IR.cmd[i]<=310) )	||
								( (IR.cmd[i]>=400)&&(IR.cmd[i]<=800) ) 	)
                    					{
                      					IR.Learnflag=1;
			          				IR.REVFLAG=RF_TYPE_NULL;
			          				IR.cmd[0]=IR.PulseN;	//脉冲长度
                    					}
                    					else
                    					{
                      					IR.DECODESTEP=IR_GUIDE;
                      					IR.Learnflag=0;
								IR.REVFLAG=RF_TYPE_315;
                      					IR.PulseN=0;
                      					IR.cmd[0]=0;
                      					IR.guide=0;
                      					return;
                    					}
                  				}
                  
                 				IR.PulseN=IR.cmd[0];
                			}
              		}

			  	if((IR.rfRcvTimer>=IR_END)||(IR.PulseN>=IR_MAX_LEN))
			  	{
			   		IR.Learnflag=0xff;//异常退出
			   		IR.REVFLAG=RF_TYPE_NULL;
			   		IR.cmd[0]=IR.PulseN;	//脉冲长度
			  	}
			   	IR.rfRcvTimer=0;
		      		break;

		   case IR_STOP:
		      break;

		   default:
		      IR.REVFLAG=RF_TYPE_NULL;
		      break;
	      }
	} 
    	//By HeYC	else //学习时不进行探头数据读取
    	else if(IR.REVFLAG==RF_TYPE_NULL)//By HeYC
       {
          //By HeYC 0830	if(		(rfInterLock!=2)&&
	//By HeYC 0830		  	(rfInterLock!=3)&&
	//By HeYC 0830		  	(rfInterLock!=4)&&
	//By HeYC 0830		  	(IR.REVFLAG==0)&&
	//By HeYC 0830		  	(rfs.type==0)				)//315是否有报警信号
	  if(	( (rfInterLock==IRRS_IDLE) || (rfInterLock==IRRS_315_GUIDE_RCVD) )&&
		  (rfs.type==0) )//315是否有报警信号
          {
            switch  (alarm.DECODESTEP)
	        {
	         case IR_GUIDE:
                
			  if((read_IR315_trig & 0x01)==0x00)
			  {
			    
                            alarm.Rcounter=(TIM2->CNTRH)*256;
			       alarm.Rcounter+=TIM2->CNTRL;//
			       TIM2->CNTRH=0;
                            TIM2->CNTRL=0;
                            if(	( (alarm.Rcounter>=370)&&(alarm.Rcounter<=1200) )||
					( (alarm.Rcounter>=280)&&(alarm.Rcounter<=400) )  ||
					( (alarm.Rcounter>=200)&&(alarm.Rcounter<=300) )	)
                            {
                               IR.cmd[1]=alarm.Rcounter;
                               alarm.PulseN=1;
                               alarm.guide=1;
                               rfInterLock=IRRS_315_GUIDE_RCVD;//读取到引导码，433被锁定，不可去读探头报警
                              
                            }
			  }
			  else if((read_IR315_trig & 0x01)==0x01)
			  {
			      if(alarm.guide==1)
                           {
                              alarm.Rcounter=(TIM2->CNTRH)*256;
			         alarm.Rcounter+=TIM2->CNTRL;//
			         TIM2->CNTRH=0;
                              TIM2->CNTRL=0;

                              if(	( (alarm.Rcounter>=10000)&&(alarm.Rcounter<=12000) ) 	||
					( (alarm.Rcounter>=6500)&&(alarm.Rcounter<=8000) )    	||
					( (alarm.Rcounter>=4500)&&(alarm.Rcounter<=5500) )			)
                              {
                                IR.cmd[2]=alarm.Rcounter;
                                alarm.DECODESTEP=IR_DECODE;
                                alarm.PulseN=2;//2
                                alarm.Tout=0;
                              }
                              else
                              {
                                alarm.guide=0;
                                rfInterLock=IRRS_IDLE;//解除锁定
                              }
                           }
                           else
                           {
                             TIM2->CNTRH=0;
                             TIM2->CNTRL=0;
                             alarm.PulseN=0;
                           }
			   
			  }
              
              else
              {}
		      break;
		   case IR_DECODE:
			  alarm.PulseN++;//3,4,5...
			  alarm.Rcounter=(TIM2->CNTRH)*256;
			  alarm.Rcounter+=TIM2->CNTRL;//
			  TIM2->CNTRH=0;
              TIM2->CNTRL=0;
			  IR.cmd[alarm.PulseN]=alarm.Rcounter;//0-长度，1-启动H, 2-启动低，3...
			  if((alarm.PulseN>=20)&&(alarm.PulseN<21))
              {
                //By HeYC 0901	if(		(	(	((IR.cmd[13]>=255)&&(IR.cmd[13]<=600)) || ((IR.cmd[13]>=800)&&(IR.cmd[13]<=1300))	)&&
		//By HeYC 0901				(	((IR.cmd[15]>=255)&&(IR.cmd[15]<=600)) || ((IR.cmd[15]>=800)&&(IR.cmd[15]<=1300))	)&&
		//By HeYC 0901				(	((IR.cmd[17]>=255)&&(IR.cmd[17]<=600)) || ((IR.cmd[17]>=800)&&(IR.cmd[17]<=1300))	)&&
		//By HeYC 0901				(	((IR.cmd[19]>=255)&&(IR.cmd[19]<=600)) || ((IR.cmd[19]>=800)&&(IR.cmd[19]<=1300))	)	)||
		//By HeYC 0901			(	(	((IR.cmd[13]>=100)&&(IR.cmd[13]<=310)) || ((IR.cmd[13]>=400)&&(IR.cmd[13]<=800))	)&&
		//By HeYC 0901				(	((IR.cmd[15]>=100)&&(IR.cmd[15]<=310)) || ((IR.cmd[15]>=400)&&(IR.cmd[15]<=800))	)&&
		//By HeYC 0901				(	((IR.cmd[17]>=100)&&(IR.cmd[17]<=310)) || ((IR.cmd[17]>=400)&&(IR.cmd[17]<=800))	)&&
		//By HeYC 0901				(	((IR.cmd[19]>=100)&&(IR.cmd[19]<=310)) || ((IR.cmd[19]>=400)&&(IR.cmd[19]<=800)))	)	)
		 if(		(IS_PLUSE_IN_RANGE(IR.cmd[13],	255,600,	800,1300) &&
				 IS_PLUSE_IN_RANGE(IR.cmd[15],	255,600,	800,1300) &&
				 IS_PLUSE_IN_RANGE(IR.cmd[17],	255,600,	800,1300) &&
				 IS_PLUSE_IN_RANGE(IR.cmd[19],	255,600,	800,1300) ) ||
							 
				(IS_PLUSE_IN_RANGE(IR.cmd[13],	100,310,	400,800) &&
				 IS_PLUSE_IN_RANGE(IR.cmd[15],	100,310,	400,800) &&
				 IS_PLUSE_IN_RANGE(IR.cmd[17],	100,310,	400,800) &&
				 IS_PLUSE_IN_RANGE(IR.cmd[19],	100,310,	400,800) )  )
		  {
                  alarm.DECODESTEP=IR_DECODE;
                }
                else
                {
                  alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                  alarm.Learnflag=0;
			     // alarm.REVFLAG=1;
                  alarm.PulseN=0;
                  IR.cmd[0]=0;
                  alarm.guide=0;
                  rfInterLock=IRRS_IDLE;//解除锁定
                  return;
                }
              }
              
              if(alarm.PulseN>=21)
              {
                if(IR.cmd[alarm.PulseN]>6500)
                { 
                  for(i=3;i<alarm.PulseN;i++)
                  {
                    if(	(	((IR.cmd[i]>=250)&&(IR.cmd[i]<=600))	||
					((IR.cmd[i]>=800)&&(IR.cmd[i]<=1300))	)||
				(	((IR.cmd[i]>=100)&&(IR.cmd[i]<=310))	||
					((IR.cmd[i]>=400)&&(IR.cmd[i]<=800))	)	)
                    {
//By HeYC0901                      alarm.Learnflag=1;
			         // alarm.REVFLAG=0;
			          IR.cmd[0]=alarm.PulseN;	//脉冲长度
                      rfInterLock=IRRS_315_RCV_SUCC;//锁定
                    }
                    else
                    {
                      alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                      alarm.Learnflag=0;
			         // alarm.REVFLAG=1;
                      alarm.PulseN=0;
                      IR.cmd[0]=0;
                      alarm.guide=0;
                      rfInterLock=IRRS_IDLE;//解除锁定
                      return;
                    }
                  }
                  
                 alarm.PulseN=IR.cmd[0];
                }
               /* else if(IR.cmd[alarm.PulseN]>3000)
                { 
                  for(i=3;i<alarm.PulseN-2;i++)
                  {
                    if((((IR.cmd[i]>=100)&&(IR.cmd[i]<=200))||((IR.cmd[i]>=400)&&(IR.cmd[i]<=500))))
                    {
                      alarm.Learnflag=1;
			         // alarm.REVFLAG=0;
			          IR.cmd[0]=alarm.PulseN;	//脉冲长度
                      rfInterLock=3;//锁定
                    }
                    else
                    {
                      alarm.DECODESTEP=IR_GUIDE;
                      alarm.Learnflag=0;
			         // alarm.REVFLAG=1;
                      alarm.PulseN=0;
                      IR.cmd[0]=0;
                      alarm.guide=0;
                      rfInterLock=0;//解除锁定
                      return;
                    }
                  }
                  
                 alarm.PulseN=IR.cmd[0];
                }*/
              }
              if(alarm.Tout>=ALARM_TIMEOUT)//超时退出
              {
               alarm.Tout=0;
               alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                      alarm.Learnflag=0;
			         // alarm.REVFLAG=0;
                      alarm.PulseN=0;
                      IR.cmd[0]=0;
                      alarm.guide=0;
                      rfInterLock=IRRS_IDLE;//解除锁定
              }
		      break;
		   default:
		      alarm.DECODESTEP=IR_GUIDE;
//By HeYC0901                      alarm.Learnflag=0;
			         // alarm.REVFLAG=0;
                      alarm.PulseN=0;
                      IR.cmd[0]=0;
                      alarm.guide=0;
                      rfInterLock=IRRS_IDLE;//解除锁定
		      break;
	      }
          }
                    
        
        }
   
    enableInterrupts();   
}
#ifdef STM8S903
/**
  * @brief External Interrupt PORTF Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void EXTI_PORTF_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EXTI_PORTF_IRQHandler(void) interrupt 8
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xA
__interrupt void EXTI_PORTF_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#ifdef STM8S208
/**
  * @brief CAN RX Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void CAN_RX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void CAN_RX_IRQHandler(void) interrupt 8
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xA
__interrupt void CAN_RX_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief CAN TX Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void CAN_TX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void CAN_TX_IRQHandler(void) interrupt 9
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xB
__interrupt void CAN_TX_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208*/

/**
  * @brief SPI Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void SPI_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void SPI_IRQHandler(void) interrupt 10
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xC
__interrupt void SPI_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) interrupt 11
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Capture/Compare Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM1_CAP_COM_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM1_CAP_COM_IRQHandler(void) interrupt 12
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S903
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void) interrupt 13
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xF
__interrupt void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief Timer5 Capture/Compare Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM5_CAP_COM_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM5_CAP_COM_IRQHandler(void) interrupt 14
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x10
__interrupt void TIM5_CAP_COM_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103*/
/**
  * @brief Timer2 Update/Overflow/Break Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM2_UPD_OVF_BRK_IRQHandler(void) interrupt 13
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0xF
__interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void)
#endif
{
    	//ADCProcess();
    	TIM2_ClearITPendingBit(TIM2_IT_UPDATE); //清中断
    
    	disableInterrupts();
		
	if(rfs.sendflag==1)  //315
	{
		rfs.PulseN++;
	 	if(rfs.PulseN > (IR.cmd[0]-2))
	 	{
	   		if(++rfs.replyCounter>(9+CNT))//已发6次
       		{
          			rfs.PulseN=0;
		  		rfs.sendflag=0;//
				//ByHeYC 0831	      rfs.SendFinish=1;
				//By HeYC 0831		  rfs.sleep=1;
          			rfs.state=0;
          			rfs.type=0;
          			RF315_TX_LO;
	      			TIME2_set(60000);//恢复60ms
          			readbuf[0]=0;
          			readbuf[1]=0;
          			if(send_flag==1)
          			{
            				for(int i=0;i<10;i++)     //服务器控制成功应答
            				{
               				sbuffer[i]=BasicCommand_answer[i];
            				}
					for(int i=10;i<16;i++)
             				{
               				sbuffer[i]=macidHEX[i-10];
             				}
            				//strcat(&sbuffer[10],macidHEX);
            				sbuffer[16] = 1;
            				strcat(&sbuffer[17],RCR_ETX);
            				// strcat(&sbuffer[20],"\r\n");
            				lenth=20;//strlen(sbuffer);
            				SMSSEND(lenth,sbuffer,4);
            				send_flag=0;
          			}
					
          			if(send_flag==2)
          			{
            				for(int i=0;i<10;i++)     //发送控制成功应答
            				{
              				sbuffer[i]=WirelearnAndControl_answer[i];
            				}
              			sbuffer[10] = rfs.number>>8;//keyValH 
              			sbuffer[11] = rfs.number&0xff;//keyValL 
              			sbuffer[12] = 1;
              			strcat(&sbuffer[13],RCR_ETX);
          				// strcat(&sbuffer[16],"\r\n");
              			lenth=16;//strlen(sbuffer);
              			SMSSEND(lenth,sbuffer,2);
              			send_flag=0;
          			}
       		}
       		else
       		{
			         rfs.SENDSTEP=0;
			         rfs.PulseN=1;
			         rfs.PulseLEN=IR.cmd[0];
			         RF315_TX_LO;
       		}
		}

		if(rfs.replyCounter<(10+CNT))
    		{	 
	  		switch  (rfs.SENDSTEP)
	  		{
	   		case  0:
             			RF315_TX_HI;//同步码高
             			TIME2_set(IR.cmd[rfs.PulseLEN-1]);//
	         		rfs.SENDSTEP=1;	
	         		break;
	   		case  1:
	         		RF315_TX_LO;//同步码低
             			TIME2_set(IR.cmd[rfs.PulseLEN]);//
	         		rfs.SENDSTEP=2;
	         		break;
	   		case  2:
            			if((rfs.PulseLEN+1)%2)//奇数  --高
            			{
              			RF315_TX_HI;//同步码高
            			}
            			else//偶数 --低
            			{
              			RF315_TX_LO;//同步码低
            			}
		    		TIME2_set(IR.cmd[rfs.PulseN]);//
            			rfs.PulseLEN--;
	        		break;
	   		default:
	         		break;
	 
			}
		}
	}
	if(rfs.sendflag==2)//433
	{
		rfs.PulseN++;
	 	if(rfs.PulseN > (IR.cmd[0]-2))
	 	{
	   		if(++rfs.replyCounter>(9+CNT))//已发6次
       		{
          			rfs.PulseN=0;
		  		rfs.sendflag=0;//
				//By HeYC 0831	      rfs.SendFinish=1;
				//By HeYC 0831		  rfs.sleep=1;
          			rfs.state=0;
          			rfs.type=0;
          			RF433_TX_LO;
	      			TIME2_set(60000);//恢复60ms
          			if(send_flag==1)
          			{
            				for(int i=0;i<10;i++)   //服务器控制成功应答
            				{
               				sbuffer[i]=BasicCommand_answer[i];
            				}
            				for(int i=10;i<16;i++)
             				{
               				sbuffer[i]=macidHEX[i-10];
             				}
           				// strcat(&sbuffer[10],macidHEX);
            				sbuffer[16] = 1;
            				strcat(&sbuffer[17],RCR_ETX);
            				// strcat(&sbuffer[20],"\r\n");
            				lenth=20;//strlen(sbuffer);
            				SMSSEND(lenth,sbuffer,4);
            				send_flag=0;
          			}
					
          			if(send_flag==2)
          			{
            				for(int i=0;i<10;i++)     //发送控制成功应答
            				{
              				sbuffer[i]=WirelearnAndControl_answer[i];
            				}
              			sbuffer[10] = rfs.number>>8;//keyValH 
              			sbuffer[11] = rfs.number&0xff;//keyValL 
              			sbuffer[12] = 1;
              			strcat(&sbuffer[13],RCR_ETX);
          				// strcat(&sbuffer[16],"\r\n");
              			lenth=16;//strlen(sbuffer);
              			SMSSEND(lenth,sbuffer,2);
              			send_flag=0;
          			}
       		}
       		else
       		{
		       	rfs.SENDSTEP=0;
         			rfs.PulseN=1;
         			rfs.PulseLEN=IR.cmd[0];
         			RF433_TX_LO;
       		}
		}

		if(rfs.replyCounter<(10+CNT))
    		{ 
	  		switch  (rfs.SENDSTEP)
	  		{
	   			case  0:
             				RF433_TX_HI;//同步码高
             				TIME2_set(IR.cmd[rfs.PulseLEN-1]);//
            				// TIME2_set(IR.cmd[1]);//
	         			rfs.SENDSTEP=1;	
	         			break;
	   			case  1:
	         			RF433_TX_LO;//同步码低
                 			TIME2_set(IR.cmd[rfs.PulseLEN]);//
            				// TIME2_set(IR.cmd[2]);//
	         			rfs.SENDSTEP=2;
	         			break;
	   			case  2:
            				if((rfs.PulseLEN+1)%2)//奇数  --高
            				{
              				RF433_TX_HI;//同步码高
            				}
            				else//偶数 --低
            				{
              				RF433_TX_LO;//同步码低
            				}
		    			TIME2_set(IR.cmd[rfs.PulseN]);//
            				rfs.PulseLEN--;
	        			break;
	   			default:
	         			break;
	 
	  		}
    		}	
   	}

	enableInterrupts(); 
}

/**
  * @brief Timer2 Capture/Compare Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM2_CAP_COM_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM2_CAP_COM_IRQHandler(void) interrupt 14
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x10
__interrupt void TIM2_CAP_COM_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

   
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
/**
  * @brief Timer3 Update/Overflow/Break Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM3_UPD_OVF_BRK_IRQHandler(void) interrupt 15
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x11
__interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void)//1ms
#endif
{
	static u8 cnt_100ms = 0;
  	/* In order to detect unexpected events during development,
     	it is recommended to set a breakpoint on the following instruction.
  	*/
	tick_10ms++;
	if(cnt_100ms < 10)
	{	
		cnt_100ms++;	
	}
  	else
  	{
    		tick_100ms++;
    		cnt_100ms = 0;
  	}
 	// if(wifiEnable==2)
 	// {
   	// ADCProcess();//test
  	//  adStart=1;
 	// }
  	sys1msFlag=1;
  	IR.rfRcvTimer++;//对RF学习结束判断计数器
  	alarm.Tout++;//RF报警报警超时计数器
  	if (++sysTickfor10ms >= 10)	// 10ms
  	{
		sysTickfor10ms = 0;
		sys10msFlag    = 1;
		tickcount++;
        
   		// if (++Heartbeattimer5s >= 500)	// 5s
   		// {
      		//  Heartbeattimer5s=0;  
   		// }
        
		
  	}
  	TIM3_ClearITPendingBit(TIM3_IT_UPDATE); //清中断
}

/**
  * @brief Timer3 Capture/Compare Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM3_CAP_COM_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM3_CAP_COM_IRQHandler(void) interrupt 16
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x12
__interrupt void TIM3_CAP_COM_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208, STM8S207 or STM8S105*/

#ifndef STM8S105
/**
  * @brief UART1 TX Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void UART1_TX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void UART1_TX_IRQHandler(void) interrupt 17
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x13
__interrupt void UART1_TX_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if(UART1_GetITStatus(UART1_IT_TC) != RESET)
  {
    UART1->SR = (u8)~(UART1_SR_TC);
//    PROTOCOL_TxHandler();
  }
}

/**
  * @brief UART1 RX Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void UART1_RX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void UART1_RX_IRQHandler(void) interrupt 18
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x14
__interrupt void UART1_RX_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if(UART1_GetITStatus(UART1_IT_RXNE) != RESET)
  {
    UART_RxHandler(UART1_ReceiveData8());
  }
}
#endif /*STM8S105*/

/**
  * @brief I2C Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void I2C_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void I2C_IRQHandler(void) interrupt 19
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x15
__interrupt void I2C_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S105
/**
  * @brief UART2 TX interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void UART2_TX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void UART2_TX_IRQHandler(void) interrupt 20
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x16
__interrupt void UART2_TX_IRQHandler(void)
#endif
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief UART2 RX interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void UART2_RX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void UART2_RX_IRQHandler(void) interrupt 21
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x17
__interrupt void UART2_RX_IRQHandler(void)
#endif
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif /* STM8S105*/

#if defined(STM8S207) || defined(STM8S208)
/**
  * @brief UART3 TX interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void UART3_TX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void UART3_TX_IRQHandler(void) interrupt 20
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x16
__interrupt void UART3_TX_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  
}

/**
  * @brief UART3 RX interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void UART3_RX_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void UART3_RX_IRQHandler(void) interrupt 21
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x17
__interrupt void UART3_RX_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
   if(UART3_GetITStatus(UART3_IT_RXNE) != RESET)
   {
     UART3_RxHandler(UART3_ReceiveData8());
   }
}
#endif /*STM8S208 or STM8S207*/

#if defined(STM8S207) || defined(STM8S208)
/**
  * @brief ADC2 interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void ADC2_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void ADC2_IRQHandler(void) interrupt 22
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x18
__interrupt void ADC2_IRQHandler(void)
#endif
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#else /*STM8S105, STM8S103 or STM8S903*/
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
#ifdef _COSMIC_
@far @interrupt void ADC1_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void ADC1_IRQHandler(void) interrupt 22
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x18
__interrupt void ADC1_IRQHandler(void)
#endif
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif /*STM8S208 or STM8S207*/

#ifdef STM8S903
/**
  * @brief Timer6 Update/Overflow/Trigger Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM6_UPD_OVF_TRG_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM6_UPD_OVF_TRG_IRQHandler(void) interrupt 23
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x19
__interrupt void TIM6_UPD_OVF_TRG_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103*/
/**
  * @brief Timer4 Update/Overflow Interruption routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
#ifdef _COSMIC_
@far @interrupt void TIM4_UPD_OVF_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void TIM4_UPD_OVF_IRQHandler(void) interrupt 23
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

/**
  * @brief Eeprom EEC Interruption routine.
  * @par Parameters:
  * None
  * @retval
	* None
  */
#ifdef _COSMIC_
@far @interrupt void EEPROM_EEC_IRQHandler(void)
#endif

#ifdef _RAISONANCE_
void EEPROM_EEC_IRQHandler(void) interrupt 24
#endif

#ifdef _IAR_SYSTEMS_
#pragma vector=0x1A
__interrupt void EEPROM_EEC_IRQHandler(void)
#endif
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/