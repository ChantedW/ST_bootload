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
	//SysTick（滴答时钟）的频率是主频率的1/8
	//通常主频率为最大的72M，那么SysTick的频率是9M
	//那么SysTick计数器19个数，代表(1/9)us
}
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*9; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
 		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
/**
  * @brief  Initialize delayms.
  * @param  None
  * @retval None
  */ 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*9*1000;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
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
	for (i=0;i<9;i++)//9个dummy clock
	{
		I2C_SCL_L;
		delay_us(5); 
		I2C_SCL_H ;
	  delay_us(5); 
		// 循环中读取 SDA 值  连续9个为高,有一个不为高 则初始化失败
		if(!READ_SDA)  
		{
			I2C_SCL_L ;
			return 0; 
		}
	}
	// 发送启始条件
	I2C_SDA_OUT();
	delay_us(5);

	I2C_SDA_H;
	delay_us(5);
	I2C_SDA_L;

	return 1; 
 
}

/*-------------------------------------------------*/
/*函数名：初始化IIC接口                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );    //使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化PB8.9
	
	I2C_SCL_H;
	I2C_SDA_H;

}
/*-------------------------------------------------*/
/*函数名：IIC起始信号                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Start(void)
{
	I2C_SDA_OUT();     //SDA线输出模式
	I2C_SDA_H; 		//SDA拉高  	
	I2C_SCL_H;    //SCL拉高
	      
	delay_us(4);   //延时
 	I2C_SDA_L;     //SCL高电平的时候，SDA由高到低，发出一个起始信号
	delay_us(4);   //延时
	I2C_SCL_L;     //SCL拉低
}

/*-------------------------------------------------*/
/*函数名：IIC停止信号                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Stop(void)
{
	I2C_SDA_OUT();     //SDA线输出模式
	I2C_SCL_L; ;     //SCL拉低
	I2C_SDA_L; ;     //SDA拉低//STOP:when CLK is high DATA change form low to high
 	delay_us(4);   //延时
	I2C_SCL_H;     //SCL拉高
	I2C_SDA_H;     //SDA拉高 SCL高电平，SDA由低到高，发出停止信号
	delay_us(4);   //延时 						   	
}

/*-------------------------------------------------*/
/*函数名：等待应答                                 */
/*参  数：无                                       */
/*返回值：0：成功  1：失败                         */
/*-------------------------------------------------*/
u8 IIC_Wait_Ack(void)
{
	u8 timeout=0;
	I2C_SDA_IN();                  //SDA线输出模式 
	I2C_SDA_H;delay_us(1);	   //SDA拉高 延时  
	I2C_SCL_H;delay_us(1);	   //SCL拉高 延时 
	
	while(READ_SDA)            //等待SDA遍地，表示应答带来，不然一直while循环，直到超时
	{
		timeout++;             //超时计数+1
		if(timeout>250)        //如果大于250
		{
			IIC_Stop();        //发送停止信号
			return 1;          //返回1，表示失败
		}
	}
	I2C_SCL_L;                 //SCL拉低
	return 0;                  //返回0，表示成功
} 

/*-------------------------------------------------*/
/*函数名：发送应答                                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Ack(void)
{
	I2C_SDA_OUT();   //SDA线输出模式 
	I2C_SCL_L;   //SCL拉低
	
	I2C_SDA_L;   //SDA拉低，表示应答
	delay_us(2); //延时
	I2C_SCL_H;   //SCL拉高
	delay_us(2); //延时
	I2C_SCL_L;   //SCL拉低
}

/*-------------------------------------------------*/
/*函数名：不发送应答                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/    
void IIC_NAck(void)
{
	I2C_SCL_L;   //SCL拉低
	I2C_SDA_OUT();   //SDA线输出模式 
	I2C_SDA_H;   //SDA拉高，表示不应答
	delay_us(2); //延时
	I2C_SCL_H;   //SCL拉高
	delay_us(2); //延时
	I2C_SCL_L;   //SCL拉低
}	

/*-------------------------------------------------*/
/*函数名：发送一个字节                             */
/*参  数：txd：发送的字节                          */
/*返回值：无                                       */
/*-------------------------------------------------*/ 			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		I2C_SDA_OUT(); 	                  //SDA线输出模式 
    I2C_SCL_L;                    //SCL拉低，开始数据传输
    for(t=0;t<8;t++)              //for循环，一位一位的发送，从最高位 位7开始
    {      
			if((txd&0x80)>>7)
				I2C_SDA_H;
			else
				I2C_SDA_L;
			txd<<=1; 	              //左移一位，准备下一次发送
			delay_us(2);              //延时
			I2C_SCL_H;                //SCL拉高
			delay_us(2);              //延时
			I2C_SCL_L;	              //SCL拉低
			delay_us(2);              //延时
    }	 
} 	    

/*-------------------------------------------------*/
/*函数名：读取一个字节                             */
/*参  数：ack：是否发送应答 1:发 0：不发           */
/*返回值：读取的数据                               */
/*-------------------------------------------------*/   
u8 IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
	
	I2C_SDA_IN();              //SDA设置为输入
   for(i=0;i<8;i++ )      //for循环，一位一位的读取，从最高位 位7开始
	{
			I2C_SCL_L;        //SCL拉低    
			delay_us(2);      //延时
			I2C_SCL_H;        //SCL拉高 
			receive<<=1;      //左移一位，准备下次的读取
			if(READ_SDA)receive++;     //如果读取的是高电平，也就是1，receive+1
			delay_us(1);      //延时
	}					 
	if (!ack)          //不需要发送
			IIC_NAck();    //发送nACK
	else               //需要发送
			IIC_Ack();     //发送ACK   
	return receive;
}

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
	IIC_Send_Byte(EPPROM_WR+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   

	IIC_Wait_Ack();                //等待应答
	IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	               //等待应答 
	IIC_Start();  	 	           //IIC开始信号
	IIC_Send_Byte(EPPROM_RD);           //进入接收模式			   
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
	IIC_Send_Byte(EPPROM_WR+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
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
// 如果是 EPPROM 的话  使用模式I2C
// 定义 初始化引脚
#if IAP_MEMORY_ENALBE 
	
	#if IAP_MEMORY_EEPROM
	
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;       // 推完输出
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		//  复位一次
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB6
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;            //PB7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB7
	
		USART_InitStructure.USART_BaudRate = 115200;                                    //波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式
    USART_Init(USART1, &USART_InitStructure);                                      //按配置设置串口1

		USART_Cmd(USART1, ENABLE);                              //使能串口 1
	
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



