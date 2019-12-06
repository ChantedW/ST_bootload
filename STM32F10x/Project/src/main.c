//	 *@@@@@@* **@@@@** @******@ *@@@@@@* **@@@@** **@@@@** **@@@@** 
//	 ***@@*** *@****@* @******@ ***@@*** *@****@* *@****@* *@****@* 
//	 ***@@*** @******@ @******@ ***@@*** @******* @******@ @******@
//	 ***@@*** @******* @******@ ***@@*** @******* @******@ @******@ 
//	 ***@@*** @***@@@@ @******@ ***@@*** @******* @******@ @******@ 
//	 ***@@*** *@****@* @******@ ***@@*** *@****@* *@****@* *@****@* 
//	 *@@@@@@* **@@@@** *@@@@@@* *@@@@@@* **@@@@** **@@@@** **@@@@**

#include "common.h"
#include "bootcfg.h"
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t key_flag = 0;
	
	// ǰ    �� ��APP�����У�������λ�����͵�������ʶ��������1��Ҫ��������ĵ�ַ��2������ʶ��3�ļ���С���ֽ�������
	// ��һ���� ��ʼ��I2C�ӿڣ���ȡ�洢��������ʶ����ȡ���ص�ַ
	// �ڶ����� ������ʶ��Ч����ʼ������
	// �������� �����洢�����ݣ�����flash��
	// ���Ĳ��� ������λ��׼����ɱ�ʶ��һ�λ�����������ʶ��
	// ���岽�� �������ݣ�дflash
	// �������� �������У��,���ݽ��մ�С��ʵ��ƥ�䣬�������֡��ʶ��
	
  /* Flash unlock */
#if IAP_MEMORY_ENALBE
	delay_init();
#endif
	FLASH_Unlock();
	
	IAP_Flag_Init();
	
  key_flag = IAP_Flag_Get();
        
  if (!key_flag)
  { 
    /* If Key is pressed */
    /* Execute the IAP driver in order to re-program the Flash */
    
		IAP_Init();
    SerialPutString("\r\n===============================================================");

		SerialPutString("\r\n*@@@@@@* **@@@@** @******@ *@@@@@@* **@@@@** **@@@@** **@@@@** ");
		SerialPutString("\r\n***@@*** *@****@* @******@ ***@@*** *@****@* *@****@* *@****@* ");
		SerialPutString("\r\n***@@*** @******* @******@ ***@@*** @******* @******@ @******@ ");
		SerialPutString("\r\n***@@*** @******* @******@ ***@@*** @******* @******@ @******@ ");
		SerialPutString("\r\n***@@*** @***@@@@ @******@ ***@@*** @******* @******@ @******@ ");
		SerialPutString("\r\n***@@*** *@****@@ @******@ ***@@*** *@****@* *@****@* *@****@* ");
		SerialPutString("\r\n*@@@@@@* **@@@@*@ *@@@@@@* *@@@@@@* **@@@@** **@@@@** **@@@@** ");
		
		SerialPutString("\r\n=              COPYRIGHT@2019       	IGUICOO              =  ");
    SerialPutString("\r\n===============================================================");
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
	SerialPutString("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
	while (1)
  {
		
  }
}


#endif



