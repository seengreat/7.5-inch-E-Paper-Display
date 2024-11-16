#ifndef _7INCH5_EPD_H_
#define _7INCH5_EPD_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define EPD_WIDTH   800 
#define EPD_HEIGHT  480
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  


 /* Pin wiring definion
 *      3.3V --> VCC  
 *      GND  --> GND
 *      D8   --> CS
 *      SCLK --> CLK
 *      MOSI --> MOSI
 *      D25  --> DC
 *      D17  --> RST
 *      D24  --> BUSY
 * */
//IO settings
#define PIN_CS      8 
#define PIN_RST     17 
#define PIN_DC      25
#define PIN_BUSY    24

void EPD_GPIO_Init(void);
void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);

//Full screen refresh display
void EPD_Init(void); 
void EPD_Init_Gui(void);  
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_WhiteScreen_White(void);
void EPD_WhiteScreen_Black(void);
void EPD_DeepSleep(void);
//Partial refresh display 
void EPD_Init_Part(void);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_WhiteScreen_White_Basemap(void);
void EPD_Dis_PartAll(const unsigned char * datas);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE);
void EPD_Dis_Part_Time(unsigned int x_start,unsigned int y_start,
                        const unsigned char * datas_A,const unsigned char * datas_B,
                        const unsigned char * datas_C,const unsigned char * datas_D,const unsigned char * datas_E,
                        unsigned char num,unsigned int PART_COLUMN,unsigned int PART_LINE);                    
//Fast refresh display
void EPD_Init_Fast(void);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);   

                         
#endif
