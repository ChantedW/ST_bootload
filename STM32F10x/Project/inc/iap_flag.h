#ifndef  _IAP_FLAG_H
#define  _IAP_FLAG_H

#include <stdint.h>
#include "stm32f10x.h"

/*IO*/ 
#define		IOSET(x)				IOSET_B(x)						// 设置IO口  // 传递的X值是一个都GPIOX,GPIO_Pin_x 样式的宏定义
#define		IOSET_B(x,y)		x->BSRR =(y)         

#define		IOCLR(x)				IOCLR_B(x)						// 清零IO口
#define		IOCLR_B(x,y)		x->BRR = (y)          

#define		IOGET(x)				IOGET_B(x)						// 读取IO口的状态
#define		IOGET_B(x,y)		(x->IDR & (y))        

#define		IOBSR(x)  			IOBSR_B(x)   					// 指定IO口翻转
#define		IOBSR_B(x,y)		x->ODR = (x->ODR ^ (y)) 
/*********/
#if IAP_MEMORY_ENALBE && IAP_MEMORY_EEPROM
#define   I2C_SCL         GPIOB,GPIO_Pin_8 
#define   I2C_SDA         GPIOB,GPIO_Pin_9 

#define   I2C_SCL_H       (IOSET(I2C_SCL)) 
#define   I2C_SCL_L       (IOCLR(I2C_SCL)) 
#define   I2C_SDA_H       (IOSET(I2C_SDA)) 
#define   I2C_SDA_L       (IOCLR(I2C_SDA)) 

#define 	I2C_SDA_IN()    {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=4<<4;}
#define 	I2C_SDA_OUT()   {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

#define   READ_SDA    (GPIOB->IDR&GPIO_Pin_9)

#define SL_ACK2	0		//应答
#define SL_NACK2	 !SL_ACK2//非应答

#define EEPROM_PAGESIZE 	(16)    // 4keeprom 每页16个数据，每页最多写16个
#endif

void  IAP_Flag_Init(void);
void  IAP_Init(void);
uint8_t IAP_Flag_Get(void);
void delay_init(void);
#endif

