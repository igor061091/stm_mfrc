/**
 *	Keil project for Mifare RC522 RFID reader
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */

#include "stm32f10x.h"
/* Include my libraries here */
#include "defines.h"
//#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_mfrc522.h"
#include "stm32f10x_flash.h"
#include "main.h"
#include <stdio.h>

#define READER_COUNT 5

FLASH_Status StatusFlash;
uint32_t Flash_Buffer;
uint8_t reader=1;
uint8_t MemoryID[5][5];
uint8_t ReadMemoryID[5][5];
uint8_t CardID[5];
	/* My cards id */
	/* I read them with program below, and write this here */
	uint8_t MyID[5] = {
		0x43, 0xdc, 0x52, 0xb6, 0x7b	/* My card on my keys */
	};
	char buffer[50];
	
int main(void) {
	

	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	//TM_DELAY_Init();
	
	/* Initialize leds */
	//TM_DISCO_LedInit();
	
	/* Initialize LCD */
	//TM_ILI9341_Init();
	//TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize MFRC522 RFID */
	   FLASH_Init();
	   TM_MFRC522_Init();
	   // switch cs for every reader
     MFRC522_CS_PORT->BSRR = GPIO_Pin_1;		
     MFRC522_CS_PORT->BSRR = GPIO_Pin_2;		
     MFRC522_CS_PORT->BSRR = GPIO_Pin_3;   
	   MFRC522_CS_PORT->BSRR = GPIO_Pin_4;	  	
	/* Watermark */
	//TM_ILI9341_Puts(90, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
	
	while (1) {
		/* If any card detected */
		 //for(reader=0; reader<READER_COUNT; reader++)
		   {
					if (TM_MFRC522_Check(CardID) == MI_OK) {
						/* CardID is valid */
						 memcpy(MemoryID[reader],CardID,sizeof(CardID));
						 FLASH_WriteMemotyId();
						 FLASH_ReadMemotyId();
						// FLASH_ReadMemotyId((void *)&ReadMemoryID, (uint32_t *) FLASH_PAGE_ADDR_ID, sizeof(MemoryID));
						// (uint32_t)ReadMemoryID[1][1]=*(__IO uint32_t*)FLASH_PAGE_ADDR;  //flash_read(*(uint32_t *) FLASH_PAGE_ADDR_ID);
						/* Check if this is my card */
						if (TM_MFRC522_Compare(CardID, MyID) == MI_OK) {
							//TM_ILI9341_Puts(10, 150, "Hello tilz0R!", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);
						} else {
							//TM_ILI9341_Puts(10, 150, "Good bye man!", &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
						}
						/* Print on LCD */
						//TM_ILI9341_Puts(10, 10, "Card detected    ", &TM_Font_11x18, 0x00, 0xFFFF);
						sprintf(buffer, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
						//TM_ILI9341_Puts(10, 30, buffer, &TM_Font_11x18, 0x00, 0xFFFF);
					} else {
						/* Some printing to delete content */
						//TM_ILI9341_Puts(10, 10, "Card not detected", &TM_Font_11x18, 0x00, 0xFFFF);
						//TM_ILI9341_Puts(10, 150, "             ", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
						//TM_ILI9341_Puts(10, 30, "    \n    \n    \n    \n    ", &TM_Font_11x18, 0x00, 0xFFFF);
					}
	    }
			//reader=0;
	}
}

void CS_Pins_HIGH(void)
{	
		MFRC522_CS_PORT->BSRR = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
}

void CS_Pin_LOW(uint8_t rdr)
{
	switch(rdr)
				{
		     // switch cs for every reader
          case 1: MFRC522_CS_PORT->BSRR = GPIO_Pin_1<<16;	break;	
          case 2: MFRC522_CS_PORT->BSRR = GPIO_Pin_2<<16;	break;	
          case 3: MFRC522_CS_PORT->BSRR = GPIO_Pin_3<<16; break;
					case 4: MFRC522_CS_PORT->BSRR = GPIO_Pin_4<<16;	break;			
				}
}


void FLASH_Init(void) {
    /* Next commands may be used in SysClock initialization function
       In this case using of FLASH_Init is not obligatorily */
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency( FLASH_Latency_2);
}

//(uint32_t)ReadMemoryID[1][1]=*(__IO uint32_t*)FLASH_PAGE_ADDR;

void FLASH_ReadMemotyId(void) {
    //Read settings
	  uint16_t i;
    uint8_t *source_addr = (__IO uint8_t *)FLASH_PAGE_ADDR_ID;
    uint8_t *dest_addr = (void *)ReadMemoryID;
	  memcpy(dest_addr, source_addr, sizeof(ReadMemoryID));
    /*for ( i=0; i<5; i++) {
        ReadMemoryID[1]= *source_addr;
        source_addr++;
        dest_addr++;
    }*/
}

/*
void FLASH_ReadMemotyId(void) {
    //Read settings
	  uint16_t i;
    uint32_t *source_addr = (uint32_t *)FLASH_PAGE_ADDR;
    uint32_t *dest_addr = (void *)ReadMemoryID;
	  memcpy(dest_addr, source_addr, sizeof(ReadMemoryID));
    for ( i=0; i<sizeof(ReadMemoryID)/4; i++) {
        *dest_addr = *(__IO uint32_t*)source_addr;
        source_addr++;
        dest_addr++;
    }
}

*/

void FLASH_WriteMemotyId(void) {
	  uint16_t i;
	  uint32_t *source_addr = (void *)&MemoryID;
    uint32_t *dest_addr = (uint32_t *) FLASH_PAGE_ADDR_ID;
    FLASH_Unlock();
    FLASH_ErasePage(FLASH_PAGE_ADDR_ID);
    // Write settings
    for ( i=0; i<sizeof(MemoryID)/4; i++) {
        StatusFlash=FLASH_ProgramWord((uint32_t)dest_addr, *source_addr);
        source_addr++;
        dest_addr++;
    }
 
    FLASH_Lock();
}
/*
uint32_t flash_read(uint32_t address)
{
	return (*(__IO uint32_t*) address);
}

	
void FLASH_ReadMemotyId(uint32_t *dest_addr, uint32_t *source_addr, uint16_t size)
{
	uint16_t i;
  //uint32_t *source_addr = (uint32_t *) FLASH_PAGE_ADDR_ID;
  //uint32_t *dest_addr = (void *)&ReadMemoryID;
	for (i=0; i<size; i++)
	{
			*dest_addr = flash_read(*source_addr);    //+(i*4)
		  dest_addr++;
		  source_addr++;
	}
}
*/
