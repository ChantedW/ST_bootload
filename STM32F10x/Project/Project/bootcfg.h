#ifndef __BOOT_CFG_
#define __BOOT_CFG_

/**************************include******************************/

#include "iap_flag.h"

/***************************************************************/
#define STM32F107VC

//------------------ <<< Use Configuration Wizard in Context Menu >>> ------------------------------

//====================================== Clock Configuration =======================================
// <e> 01时钟配置
//	 <o> 外部晶振频率
//     <i> 内部晶振固定为8MHz
//	   <7372800=> 7.3728MHz
//	   <8000000=> 8.0000MHz
//     <25000000=> 25.0000MHz
#define __CLOCK_SETUP             1								// 0
#define HSE_VALUE             	  25000000UL			  // 1


// </e> End of Clock Configuration

//====================================== flash Configuration =======================================

// <e> 02Flash 大小
//   <o> 芯片flash大小 
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

//<e0> 03IAP Flag 设置           
//<e1> GPIO 配置
//	   <o2> 端口
//			<0=> GPIOA
//			<1=> GPIOB
//			<2=> GPIOC
//			<3=> GPIOD
//			<4=> GPIOE
//	   <o3> 引脚
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
//<e4> 存储配置
// 			<e5> AT24C04
//	      <O6> 物理地址
//        <O7> 数据地址
//      </e>
//			<e8> Flash
//        <O9> 数据地址
//</e>

#define IAP_SET             			1    // 1 配置开启
#define IAP_PORT_SET_ENALBE       1    // 3 端口使能      		  
#define IAP_FLAG_PORT         		0    // 4 端口号选择
#define IAP_FLAG_PIN         	   	7    // 5 端口引脚
#define IAP_MEMORY_ENALBE         0    // 6 存储使能
#define IAP_MEMORY_EEPROM         1    // 7 选择EEPrROM
#define AT_ADDR                   0xA0 // 8 物理地址
#define AT_DATA_ADDR              0    // 8 数据地址
#define IAP_MEMORY_FLASH          0    // 9 选择flash
#define FLASH_DATA_ADDR           0x80013F5 // 10 数据地址

#if IAP_SET

	#if IAP_PORT_SET_ENALBE           // 使用GPIO口
		
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
	
	#if IAP_MEMORY_ENALBE            // 使用存储作为标识
		
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

