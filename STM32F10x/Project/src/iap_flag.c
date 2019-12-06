#include "iap_flag.h"
#if IAP_MEMORY_ENALBE

/**
  * @brief  Initialize delayus.
  * @param  None
  * @retval None
  */  
	
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick���δ�ʱ�ӣ���Ƶ������Ƶ�ʵ�1/8
	//ͨ����Ƶ��Ϊ����72M����ôSysTick��Ƶ����9M
	//��ôSysTick������19����������(1/9)us
}
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*9; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
 		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
/**
  * @brief  Initialize delayms.
  * @param  None
  * @retval None
  */ 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*9*1000;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
#if IAP_MEMORY_EEPROM

uint8_t Write_WRmode,Write_WRaddr,Write_RDdata;
uint8_t READ_RDmode,READ_WRmode,READ_RDaddr,READ_RDdata;

/**
  * @brief  Initialize the eeprom.
  * @param  None
  * @retval None
  */
uint8_t E2PROM_Reset(void)
{
	u8 i;
	I2C_SDA_IN();
	for (i=0;i<9;i++)//9��dummy clock
	{
		I2C_SCL_L;
		delay_us(5); 
		I2C_SCL_H ;
	  delay_us(5); 
		// ѭ���ж�ȡ SDA ֵ  ����9��Ϊ��,��һ����Ϊ�� ���ʼ��ʧ��
		if(!READ_SDA)  
		{
			I2C_SCL_L ;
			return 0; 
		}
	}
	// ������ʼ����
	I2C_SDA_OUT();
	delay_us(5);

	I2C_SDA_H;
	delay_us(5);
	I2C_SDA_L;

	return 1; 
 
}

/*-------------------------------------------------*/
/*����������ʼ��IIC�ӿ�                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );    //ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��PB8.9
	
	I2C_SCL_H;
	I2C_SDA_H;

}
/*-------------------------------------------------*/
/*��������IIC��ʼ�ź�                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Start(void)
{
	I2C_SDA_OUT();     //SDA�����ģʽ
	I2C_SDA_H; 		//SDA����  	
	I2C_SCL_H;    //SCL����
	      
	delay_us(4);   //��ʱ
 	I2C_SDA_L;     //SCL�ߵ�ƽ��ʱ��SDA�ɸߵ��ͣ�����һ����ʼ�ź�
	delay_us(4);   //��ʱ
	I2C_SCL_L;     //SCL����
}

/*-------------------------------------------------*/
/*��������IICֹͣ�ź�                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Stop(void)
{
	I2C_SDA_OUT();     //SDA�����ģʽ
	I2C_SCL_L; ;     //SCL����
	I2C_SDA_L; ;     //SDA����//STOP:when CLK is high DATA change form low to high
 	delay_us(4);   //��ʱ
	I2C_SCL_H;     //SCL����
	I2C_SDA_H;     //SDA���� SCL�ߵ�ƽ��SDA�ɵ͵��ߣ�����ֹͣ�ź�
	delay_us(4);   //��ʱ 						   	
}

/*-------------------------------------------------*/
/*���������ȴ�Ӧ��                                 */
/*��  ������                                       */
/*����ֵ��0���ɹ�  1��ʧ��                         */
/*-------------------------------------------------*/
u8 IIC_Wait_Ack(void)
{
	u8 timeout=0;
	I2C_SDA_IN();                  //SDA�����ģʽ 
	I2C_SDA_H;delay_us(1);	   //SDA���� ��ʱ  
	I2C_SCL_H;delay_us(1);	   //SCL���� ��ʱ 
	
	while(READ_SDA)            //�ȴ�SDA��أ���ʾӦ���������Ȼһֱwhileѭ����ֱ����ʱ
	{
		timeout++;             //��ʱ����+1
		if(timeout>250)        //�������250
		{
			IIC_Stop();        //����ֹͣ�ź�
			return 1;          //����1����ʾʧ��
		}
	}
	I2C_SCL_L;                 //SCL����
	return 0;                  //����0����ʾ�ɹ�
} 

