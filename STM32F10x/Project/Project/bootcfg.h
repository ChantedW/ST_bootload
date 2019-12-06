#ifndef __BOOT_CFG_
#define __BOOT_CFG_

/**************************include******************************/

#include "iap_flag.h"

/***************************************************************/
#define STM32F107VC

//------------------ <<< Use Configuration Wizard in Context Menu >>> ------------------------------

//====================================== Clock Configuration =======================================
// <e> 01�cʱ������
//	 <o> �ⲿ����Ƶ��
//     <i> �ڲ�����̶�Ϊ8MHz
//	   <7372800=> 7.3728MHz
//	   <8000000=> 8.0000MHz
//     <25000000=> 25.0000MHz
#define __CLOCK_SETUP             1								// 0
#define HSE_VALUE             	  25000000UL			  // 1


// </e> End of Clock Configuration

//====================================== flash Configuration =======================================

// <e> 02�cFlash ��С
//   <o> оƬflash��С 
//	   <0=>  STM32F10X_HD 
//     <1=>  STM32F10X_MD 
//     <2=>  STM32F10X_CL 

#define __FLASH_SETUP             1               // 0
#define __FLASH_CAPACITY          2               // 1

#if __FLASH_SETUP
	#if   __FLASH_CAPACITY ==0 
			 #define  STM32F10X_HD 
	#elif __FLASH_CAPACITY ==1 
			 #define	STM32F10X_MD 
	#elif __FLASH_CAPACITY ==2 
			 #define	STM32F10X_CL 
	#endif
#endif

// </e> End of flash Configuration

//====================================== IAP Signal Configuration =======================================

//<e0> 03�cIAP Flag ����           
//<e1> GPIO ����
//	   <o2> �˿�
//			<0=> GPIOA
//			<1=> GPIOB
//			<2=> GPIOC
//			<3=> GPIOD
//			<4=> GPIOE
//	   <o3> ����
//			<0=> Pin0
//			<1=> Pin1
//			<2=> Pin2
//			<3=> Pin3
//			<4=> Pin4
//			<5=> Pin5
//			<6=> Pin6
//			<7=> Pin7
//			<8=> Pin8
//			<9=> Pin9
//			<10=> Pin10
//			<11=> Pin11
//			<12=> Pin12
//			<13=> Pin13
//			<14=> Pin14
//			<15=> Pin15
//</e>
//<e4> �洢����
// 			<e5> AT24C04
//	      <O6> �����ַ
//        <O7> ���ݵ�ַ
//      </e>
//			<e8> Flash
//        <O9> ���ݵ�ַ
//</e>

#define IAP_SET             			1    // 1 ���ÿ���
#define IAP_PORT_SET_ENALBE       1    // 3 �˿�ʹ��      		  
#define IAP_FLAG_PORT         		0    // 4 �˿ں�ѡ��
#define IAP_FLAG_PIN         	   	7    // 5 �˿�����
#define IAP_MEMORY_ENALBE         0    // 6 �洢ʹ��
#define IAP_MEMORY_EEPROM         1    // 7 ѡ��EEPrROM
#define AT_ADDR                   0xA0 // 8 �����ַ
#define AT_DATA_ADDR              0    // 8 ���ݵ�ַ
#define IAP_MEMORY_FLASH          0    // 9 ѡ��flash
#define FLASH_DATA_ADDR           0x80013F5 // 10 ���ݵ�ַ

#if IAP_SET

	#if IAP_PORT_SET_ENALBE           // ʹ��GPIO��
		
		#if IAP_FLAG_PORT == 0
        #define FLAG_PORT   GPIOA
		#elif IAP_FLAG_PORT == 1
				#define FLAG_PORT   GPIOB
		#elif IAP_FLAG_PORT == 2
				#define FLAG_PORT   GPIOC
		#elif IAP_FLAG_PORT == 3
				#define FLAG_PORT   GPIOD
		#elif IAP_FLAG_PORT == 4
				#define FLAG_PORT   GPIOE
		#endif
		
		#define FLAG_PIN   ((uint16_t)1 << IAP_FLAG_PIN)
		
	#endif
	
	#if IAP_MEMORY_ENALBE            // ʹ�ô洢��Ϊ��ʶ
		
		#if IAP_MEMORY_EEPROM &&  (!IAP_MEMORY_FLASH) 
		    
			#define STORE_PHYSIC_ADDR   	AT_ADDR
			
			#define STORE_DATA_ADDR       AT_DATA_ADDR
			
			#define EPPROM_WR   STORE_PHYSIC_ADDR
			#define EPPROM_RD   (STORE_PHYSIC_ADDR|0x01)
			
		#endif
		
		#if IAP_MEMORY_FLASH && (!IAP_MEMORY_EEPROM)
		    
			#define STORE_DATA_ADDR      FLASH_DATA_ADDR
			
		#endif
		
	#endif
		
#endif

//</e>

// </e> End of IAP Signal Configuration

// <<< End of configuration section >>>


/**************************function declare******************************/



#endif

