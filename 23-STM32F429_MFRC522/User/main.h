#ifndef MAIN_H
#define MAIN_H


#define FLASH_PAGE_ADDR_ID 0x800A000
#define FLASH_PAGE_ADDR 0x800A005


typedef enum Bool {
    FALSE=0,
    TRUE=1
} Bool;

void CS_Pin_LOW(uint8_t rdr);
void CS_Pins_HIGH(void);
void FLASH_Init(void);	
void FLASH_ReadMemotyId(void);
void FLASH_WriteMemotyId(void);
//void FLASH_ReadMemotyId(uint32_t *dest_addr, uint32_t *source_addr, uint16_t size);
uint32_t flash_read(uint32_t address);


#endif