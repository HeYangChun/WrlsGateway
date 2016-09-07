/*******************************************************************************
*
*   
*   Soft version:   
*   File Name:      AT24C512.c
*   Author   :      
*   creation date:  20011-08-15
*   module description:EEPROM控制函数  65536byte
*   Copyright (C) 
*   地址范围为0000～0FFFFH
********************************************************************************/
#include "AT24C32A.h"
#include "GLOBAL.h"
#include "gpio.h"
#define dalay 50
/*******************************************************************************
Function: 		AT24_init(void)
Description: 	I2C启动
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		下降沿开始
********************************************************************************/
void AT24_init(void)
{
   HXD_A0_LO;
   HXD_A0_LO;//设置器件地址为0x00;

   SET_AT24SDA_OUT;//默认SDA为输出

}

/*******************************************************************************
Function: 		AT24_I2C_START(void)
Description: 	I2C启动
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		下降沿开始
********************************************************************************/
void AT24_I2C_START(void)
{
    int i;
    
	HXD_SCL_LO;
	for(i=0;i<dalay;i++) {;}  // 10us延时
	HXD_SDA_HI;
	for(i=0;i<dalay;i++) {;}//注意先后顺序
	HXD_SCL_HI;
	for(i=0;i<dalay;i++) {;}
	HXD_SDA_LO;
	for(i=0;i<dalay;i++) {;}//下降沿开始
	HXD_SCL_LO;
	for(i=0;i<dalay;i++) {;}
}

/*******************************************************************************
Function: 		AT24_I2C_STOP(void)
Description: 	I2C停止
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		上升沿停止
********************************************************************************/
void AT24_I2C_STOP(void)
{
    int i;
    
	HXD_SCL_LO;
	for(i=0;i<dalay;i++) {;}
	HXD_SDA_LO;
	for(i=0;i<dalay;i++) {;}
	HXD_SCL_HI;
	for(i=0;i<dalay;i++) {;}
	HXD_SDA_HI;
	for(i=0;i<dalay;i++) {;}//上升沿停止
}

/*******************************************************************************
Function: 		AT24_I2C_RecvACK(void)
Description: 	接收应答信号
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		不判断应答信号，可发送空脉冲
********************************************************************************/
void AT24_I2C_RecvACK(void)
{
    int i;
	HXD_SCL_HI;
	for(i=0;i<dalay;i++) {;}
	HXD_SCL_LO;
	for(i=0;i<dalay;i++) {;}
}

/*******************************************************************************
Function: 		AT24_I2C_SendACK(u08 ack)
Description: 	发送应答信号
Calls: 			
Called By: 		no
Input: 			0:ACK 1:NAK
Output: 		no
Return: 		no
Others: 		不需要应答信号，可发送空脉冲
********************************************************************************/
void AT24_I2C_SendACK(u08 ack) 
{ 
    int i;
    if(ack==1)//写应答信号
	  {
	   HXD_SDA_HI;
	  }
    else
	  {
	   HXD_SDA_LO;
	  }
                
    HXD_SCL_HI;                    //拉高时钟线 
    for(i=0;i<dalay;i++) {;}               //延时 
    HXD_SCL_LO;                    //拉低时钟线 
    for(i=0;i<dalay;i++) {;}               //延时 
} 