/*-------------------------------------------------*/
/*������������Ӧ��                                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Ack(void)
{
	I2C_SDA_OUT();   //SDA�����ģʽ 
	I2C_SCL_L;   //SCL����
	
	I2C_SDA_L;   //SDA���ͣ���ʾӦ��
	delay_us(2); //��ʱ
	I2C_SCL_H;   //SCL����
	delay_us(2); //��ʱ
	I2C_SCL_L;   //SCL����
}

/*-------------------------------------------------*/
/*��������������Ӧ��                               */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/    
void IIC_NAck(void)
{
	I2C_SCL_L;   //SCL����
	I2C_SDA_OUT();   //SDA�����ģʽ 
	I2C_SDA_H;   //SDA���ߣ���ʾ��Ӧ��
	delay_us(2); //��ʱ
	I2C_SCL_H;   //SCL����
	delay_us(2); //��ʱ
	I2C_SCL_L;   //SCL����
}	

/*-------------------------------------------------*/
/*������������һ���ֽ�                             */
/*��  ����txd�����͵��ֽ�                          */
/*����ֵ����                                       */
/*-------------------------------------------------*/ 			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		I2C_SDA_OUT(); 	                  //SDA�����ģʽ 
    I2C_SCL_L;                    //SCL���ͣ���ʼ���ݴ���
    for(t=0;t<8;t++)              //forѭ����һλһλ�ķ��ͣ������λ λ7��ʼ
    {      
			if((txd&0x80)>>7)
				I2C_SDA_H;
			else
				I2C_SDA_L;
			txd<<=1; 	              //����һλ��׼����һ�η���
			delay_us(2);              //��ʱ
			I2C_SCL_H;                //SCL����
			delay_us(2);              //��ʱ
			I2C_SCL_L;	              //SCL����
			delay_us(2);              //��ʱ
    }	 
} 	    

/*-------------------------------------------------*/
/*����������ȡһ���ֽ�                             */
/*��  ����ack���Ƿ���Ӧ�� 1:�� 0������           */
/*����ֵ����ȡ������                               */
/*-------------------------------------------------*/   
u8 IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
	
	I2C_SDA_IN();              //SDA����Ϊ����
   for(i=0;i<8;i++ )      //forѭ����һλһλ�Ķ�ȡ�������λ λ7��ʼ
	{
			I2C_SCL_L;        //SCL����    
			delay_us(2);      //��ʱ
			I2C_SCL_H;        //SCL���� 
			receive<<=1;      //����һλ��׼���´εĶ�ȡ
			if(READ_SDA)receive++;     //�����ȡ���Ǹߵ�ƽ��Ҳ����1��receive+1
			delay_us(1);      //��ʱ
	}					 
	if (!ack)          //����Ҫ����
			IIC_NAck();    //����nACK
	else               //��Ҫ����
			IIC_Ack();     //����ACK   
	return receive;
}

/*-------------------------------------------------*/
/*����������ʼ��IIC�ӿ�                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_Init(void)
{
	IIC_Init();            //IIC��ʼ��
}

/*-------------------------------------------------*/
/*��������ָ����ַ����һ������                     */
/*��  ����ReadAddr:��ʼ�����ĵ�ַ                  */
/*����ֵ������������                               */
/*-------------------------------------------------*/
u8 AT24C02_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 

	IIC_Start();                               //IIC��ʼ�ź�
	IIC_Send_Byte(EPPROM_WR+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   

	IIC_Wait_Ack();                //�ȴ�Ӧ��
	IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	               //�ȴ�Ӧ�� 
	IIC_Start();  	 	           //IIC��ʼ�ź�
	IIC_Send_Byte(EPPROM_RD);           //�������ģʽ			   
	IIC_Wait_Ack();	               //�ȴ�Ӧ��
	temp=IIC_Read_Byte(0);		   //��һ�ֽ�����     
	IIC_Stop();                    //����һ��ֹͣ����	    
	return temp;                   //���ض�ȡ������
}

