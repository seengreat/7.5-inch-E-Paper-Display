#ifndef _7INCH5_EPD_H_
#define _7INCH5_EPD_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"

#define EPD_WIDTH   800 
#define EPD_HEIGHT  480
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  


 /* Pin wiring definion
 *      3.3V --> VCC  
 *      GND  --> GND
 *      IO27 --> CS
 *      IO18 --> CLK
 *      IO23 --> MOSI
 *      IO14  --> DC
 *      IO33  --> RST
 *      IO13  --> BUSY
 * */
//IO settings
#define TEST_Pin  7 
#define BUSY_Pin  13 
#define RES_Pin   33 
#define DC_Pin    14 

#define CS_Pin    27 
#define SCK_Pin   18 
#define SDI_Pin   23


#define TEST_0  digitalWrite(TEST_Pin,LOW)
#define TEST_1  digitalWrite(TEST_Pin,HIGH) 

#define EPD_W21_MOSI_0  digitalWrite(SDI_Pin,LOW)
#define EPD_W21_MOSI_1  digitalWrite(SDI_Pin,HIGH) 

#define EPD_W21_CLK_0  digitalWrite(SCK_Pin,LOW)
#define EPD_W21_CLK_1  digitalWrite(SCK_Pin,HIGH)


#define EPD_W21_CS_0   digitalWrite(CS_Pin,LOW)
#define EPD_W21_CS_1   digitalWrite(CS_Pin,HIGH)

#define EPD_W21_DC_0   digitalWrite(DC_Pin,LOW)
#define EPD_W21_DC_1   digitalWrite(DC_Pin,HIGH)
#define EPD_W21_RST_0  digitalWrite(RES_Pin,LOW)
#define EPD_W21_RST_1  digitalWrite(RES_Pin,HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin)

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);

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
