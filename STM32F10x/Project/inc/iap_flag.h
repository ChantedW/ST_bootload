#ifndef  _IAP_FLAG_H
#define  _IAP_FLAG_H

#include <stdint.h>
#include "stm32f10x.h"

/*IO*/ 
#define		IOSET(x)				IOSET_B(x)						// ����IO��  // ���ݵ�Xֵ��һ����GPIOX,GPIO_Pin_x ��ʽ�ĺ궨��
#define		IOSET_B(x,y)		x->BSRR =(y)         

#define		IOCLR(x)				IOCLR_B(x)						// ����IO��
#define		IOCLR_B(x,y)		x->BRR = (y)          

#define		IOGET(x)				IOGET_B(x)						// ��ȡIO�ڵ�״̬
#define		IOGET_B(x,y)		(x->IDR & (y))        

#define		IOBSR(x)  			IOBSR_B(x)   					// ָ��IO�ڷ�ת
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

#define SL_ACK2	0		//Ӧ��
#define SL_NACK2	 !SL_ACK2//��Ӧ��

#define EEPROM_PAGESIZE 	(16)    // 4keeprom ÿҳ16�����ݣ�ÿҳ���д16��
#endif

void  IAP_Flag_Init(void);
void  IAP_Init(void);
uint8_t IAP_Flag_Get(void);
void delay_init(void);
#endif