/*-------------------------------------------------*/
/*��������ָ����ַд��һ������                     */
/*��  ����WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ           */
/*��  ����DataToWrite:Ҫд�������                 */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
	IIC_Start();                                 //��ʼ�ź�
	IIC_Send_Byte(EPPROM_WR+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	                //�ȴ�Ӧ��
	IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 	        //�ȴ�Ӧ��									  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		        //�ȴ�Ӧ��	   
	IIC_Stop();                     //����һ��ֹͣ���� 	  
	delay_ms(2);	                //��ʱ
}

/*-------------------------------------------------*/
/*��������ָ����ַ��ʼ����ָ������������           */
/*��  ����ReadAddr :��ʼ�����ĵ�ַ                 */
/*��  ����pBuffer  :���������׵�ַ                 */
/*��  ����NumToRead:Ҫ�������ݵĸ���               */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)   //ѭ����һ��һ���ֽڵĶ����������pBufferָ��Ŀռ���
	{
		*pBuffer++=AT24C02_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 

/*-------------------------------------------------*/
/*��������ָ����ַ��ʼд��ָ������������           */
/*��  ����WriteAddr :��ʼд��ĵ�ַ                */
/*��  ����pBuffer  :���������׵�ַ                 */
/*��  ����NumToRead:Ҫд�����ݵĸ���               */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--) //ѭ����һ��һ���İѴ����pBufferָ��Ŀռ��ڵ��ֽ�д�룬
	{
		AT24C02_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

#endif

#endif
/**
  * @brief  Initialize the way of flag: Configure RCC, USART and GPIOs.
  * @param  None
  * @retval None
  */
void IAP_Flag_Init(void)
{
#if IAP_PORT_SET_ENALBE
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	uint32_t rcc = 0;

	if(FLAG_PORT == GPIOA)
	{
		rcc = RCC_APB2Periph_GPIOA;
	}
	else if (FLAG_PORT == GPIOB)
	{
		rcc = RCC_APB2Periph_GPIOB;
	}
	else if (FLAG_PORT == GPIOC)
	{
		rcc = RCC_APB2Periph_GPIOC;
	}
	else if (FLAG_PORT == GPIOD)
	{
		rcc = RCC_APB2Periph_GPIOD;
	}
	else if (FLAG_PORT == GPIOE)
	{
		rcc = RCC_APB2Periph_GPIOE;	
	}
	
 	RCC_APB2PeriphClockCmd(rcc,ENABLE);	
	GPIO_InitStructure.GPIO_Pin  = FLAG_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
 	GPIO_Init(FLAG_PORT, &GPIO_InitStructure);//
	
#endif
// ����� EPPROM �Ļ�  ʹ��ģʽI2C
// ���� ��ʼ������
#if IAP_MEMORY_ENALBE 
	
	#if IAP_MEMORY_EEPROM
	
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;       // �������
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		//  ��λһ��
		E2PROM_Reset();
		
	#endif
	
#endif

}

/**
  * @brief  Initialize the IAP: Configure RCC, USART and GPIOs.
  * @param  None
  * @retval None
  */
void IAP_Init(void)
{
	
	  GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            //PB6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB6
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;            //PB7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB7
	
		USART_InitStructure.USART_BaudRate = 115200;                                    //����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);                                      //���������ô���1

		USART_Cmd(USART1, ENABLE);                              //ʹ�ܴ��� 1
	
}
/**
  * @brief  get flag 
  * @param  None
  * @retval None
  */
uint8_t IAP_Flag_Get(void)
{

	
	#if IAP_PORT_SET_ENALBE

		  return GPIO_ReadInputDataBit(FLAG_PORT,FLAG_PIN);

	#endif
		
	#if IAP_MEMORY_ENALBE
	
			#if IAP_MEMORY_EEPROM &&  (!IAP_MEMORY_FLASH) 
	
			uint8_t flag = 0;
	
			AT24C02_Read(STORE_DATA_ADDR,&flag,1);

			if (flag == 0xA5)
			{
				return 0;
			}
		  else
			{
				return 1;
			}
		
			#endif
	#endif

}



