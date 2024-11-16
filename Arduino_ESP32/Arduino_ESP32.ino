#include <SPI.h>
#include "epd_gui.h"
#include "7inch5_epd.h"
#include "Ap_29demo.h" 

/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino ESP32 + 7.5inch E-Paper Display
 * Hardware Connection :Arduino ESP32-> 7.5inch E-Paper
 *      IO27 --> CS
 *      IO18 --> CLK
 *      IO23 --> MOSI
 *      IO14  --> DC
 *      IO33  --> RST
 *      IO13  --> BUSY
 * Author      : Andy Li
 * Web Site    :www.seengreat.com
***************************************************************************************/
/******************************
 * image :
 * 0------------> x (1~800) 
 * |
 * |
 * |
 * |
 * y (1~480)
 * 
 * */
unsigned char image[48000];//Define canvas space  


void setup() {
   pinMode(BUSY_Pin, INPUT);  //BUSY
   pinMode(RES_Pin, OUTPUT); //RES 
   pinMode(DC_Pin, OUTPUT); //DC   
   pinMode(CS_Pin, OUTPUT); //CS   
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
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s. 
     /************Full display(2s)*******************/
      EPD_Init(); //Full screen refresh initialization.
      EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen refresh.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.
      delay(1000); //Delay for 2s.
            
      /************Fast refresh mode(1.5s)*******************/
      EPD_Init_Fast(); //Fast refresh initialization.
      EPD_WhiteScreen_ALL_Fast(gImage_2); //To display one image using fast refresh.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.
      delay(1000); //Delay for 2s.
  //Partial refresh demo support displaying a clock at 5 locations with 00:00.  If you need to perform partial refresh more than 5 locations, please use the feature of using partial refresh at the full screen demo.
  //After 5 partial refreshes, implement a full screen refresh to clear the ghosting caused by partial refreshes.
  //////////////////////Partial refresh time demo/////////////////////////////////////
      EPD_Init(); //Electronic paper initialization.  
      EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial refresh.
      EPD_Init_Part(); //Pa refresh initialization.
      for(i=0;i<6;i++)
      {
         EPD_Dis_Part_Time(240,180,Num[1],Num[0],gImage_numdot,Num[0],Num[i],5,104,48); //x,y,DATA-A~E,Resolution 48*104                 
      }       
      
      EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s.
      EPD_Init(); //Full screen refresh initialization.
      EPD_WhiteScreen_White(); //Clear screen function.
      EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      delay(2000); //Delay for 2s. 

      Image_Init(image, EPD_WIDTH, EPD_HEIGHT, 0, WHITE); //Set screen size and display orientation
      //EPD_Init(); //Electronic paper initialization. 
      EPD_Init_Gui();  

    /**************Drawing**********************/
    Gui_Clear(WHITE);
    //Point   
    Gui_Draw_Point(2, 2, BLACK, PIXEL_1X1, DOT_STYLE_DFT);
    Gui_Draw_Point(4, 4, BLACK, PIXEL_2X2, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 40, BLACK, PIXEL_4X4, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 55, BLACK, PIXEL_8X8, DOT_STYLE_DFT);
    //Line
    Gui_Draw_Line(1, 3,1, 480, BLACK, PIXEL_1X1, SOLID);
    Gui_Draw_Line(1, 1, 800, 1, BLACK, PIXEL_1X1, SOLID);
    //Rectangle
    Gui_Draw_Rectangle(20, 20, 100, 90, BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Rectangle(120, 35, 170, 75, BLACK, FULL, PIXEL_1X1);
    //Circle
    Gui_Draw_Circle(260, 60, 30, BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Circle(320, 60, 18, BLACK, FULL, PIXEL_1X1);
    Gui_Draw_Str(30, 120, "abcdefg", &Font12, WHITE, BLACK); //7*12
    Gui_Draw_Str(150, 118, "ABCabc012345", &Font16, WHITE, BLACK); //11*16
    Gui_Draw_Str(50, 140, "7.5\" E-Paper", &Font20, WHITE, BLACK); //14*20
    Gui_Draw_Str(50, 165, "2 Gray", &Font20, WHITE, BLACK); //14*20
    Gui_Draw_Str(50, 190, "800 x 480 Pixels", &Font20, WHITE, BLACK); //14*20
    Gui_Draw_Str(50, 215, "Black and White", &Font20, WHITE, BLACK); //14*20
    Gui_Draw_Str(57, 240, "SEENGREAT", &Font24, WHITE, BLACK); //17*24

    EPD_WhiteScreen_ALL(image);
    EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
    delay(3000); 

    EPD_Init(); //Full screen refresh initialization.
    EPD_WhiteScreen_White(); //Clear screen function.
    EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000); //Delay for 2s. 
    while(1);  // The program stops here   
}




//////////////////////////////////END//////////////////////////////////////////////////
