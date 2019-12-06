/*-------------------------------------------------*/
/*           超纬电子STM32系列开发板                */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __24C02_H
#define __24C02_H

#include "iic.h"   
				  
u8 AT24C02_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24C02_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24C02_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24C02_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开读出指定长度的数据
void AT24C02_Init(void);                                        //初始化IIC

#endif
