/*******************************************************************************
Function: 		AT24_I2C_Wdata(u08 data)
Description: 	写数据到AT24C32A
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void AT24_I2C_Wdata(u08 data)
{
	u08 i,j;
	u08 temp;
	temp=data;
	for(i=0;i<8;i++)
	{
		HXD_SCL_LO;
		if((temp&0x80)==0x80)
		  {
		   HXD_SDA_HI;
		  }
        else
		  {
		   HXD_SDA_LO;
		  }
		for(j=0;j<dalay;j++) {;}
		temp=temp<<1;//先发送高位
		HXD_SCL_HI;
		for(j=0;j<dalay;j++) {;}
	}
	HXD_SCL_LO;
	for(j=0;j<dalay;j++) {;}
    AT24_I2C_RecvACK();
}

/*******************************************************************************
Function: 		AT24_I2C_Rdata(void)
Description: 	从AT24C32A读数据,读一个字节
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
u08 AT24_I2C_Rdata(void)
{
	u08 i,j;
	u08 data;
    
	HXD_SCL_LO;
	for(j=0;j<dalay;j++) {;}
	SET_AT24SDA_IN;//设置SDA口为输入
	HXD_SDA_HI;        //使能内部上拉,准备读取数据 

	for(i=0;i<8;i++)
	{
		data=data<<1;//读数据时高位在前
		HXD_SCL_HI;
		for(j=0;j<dalay;j++) {;}
	    if(AT24_GET_SDA)
          {
		    data++;
		  }
		HXD_SCL_LO;
		for(j=0;j<dalay;j++) {;}
	}
	for(j=0;j<dalay;j++) {;}
	SET_AT24SDA_OUT;//设置SDA口为输出
	return data; 
}
/*******************************************************************************
Function: 		AT24_Write_byte(u08 addr,u08 data)
Description: 	向AT24C32写入一字节数据
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void AT24_Write_byte(u16 addr,u08 data)
{
    u08 firstaddr;
	u08 secondaddr;
	
	firstaddr=(u08)(addr>>8);
	secondaddr=(u08)(addr&0x00ff);	

	AT24_I2C_START();
	AT24_I2C_Wdata(0xa0);//写入型总线
	AT24_I2C_Wdata(firstaddr);//写入数据的存储地址高八位
	AT24_I2C_Wdata(secondaddr);//写入数据的存储地址低八位
	AT24_I2C_Wdata(data);//待写入的数据
	AT24_I2C_STOP();
}

/*******************************************************************************
Function: 		AT24_Read_byte(u16 addr)
Description: 	从AT24C32读出一字节数据
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
u08 AT24_Read_byte(u16 addr)
{
    u08 firstaddr;
	u08 secondaddr;
    u08 data;
	
	firstaddr=(u08)(addr>>8);
	secondaddr=(u08)(addr&0x00ff);	


	AT24_I2C_START();
	AT24_I2C_Wdata(0xa0);//写入型总线
	AT24_I2C_Wdata(firstaddr);//需要读出数据的地址高四位
	AT24_I2C_Wdata(secondaddr);//需要读出数据的地址低八位
	AT24_I2C_START();//再一次重新开始

	AT24_I2C_Wdata(0xa1);//读出型总线
	data=AT24_I2C_Rdata();//转存读出的数据

	AT24_I2C_STOP();

	return data;//返回读出的数据

}
/*******************************************************************************
Function: 		AT24_Write_page(u16 addr,u08 *buf,u08 len)
Description: 	向AT24C32写入一页数据
Calls: 			
Called By: 		no
Input: 			addr--页首地址，buf--数据，len--数据长度，AT24C512固定为128-byte Page
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void AT24_Write_page(u16 addr,u08 *buf,u08 len)
{
	u08 *sbuf,i,data;
    u08 firstaddr;
	u08 secondaddr;
	
	sbuf=buf;
    firstaddr=(u08)(addr>>8);
	secondaddr=(u08)(addr&0x00ff);
 
	AT24_I2C_START();
	AT24_I2C_Wdata(0xa0);
	AT24_I2C_Wdata(firstaddr);//需要写入数据的地址高四位
	AT24_I2C_Wdata(secondaddr);//需要写入数据的地址低八位

	for(i=0;i<len;i++)
	{
	  data=*sbuf++;
	  AT24_I2C_Wdata(data);//写入数据到AT24C512
	}

	AT24_I2C_STOP();

}
/*******************************************************************************
Function: 		AT24__Read_Page(u16 addr,u08 *buf,u08 len)
Description: 	向AT24C32读取一页数据
Calls: 			
Called By: 		no
Input: 			addr--页首地址，buf--数据，len--数据长度
Output: 		no
Return: 		no
Others: 		no
********************************************************************************/
void AT24__Read_Page(u16 addr,u08 *buf,u08 len) 
{ 
    u08 *sbuf,i; 
    u08 firstaddr;
	u08 secondaddr;
	
	sbuf=buf;
    firstaddr=(u08)(addr>>8);
	secondaddr=(u08)(addr&0x00ff);
    AT24_I2C_START();            //起始信号 
   	AT24_I2C_Wdata(0xa0);     //发送设备地址+写信号 

  	AT24_I2C_Wdata(firstaddr);//需要读出数据的地址高八位
	AT24_I2C_Wdata(secondaddr);//需要读出数据的地址低八位
    AT24_I2C_START();            //起始信号 
    AT24_I2C_Wdata(0xa1);     //发送设备地址+读信号
	 
    for (i=0; i<len; i++) 
    { 
        sbuf[i] = AT24_I2C_Rdata(); 
        if (i == (len-1)) 
        { 
            AT24_I2C_SendACK(1); //最后一个数据需要回应NAK 
        } 
        else 
        { 
            AT24_I2C_SendACK(0); //回应ACK 
        } 
    } 
    
	AT24_I2C_STOP();             //停止信号 

} 



/* EOF */
