#include "Ap_29demo.h"
#include "7inch5_epd.h"
#include <SPI.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"
/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino Mega 2560 + 7.5inch E-Paper Display
 * Hardware Connection :Arduino Mega 2560-> 7.5inch E-Paper
 * Pin wiring definion
 * VCC---->5V
 * GND---->GND
 * RST---->D9
 * BUSY--->D10
 * D/C---->D8
 * MOSI--->D51
 * CLK---->D52
 * CS----->D53
 * Author      : Andy Li
 * Web Site    :www.seengreat.com
***************************************************************************************/
void setup() {
    Serial.begin(115200);
   Serial.print("Init gpio for arduino\r\n");
   pinMode(BUSY_Pin, INPUT); 
   pinMode(RES_Pin, OUTPUT);  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);    
   //SPI
   SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
   SPI.begin ();  
}
//Tips//
/*
1.Flickering is normal when EPD is performing a full screen update to clear ghosting from the previous image so to ensure better clarity and legibility for the new image.
2.There will be no flicker when EPD performs a partial refresh.
3.Please make sue that EPD enters sleep mode when refresh is completed and always leave the sleep mode command. Otherwise, this may result in a reduced lifespan of EPD.
4.Please refrain from inserting EPD to the FPC socket or unplugging it when the MCU is being powered to prevent potential damage.)
5.Re-initialization is required for every full screen update.
6.When porting the program, set the BUSY pin to input mode and other pins to output mode.
*/
void loop() {
      unsigned char i;
      EPD_Init(); //Full screen refresh initialization.
      EPD_WhiteScreen_White(); //Clear screen function.    
      //EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s. 

      EPD_Init(); //Full screen refresh initialization.
      EPD_Dis_PartAll(200,120,gImage_1,240,400); //To Display one image using full screen refresh.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.

      EPD_Init(); //Full screen refresh initialization.
      EPD_WhiteScreen_White(); //Clear screen function.
      delay(1000); //Delay for 2s.
      EPD_Dis_PartAll(200,120,gImage_2,240,400); //To Display one image using full screen refresh.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.
            
      EPD_Init(); //Electronic paper initialization.  
      EPD_WhiteScreen_White_Basemap();
      EPD_Init_Part(); //Pa refresh initialization.
      for(i=0;i<6;i++)
      {
         EPD_Dis_Part_Time(240,180,Num[1],Num[0],gImage_numdot,Num[0],Num[i],5,104,48); //x,y,DATA-A~E,Resolution 48*104                 
      }       
      EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.

      EPD_Init(); //Full screen refresh initialization.
      EPD_WhiteScreen_White(); //Clear screen function.
      delay(1000); //Delay for 2s.
      while(1);  // The program stops here   
}