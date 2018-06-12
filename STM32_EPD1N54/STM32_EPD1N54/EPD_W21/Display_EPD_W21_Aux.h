
#ifndef _DISPLAY_EPD_W21_AUX_H_
#define _DISPLAY_EPD_W21_AUX_H_
#include "stm32f10x.h"


#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOB, GPIO_Pin_9)

#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define EPD_W21_CLK_1	GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define EPD_W21_CS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define EPD_W21_CS_1	GPIO_SetBits(GPIOB, GPIO_Pin_7)

#define EPD_W21_DC_0	GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define EPD_W21_DC_1	GPIO_SetBits(GPIOB, GPIO_Pin_6)

#define EPD_W21_RST_0	GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define EPD_W21_RST_1	GPIO_SetBits(GPIOB, GPIO_Pin_5)

#define EPD_W21_BS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4) 


void SPI_Write(unsigned char value);
void driver_delay_xms(unsigned long xms);
#endif
/***********************************************************
						end file
***********************************************************/


