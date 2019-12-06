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
	
	// 前    ： 在APP程序中，接收上位机发送的升级标识（包含：1需要升级程序的地址，2升级标识，3文件大小（字节数））
	// 第一步： 初始化I2C接口，读取存储区升级标识，读取本地地址
	// 第二步： 升级标识生效，初始化串口
	// 第三步： 擦出存储区数据（解锁flash）
	// 第四步： 发送下位机准备完成标识（一段话或者其他标识）
	// 第五步： 接收数据，写flash
	// 第六步： 接收完成校验,数据接收大小与实际匹配，发送完成帧标识。
	
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



