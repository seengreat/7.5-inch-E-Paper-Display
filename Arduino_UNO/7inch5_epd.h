#ifndef _7INCH5_EPD_H_
#define _7INCH5_EPD_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"

#define EPD_WIDTH   800 
#define EPD_HEIGHT  480
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  
//  *      D7 --> CS
//  *      D13 --> CLK
//  *      D11 --> MOSI
//  *      D10  --> DC
//  *      D9  --> RST
//  *      D8  --> BUSY
//IO settings
#define BUSY_Pin  8 
#define RES_Pin   9 
#define DC_Pin    10 

#define CS_Pin    7 


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
void EPD_WhiteScreen_White(void);
void EPD_WhiteScreen_Black(void);
void EPD_DeepSleep(void);
//Partial refresh display 
void EPD_Init_Part(void);
void EPD_WhiteScreen_White_Basemap(void);
void EPD_Dis_PartAll(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE);
void EPD_Dis_Part_Time(unsigned int x_start,unsigned int y_start,
                        const unsigned char * datas_A,const unsigned char * datas_B,
                        const unsigned char * datas_C,const unsigned char * datas_D,const unsigned char * datas_E,
                        unsigned char num,unsigned int PART_COLUMN,unsigned int PART_LINE);                    
                         
#endif
