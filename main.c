/**
  ******************************************************************************
  * @file    FLASH_Program/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include <stdio.h>
__IO uint32_t TimingDelay;


/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup FLASH_Program
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_2  /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11  /* End @ of user Flash area */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define DATA_32                 ((uint32_t)0x12345678)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t StartSector = 0, EndSector = 0, Address = 0, i = 0, a = 2;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0 ;
uint32_t *x,*y;  
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t GetSector(uint32_t Address);

void delay(int ledNo){
  Led_TypeDef Led;
  switch(ledNo){
      case 0: Led = LED3; break;
      case 1: Led = LED4; break;
      case 2: Led = LED6; break;
      case 3: Led = LED5; break;
    }
  uint64_t n = 1250000;  
  while(n != 0){    
    switch(n % 4){
      case 0: STM_EVAL_LEDOn(Led); break;
      case 1: STM_EVAL_LEDOn(Led); break;
      case 2: STM_EVAL_LEDOn(Led); break;
      case 3: STM_EVAL_LEDOn(Led); break;
    }
    n--;
  }
  STM_EVAL_LEDOff(Led);
}

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* printf init */
  USART3_Config();
  /* LED init */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  
  /* Unlock the Flash to enable the flash control register access *************/ 
  FLASH_Unlock();
  STM_EVAL_LEDOn(LED4);
  printf("\r\n FLASH Unlock \r\n");  
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

  /* Get the number of the start and end sectors */
  StartSector = GetSector(FLASH_USER_START_ADDR);
  EndSector = GetSector(FLASH_USER_END_ADDR);

  for (i = StartSector; i <= EndSector; i += 8){
    STM_EVAL_LEDOn(LED5);
    printf("\r\n Erase sector:%d \r\n",a);
    a++;
    //if (a>11){a=2;}  
    
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
    { 
      /* Error occurred while sector erase. 
         User can add here some code to deal with this error  */
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOn(LED4);
      printf("\r\n Erase error!! \r\n");  

      while (1)
      {
      }
    }
    STM_EVAL_LEDOff(LED5);
  }
    printf("\r\n Erase complete \r\n");  

  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR;

  printf("\r\n FLASH is writing... \r\n");  
  while (Address < FLASH_USER_END_ADDR){
    STM_EVAL_LEDOn(LED6);
    if (FLASH_ProgramWord(Address, DATA_32) == FLASH_COMPLETE)
    {
      Address = Address + 4;
    }
    else
    { 
      /* Error occurred while writing data in Flash memory. 
         User can add here some code to deal with this error */
      while (1)
      {
        STM_EVAL_LEDOn(LED3);
        STM_EVAL_LEDOn(LED4);
        printf("\r\n Write error!! \r\n");  
      }
    }
    STM_EVAL_LEDOff(LED6);
  }
    printf("\r\n Write complete \r\n");
   //x=Address-4;
   //y=Address-8;

    //printf("\r\n %d\r\n", *x);  
    //printf("\r\n %d\r\n", *y);  

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  FLASH_Lock(); 
  STM_EVAL_LEDOff(LED4);
  printf("\r\n FLASH lock \r\n");  
 

  /* Check if the programmed data is OK 
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;
    printf("\r\n FLASH is reading... \r\n");  
    while (Address < FLASH_USER_END_ADDR){
    STM_EVAL_LEDOn(LED3);
    data32 = *(__IO uint32_t*)Address;    

    if (data32 != DATA_32){
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOn(LED4);
      MemoryProgramStatus++;     
      printf("\r\n Read error!! \r\n");  
    }

    Address = FLASH_USER_END_ADDR;
    STM_EVAL_LEDOff(LED3);
  }  
    printf("\r\n Read complete \r\n");

  /*print Data_32*/
   printf("\r\n print Data_32 \r\n");
   x=Address-4;
   y=Address-8;
   printf("\r\n FLASH_USER_END_ADDR  :%d\r\n", *x);  
   printf("\r\n FLASH_USER_END_ADDR-4:%d\r\n", *y);  
  
  printf("\r\n Waiting... \r\n");  
  int n = 0;
  while (1) {
    delay(n % 4);
    n++;
  }
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}

#ifdef  USE_FULL_ASSERT

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

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
