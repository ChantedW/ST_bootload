#include "stm32f10x_it.h"

/** @addtogroup IAP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	
}
__asm void wait (void)
{
		BX lr
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
 wait ();
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
		
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

	
}


typedef  void (*pFunction)(void);     // º¯ÊýÖ¸Õë
/*add by zhangjinming*/
void m_enter_func(unsigned int offset_addr)
{
		/*add by zhangjinming*/
	  pFunction Jump_To_Application;
	  uint32_t JumpAddress;
		#define TAB_ADDR 0x08002000
		/*add end*/
		JumpAddress = *(__IO uint32_t*) (TAB_ADDR + offset_addr);
    Jump_To_Application = (pFunction) JumpAddress;
    Jump_To_Application();
}

void WWDG_IRQHandler(void)
{
		m_enter_func(0x40);
}
void PVD_IRQHandler(void)
{
		m_enter_func(0x44);
}

void TAMPER_IRQHandler(void)
{
		m_enter_func(0x48);
}
void RTC_IRQHandler(void)
{
		m_enter_func(0x4C);
}
void FLASH_IRQHandler(void)
{
		m_enter_func(0x50);
}

void RCC_IRQHandler(void)
{
		m_enter_func(0x54);
}

void EXTI0_IRQHandler(void)
{
		m_enter_func(0x58);
}

void EXTI1_IRQHandler(void)
{
		m_enter_func(0x5C);
}
void EXTI2_IRQHandler(void)
{
		m_enter_func(0x60);
}

void EXTI3_IRQHandler(void)
{
		m_enter_func(0x64);
}
void EXTI4_IRQHandler(void)
{
		m_enter_func(0x68);
}
void DMA1_Channel1_IRQHandler(void)
{
		m_enter_func(0x6C);
}
void DMA1_Channel2_IRQHandler(void)
{
		m_enter_func(0x70);
}
void DMA1_Channel3_IRQHandler(void)
{
		m_enter_func(0x74);
}
void DMA1_Channel4_IRQHandler(void)
{
		m_enter_func(0x78);
}
void DMA1_Channel5_IRQHandler(void)
{
		m_enter_func(0x7C);
}
void DMA1_Channel6_IRQHandler(void)
{
		m_enter_func(0x80);
}
void DMA1_Channel7_IRQHandler(void)
{
		m_enter_func(0x84);
}
void ADC1_2_IRQHandler(void)
{
		m_enter_func(0x88);
}
void USB_HP_CAN1_TX_IRQHandler(void)
{
		m_enter_func(0x8C);
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
		m_enter_func(0x90);
}
void CAN1_RX1_IRQHandler(void)
{
		m_enter_func(0x94);
}
void CAN1_SCE_IRQHandler(void)
{
		m_enter_func(0x98);
}
void EXTI9_5_IRQHandler(void)
{
		m_enter_func(0x9C);
}
void TIM1_BRK_IRQHandler(void)
{
		m_enter_func(0xA0);
}
void TIM1_UP_IRQHandler(void)
{
		m_enter_func(0xA4);
}
void TIM1_TRG_COM_IRQHandler(void)
{
		m_enter_func(0xA8);
}
void TIM1_CC_IRQHandler(void)
{
		m_enter_func(0xAC);
}
void TIM2_IRQHandler(void)
{
		m_enter_func(0xB0);
}
void TIM3_IRQHandler(void)
{
		m_enter_func(0xB4);
}
void TIM4_IRQHandler(void)
{
		m_enter_func(0xB8);
}
void I2C1_EV_IRQHandler(void)
{
		m_enter_func(0xBC);
}
void I2C1_ER_IRQHandler(void)
{
		m_enter_func(0xC0);
}
void I2C2_EV_IRQHandler(void)
{
		m_enter_func(0xC4);
}
void I2C2_ER_IRQHandler(void)
{
		m_enter_func(0xC8);
}
void SPI1_IRQHandler(void)
{
		m_enter_func(0xCC);
}
void SPI2_IRQHandler(void)
{
		m_enter_func(0xD0);
}
void USART1_IRQHandler(void)
{
		m_enter_func(0xD4);
}
void USART2_IRQHandler(void)
{
		m_enter_func(0xD8);
}
void USART3_IRQHandler(void)
{
		m_enter_func(0xDC);
}
void EXTI15_10_IRQHandler(void)
{
		m_enter_func(0xE0);
}
void RTCAlarm_IRQHandler(void)
{
		m_enter_func(0xE4);
}
void USBWakeUp_IRQHandler(void)
{
		m_enter_func(0xE8);
}
void TIM8_BRK_IRQHandler(void)
{
		m_enter_func(0xEC);
}
void TIM8_UP_IRQHandler(void)
{
		m_enter_func(0xF0);
}
void TIM8_TRG_COM_IRQHandler(void)
{
		m_enter_func(0xF4);
}
void TIM8_CC_IRQHandler(void)
{
		m_enter_func(0xF8);
}
void ADC3_IRQHandler(void)
{
		m_enter_func(0xFC);
}
void FSMC_IRQHandler(void)
{
		m_enter_func(0x100);
}
void SDIO_IRQHandler(void)
{
		m_enter_func(0x104);
}
void TIM5_IRQHandler(void)
{
		m_enter_func(0x108);
}
void SPI3_IRQHandler(void)
{
		m_enter_func(0x10C);
}
void UART4_IRQHandler(void)
{
		m_enter_func(0x110);
}
void UART5_IRQHandler(void)
{
		m_enter_func(0x114);
}
void TIM6_IRQHandler(void)
{
		m_enter_func(0x118);
}
void TIM7_IRQHandler(void)
{
		m_enter_func(0x11C);
}
void DMA2_Channel1_IRQHandler(void)
{
		m_enter_func(0x120);
}
void DMA2_Channel2_IRQHandler(void)
{
		m_enter_func(0x124);
}
void DMA2_Channel3_IRQHandler(void)
{
		m_enter_func(0x128);
}
void DMA2_Channel4_5_IRQHandler(void)
{
		m_enter_func(0x12C);
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
