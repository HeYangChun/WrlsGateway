/*******************************************************************************
*
*   
*   Soft version:   
*   File Name:      AT24C512.c
*   Author   :      
*   creation date:  20011-08-15
*   module description:EEPROM���ƺ���  65536byte
*   Copyright (C) 
*   ��ַ��ΧΪ0000��0FFFFH
********************************************************************************/
#include "AT24C32A.h"
#include "GLOBAL.h"
#include "gpio.h"
#define dalay 50
/*******************************************************************************
Function: 		AT24_init(void)
Description: 	I2C����
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		�½��ؿ�ʼ
********************************************************************************/
void AT24_init(void)
{
   HXD_A0_LO;
   HXD_A0_LO;//����������ַΪ0x00;

   SET_AT24SDA_OUT;//Ĭ��SDAΪ���

}

/*******************************************************************************
Function: 		AT24_I2C_START(void)
Description: 	I2C����
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		�½��ؿ�ʼ
********************************************************************************/
void AT24_I2C_START(void)
{
    int i;
    
	HXD_SCL_LO;
	for(i=0;i<dalay;i++) {;}  // 10us��ʱ
	HXD_SDA_HI;
	for(i=0;i<dalay;i++) {;}//ע���Ⱥ�˳��
	HXD_SCL_HI;
	for(i=0;i<dalay;i++) {;}
	HXD_SDA_LO;
	for(i=0;i<dalay;i++) {;}//�½��ؿ�ʼ
	HXD_SCL_LO;
	for(i=0;i<dalay;i++) {;}
}

/*******************************************************************************
Function: 		AT24_I2C_STOP(void)
Description: 	I2Cֹͣ
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		������ֹͣ
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
	for(i=0;i<dalay;i++) {;}//������ֹͣ
}

/*******************************************************************************
Function: 		AT24_I2C_RecvACK(void)
Description: 	����Ӧ���ź�
Calls: 			
Called By: 		no
Input: 			no
Output: 		no
Return: 		no
Others: 		���ж�Ӧ���źţ��ɷ��Ϳ�����
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
Description: 	����Ӧ���ź�
Calls: 			
Called By: 		no
Input: 			0:ACK 1:NAK
Output: 		no
Return: 		no
Others: 		����ҪӦ���źţ��ɷ��Ϳ�����
********************************************************************************/
void AT24_I2C_SendACK(u08 ack) 
{ 
    int i;
    if(ack==1)//дӦ���ź�
	  {
	   HXD_SDA_HI;
	  }
    else
	  {
	   HXD_SDA_LO;
	  }
                
    HXD_SCL_HI;                    //����ʱ���� 
    for(i=0;i<dalay;i++) {;}               //��ʱ 
    HXD_SCL_LO;                    //����ʱ���� 
    for(i=0;i<dalay;i++) {;}               //��ʱ 
} 

/*******************************************************************************
Function: 		AT24_I2C_Wdata(u08 data)
Description: 	д���ݵ�AT24C32A
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
		temp=temp<<1;//�ȷ��͸�λ
		HXD_SCL_HI;
		for(j=0;j<dalay;j++) {;}
	}
	HXD_SCL_LO;
	for(j=0;j<dalay;j++) {;}
    AT24_I2C_RecvACK();
}

/*******************************************************************************
Function: 		AT24_I2C_Rdata(void)
Description: 	��AT24C32A������,��һ���ֽ�
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
	SET_AT24SDA_IN;//����SDA��Ϊ����
	HXD_SDA_HI;        //ʹ���ڲ�����,׼����ȡ���� 

	for(i=0;i<8;i++)
	{
		data=data<<1;//������ʱ��λ��ǰ
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
	SET_AT24SDA_OUT;//����SDA��Ϊ���
	return data; 
}
/*******************************************************************************
Function: 		AT24_Write_byte(u08 addr,u08 data)
Description: 	��AT24C32д��һ�ֽ�����
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
	AT24_I2C_Wdata(0xa0);//д��������
	AT24_I2C_Wdata(firstaddr);//д�����ݵĴ洢��ַ�߰�λ
	AT24_I2C_Wdata(secondaddr);//д�����ݵĴ洢��ַ�Ͱ�λ
	AT24_I2C_Wdata(data);//��д�������
	AT24_I2C_STOP();
}

/*******************************************************************************
Function: 		AT24_Read_byte(u16 addr)
Description: 	��AT24C32����һ�ֽ�����
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
	AT24_I2C_Wdata(0xa0);//д��������
	AT24_I2C_Wdata(firstaddr);//��Ҫ�������ݵĵ�ַ����λ
	AT24_I2C_Wdata(secondaddr);//��Ҫ�������ݵĵ�ַ�Ͱ�λ
	AT24_I2C_START();//��һ�����¿�ʼ

	AT24_I2C_Wdata(0xa1);//����������
	data=AT24_I2C_Rdata();//ת�����������

	AT24_I2C_STOP();

	return data;//���ض���������

}
/*******************************************************************************
Function: 		AT24_Write_page(u16 addr,u08 *buf,u08 len)
Description: 	��AT24C32д��һҳ����
Calls: 			
Called By: 		no
Input: 			addr--ҳ�׵�ַ��buf--���ݣ�len--���ݳ��ȣ�AT24C512�̶�Ϊ128-byte Page
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
	AT24_I2C_Wdata(firstaddr);//��Ҫд�����ݵĵ�ַ����λ
	AT24_I2C_Wdata(secondaddr);//��Ҫд�����ݵĵ�ַ�Ͱ�λ

	for(i=0;i<len;i++)
	{
	  data=*sbuf++;
	  AT24_I2C_Wdata(data);//д�����ݵ�AT24C512
	}

	AT24_I2C_STOP();

}
/*******************************************************************************
Function: 		AT24__Read_Page(u16 addr,u08 *buf,u08 len)
Description: 	��AT24C32��ȡһҳ����
Calls: 			
Called By: 		no
Input: 			addr--ҳ�׵�ַ��buf--���ݣ�len--���ݳ���
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
    AT24_I2C_START();            //��ʼ�ź� 
   	AT24_I2C_Wdata(0xa0);     //�����豸��ַ+д�ź� 

  	AT24_I2C_Wdata(firstaddr);//��Ҫ�������ݵĵ�ַ�߰�λ
	AT24_I2C_Wdata(secondaddr);//��Ҫ�������ݵĵ�ַ�Ͱ�λ
    AT24_I2C_START();            //��ʼ�ź� 
    AT24_I2C_Wdata(0xa1);     //�����豸��ַ+���ź�
	 
    for (i=0; i<len; i++) 
    { 
        sbuf[i] = AT24_I2C_Rdata(); 
        if (i == (len-1)) 
        { 
            AT24_I2C_SendACK(1); //���һ��������Ҫ��ӦNAK 
        } 
        else 
        { 
            AT24_I2C_SendACK(0); //��ӦACK 
        } 
    } 
    
	AT24_I2C_STOP();             //ֹͣ�ź� 

} 



/* EOF */
