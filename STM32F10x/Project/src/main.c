#include "common.h"
#include "bootcfg.h"
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/* Private function prototypes -----------------------------------------------*/
static void IAP_Init(void);
/* Private functions ---------------------------------------------------------*/
void IAP_Flag_Init(void)
{

	uint32_t rcc = 0;
	GPIO_InitTypeDef GPIO_InitStructure;

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

}
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
unsigned int key_flag = 0;
int main(void)
{
	// 前    ： 在APP程序中，接收上位机发送的升级标识（包含：1需要升级程序的地址，2升级标识，3文件大小（字节数））
	// 第一步： 初始化I2C接口，读取存储区升级标识，读取本地地址
	// 第二步： 升级标识生效，初始化串口
	// 第三步： 擦出存储区数据（解锁flash）
	// 第四步： 发送下位机准备完成标识（一段话或者其他标识）
	// 第五步： 接收数据，写flash
	// 第六步： 接收完成校验,数据接收大小与实际匹配，发送完成帧标识。
	
  /* Flash unlock */
	FLASH_Unlock();
	IAP_Flag_Init();
	
	key_flag = GPIO_ReadInputDataBit(FLAG_PORT,FLAG_PIN);

  if (!key_flag)
  { 
    /* If Key is pressed */
    /* Execute the IAP driver in order to re-program the Flash */
    
		IAP_Init();
    SerialPutString("\r\n======================================================================");
		SerialPutString("\r\n                .\"\".    .\"\",");
		SerialPutString("\r\n                |  |   /  /");
		SerialPutString("\r\n                |  |  /  /");
		SerialPutString("\r\n                |  | /  /");
		SerialPutString("\r\n                |  |/  ;-._ ");
		SerialPutString("\r\n                |  ` _/  / ;");
		SerialPutString("\r\n                |  /` ) /  /");
		SerialPutString("\r\n                | /  /_/\\_/\\");
		SerialPutString("\r\n                |/  /      |");
		SerialPutString("\r\n                (  ' \\ '-  |");
		SerialPutString("\r\n                 \\    `.  /");
		SerialPutString("\r\n                  |      |");
		SerialPutString("\r\n                  |      |");
		SerialPutString("\r\n=                               COPYRIGHT@2019       	IGUICOO       =");
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\r\n");
    Main_Menu ();
  }
  /* Keep the user application running */
  else
  {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) ApplicationAddress);
      Jump_To_Application();
  }
	/* Never run*/
  while (1)
  {
		 
	}
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

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



