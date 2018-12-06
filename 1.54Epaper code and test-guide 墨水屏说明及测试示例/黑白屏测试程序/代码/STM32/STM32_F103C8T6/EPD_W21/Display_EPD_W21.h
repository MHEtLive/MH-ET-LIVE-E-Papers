
#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_



extern void driver_delay_xms(unsigned long xms);


void EPD_Dis_Part(unsigned char xStart,unsigned char xEnd,unsigned long yStart,unsigned long yEnd,unsigned char *DisBuffer,unsigned char Label);
void EPD_Dis_Full(unsigned char *DisBuffer,unsigned char Label);
void EPD_init_Full(void);
void EPD_init_Part(void);

#endif
/***********************************************************
						end file
***********************************************************/


