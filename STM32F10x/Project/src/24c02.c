/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板               */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            24C02源文件,有5个函数                */
/*-------------------------------------------------*/


#include "24c02.h" 
#include "delay.h" 				 

/*-------------------------------------------------*/
/*函数名：初始化IIC接口                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void AT24C02_Init(void)
{
	IIC_Init();            //IIC初始化
}

/*-------------------------------------------------*/
/*函数名：指定地址读出一个数据                     */
/*参  数：ReadAddr:开始读数的地址                  */
/*返回值：读到的数据                               */
/*-------------------------------------------------*/
u8 AT24C02_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
   
	IIC_Start();                               //IIC开始信号
	IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	
	IIC_Wait_Ack();                //等待应答
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	               //等待应答 
	IIC_Start();  	 	           //IIC开始信号
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	               //等待应答
    temp=IIC_Read_Byte(0);		   //读一字节数据     
    IIC_Stop();                    //产生一个停止条件	    
	return temp;                   //返回读取的数据
}

/*-------------------------------------------------*/
/*函数名：指定地址写入一个数据                     */
/*参  数：WriteAddr  :写入数据的目的地址           */
/*参  数：DataToWrite:要写入的数据                 */
/*返回值：无                                       */
/*-------------------------------------------------*/
void AT24C02_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();                                 //开始信号
	IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	                //等待应答
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 	        //等待应答									  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		        //等待应答	   
    IIC_Stop();                     //产生一个停止条件 	  
	delay_ms(2);	                //延时
}

/*-------------------------------------------------*/
/*函数名：指定地址开始读出指定个数的数据           */
/*参  数：ReadAddr :开始读出的地址                 */
/*参  数：pBuffer  :数据数组首地址                 */
/*参  数：NumToRead:要读出数据的个数               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void AT24C02_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)   //循环的一个一个字节的独处，存放在pBuffer指向的空间内
	{
		*pBuffer++=AT24C02_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 

/*-------------------------------------------------*/
/*函数名：指定地址开始写入指定个数的数据           */
/*参  数：WriteAddr :开始写入的地址                */
/*参  数：pBuffer  :数据数组首地址                 */
/*参  数：NumToRead:要写入数据的个数               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void AT24C02_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--) //循环的一个一个的把存放在pBuffer指向的空间内的字节写入，
	{
		AT24C02_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








