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
	// ǰ    �� ��APP�����У�������λ�����͵�������ʶ��������1��Ҫ��������ĵ�ַ��2������ʶ��3�ļ���С���ֽ�������
	// ��һ���� ��ʼ��I2C�ӿڣ���ȡ�洢��������ʶ����ȡ���ص�ַ
	// �ڶ����� ������ʶ��Ч����ʼ������
	// �������� �����洢�����ݣ�����flash��
	// ���Ĳ��� ������λ��׼����ɱ�ʶ��һ�λ�����������ʶ��
	// ���岽�� �������ݣ�дflash
	// �������� �������У��,���ݽ��մ�С��ʵ��ƥ�䣬�������֡��ʶ��
	
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



