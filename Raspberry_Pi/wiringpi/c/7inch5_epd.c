#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include <fcntl.h>
#include "7inch5_epd.h"
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdint.h> 

void EPD_GPIO_Init(void)
{
	  printf("Init gpio for wiringpi\r\n");
    if(wiringPiSetup() < 0)//use wiringpi Pin number
    { 
        printf("wiringPi lib init failed! \r\n");
    } 
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_BUSY, INPUT);
    pullUpDnControl(PIN_BUSY, PUD_UP);
    wiringPiSPISetup(0,8000000); 
}
void Epaper_Write_Command(unsigned char cmd)
{
    digitalWrite(PIN_CS, 1); 
    digitalWrite(PIN_CS, 0); 
    digitalWrite(PIN_DC, 0);   // D/C#   0:command  1:data
    wiringPiSPIDataRW(0,&cmd,1); 
    digitalWrite(PIN_CS, 1); 
}

void Epaper_Write_Data(unsigned char data)
{
    digitalWrite(PIN_CS, 1); 
    digitalWrite(PIN_CS, 0); 
    digitalWrite(PIN_DC, 1);   // D/C#   0:command  1:data
    wiringPiSPIDataRW(0,&data,1); 
    digitalWrite(PIN_CS, 1); 
}

////////////////////////////////////E-paper demo//////////////////////////////////////////////////////////
//Busy function
void lcd_chkstatus(void)
{ 
  while(digitalRead(PIN_BUSY)==0)
  {  //=0 BUSY
     delay(2);
  }  
}
//Full screen refresh initialization
void EPD_Init(void)
{
	digitalWrite(PIN_RST, 0);  // Module reset    
  delay(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
  delay(10); //At least 10ms delay 

  Epaper_Write_Command(0x01);     //POWER SETTING
  Epaper_Write_Data (0x07);
  Epaper_Write_Data (0x07);    //VGH=20V,VGL=-20V
  Epaper_Write_Data (0x3f);   //VDH=15V
  Epaper_Write_Data (0x3f);   //VDL=-15V

  //Enhanced display drive(Add 0x06 command)
  Epaper_Write_Command(0x06);  //Booster Soft Start 
  Epaper_Write_Data (0x17);
  Epaper_Write_Data (0x17);   
  Epaper_Write_Data (0x28);   
  Epaper_Write_Data (0x17); 

  Epaper_Write_Command(0x04); //POWER ON
  delay(100);  
  lcd_chkstatus();        //waiting for the electronic paper IC to release the idle signal

  Epaper_Write_Command(0X00);     //PANNEL SETTING
  Epaper_Write_Data(0x1F);   //KW-3f   KWR-2F BWROTP 0f BWOTP 1f

  Epaper_Write_Command(0x61);      //tres      
  Epaper_Write_Data (0x03);    //source 800
  Epaper_Write_Data (0x20);
  Epaper_Write_Data (0x01);   //gate 480
  Epaper_Write_Data (0xE0);  

  Epaper_Write_Command(0X15);   
  Epaper_Write_Data(0x00);    

  Epaper_Write_Command(0X50);     //VCOM AND DATA INTERVAL SETTING
  Epaper_Write_Data(0x10);
  Epaper_Write_Data(0x07);

  Epaper_Write_Command(0X60);     //TCON SETTING
  Epaper_Write_Data(0x22);
  
}
//Fast refresh 1 initialization
void EPD_Init_Fast(void)
{
	digitalWrite(PIN_RST, 0);  // Module reset    
  delay(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
  delay(10); //At least 10ms delay 
  
  Epaper_Write_Command(0X00);     //PANNEL SETTING
  Epaper_Write_Data(0x1F);   //KW-3f   KWR-2F BWROTP 0f BWOTP 1f

  Epaper_Write_Command(0X50);     //VCOM AND DATA INTERVAL SETTING
  Epaper_Write_Data(0x10);
  Epaper_Write_Data(0x07);

  Epaper_Write_Command(0x04); //POWER ON
  delay(100);  
  lcd_chkstatus();        //waiting for the electronic paper IC to release the idle signal

  //Enhanced display drive(Add 0x06 command)
  Epaper_Write_Command(0x06);     //Booster Soft Start 
  Epaper_Write_Data (0x27);
  Epaper_Write_Data (0x27);   
  Epaper_Write_Data (0x18);   
  Epaper_Write_Data (0x17);   

  Epaper_Write_Command(0xE0);
  Epaper_Write_Data(0x02);
  Epaper_Write_Command(0xE5);
  Epaper_Write_Data(0x5A);
}
//Partial refresh  initialization
void EPD_Init_Part(void)
{ 
	digitalWrite(PIN_RST, 0);  // Module reset  
  delay(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
  delay(10); //At least 10ms delay 

  Epaper_Write_Command(0X00);     //PANNEL SETTING
  Epaper_Write_Data(0x1F);   //KW-3f   KWR-2F BWROTP 0f BWOTP 1f
  
  Epaper_Write_Command(0x04); //POWER ON
  delay(100);  
  lcd_chkstatus();        //waiting for the electronic paper IC to release the idle signal
  
  Epaper_Write_Command(0xE0);
  Epaper_Write_Data(0x02);
  Epaper_Write_Command(0xE5);
  Epaper_Write_Data(0x6E);
}

//////////////////////////////Display Update Function///////////////////////////////////////////////////////
//Full screen refresh update function
void EPD_Update(void)
{   
  //Refresh
  Epaper_Write_Command(0x12);   //DISPLAY REFRESH   
  delay(1);              //!!!The delay here is necessary, 200uS at least!!!     
  lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal
}

//////////////////////////////Display Data Transfer Function////////////////////////////////////////////
//Full screen refresh display function
void EPD_WhiteScreen_ALL(const unsigned char *datas)
{
   unsigned int i;  
  Epaper_Write_Command(0x10);  //write old data 
  for(i=0;i<EPD_ARRAY;i++)
   {               
     Epaper_Write_Data(0x00);
   }
  Epaper_Write_Command(0x13);  //write new data 
  for(i=0;i<EPD_ARRAY;i++)
   {               
     Epaper_Write_Data(datas[i]);
   }   
   EPD_Update();   
}
//Fast refresh display function
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas)
{
  unsigned int i; 
  Epaper_Write_Command(0x10);  //write old data 
  for(i=0;i<EPD_ARRAY;i++)
   {               
     Epaper_Write_Data(0x00);
   }
  Epaper_Write_Command(0x13);  //write new data 
  for(i=0;i<EPD_ARRAY;i++)
   {               
     Epaper_Write_Data(datas[i]);
   }   
   EPD_Update();     
}

//Clear screen display
void EPD_WhiteScreen_White(void)
{
  unsigned int i;
  //Write Data
  Epaper_Write_Command(0x10);      
  for(i=0;i<EPD_ARRAY;i++)       
  {
    Epaper_Write_Data(0x00);  
  }
  Epaper_Write_Command(0x13);      
  for(i=0;i<EPD_ARRAY;i++)       
  {
    Epaper_Write_Data(0x00);  
  }
   EPD_Update();   
}
//Clear screen display
void EPD_WhiteScreen_White_Basemap(void)
{
  unsigned int i;
  //Write Data
  Epaper_Write_Command(0x10);      
  for(i=0;i<EPD_ARRAY;i++)       
  {
    Epaper_Write_Data(0xFF);  //is  different
  }
  Epaper_Write_Command(0x13);      
  for(i=0;i<EPD_ARRAY;i++)       
  {
    Epaper_Write_Data(0x00);  
  }
   EPD_Update();   
}
//Display all black
void EPD_WhiteScreen_Black(void)
{
  unsigned int i;
  //Write Data
  Epaper_Write_Command(0x10);      
  for(i=0;i<EPD_ARRAY;i++)       
  {
    Epaper_Write_Data(0x00);  
  }
  Epaper_Write_Command(0x13);      
  for(i=0;i<EPD_ARRAY;i++)       
  {
    Epaper_Write_Data(0xff);  
  }
   EPD_Update();  
}
//Partial refresh of background display, this function is necessary, please do not delete it!!!
void EPD_SetRAMValue_BaseMap( const unsigned char * datas)
{
  unsigned int i; 
  Epaper_Write_Command(0x10);  //write old data 
  for(i=0;i<EPD_ARRAY;i++)
   {               
     Epaper_Write_Data(0xFF); //is  different
   }
  Epaper_Write_Command(0x13);  //write new data 
  for(i=0;i<EPD_ARRAY;i++)
   {               
     Epaper_Write_Data(datas[i]);
   }   
   EPD_Update();     
   
}
//Partial refresh display
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
  unsigned int i;  
  unsigned int x_end,y_end;

  x_end=x_start+PART_LINE-1; 
  y_end=y_start+PART_COLUMN-1;

  Epaper_Write_Command(0x50);
  Epaper_Write_Data(0xA9);
  Epaper_Write_Data(0x07);

  Epaper_Write_Command(0x91);   //This command makes the display enter partial mode
  Epaper_Write_Command(0x90);   //resolution setting
  Epaper_Write_Data (x_start/256);
  Epaper_Write_Data (x_start%256);   //x-start    

  Epaper_Write_Data (x_end/256);    
  Epaper_Write_Data (x_end%256-1);  //x-end 

  Epaper_Write_Data (y_start/256);  //
  Epaper_Write_Data (y_start%256);   //y-start    

  Epaper_Write_Data (y_end/256);    
  Epaper_Write_Data (y_end%256-1);  //y-end
  Epaper_Write_Data (0x01); 

  Epaper_Write_Command(0x13);        //writes New data to SRAM.
  for(i=0;i<PART_COLUMN*PART_LINE/8;i++)       
   {
     Epaper_Write_Data(datas[i]);  
   }  
    EPD_Update(); 

}
//Full screen partial refresh display
void EPD_Dis_PartAll(const unsigned char * datas)
{
  unsigned int i;  
  unsigned int x_start=0,y_start=0,x_end,y_end;
  unsigned int PART_COLUMN=EPD_HEIGHT,PART_LINE=EPD_WIDTH;

  x_end=x_start+PART_LINE-1; 
  y_end=y_start+PART_COLUMN-1;

  Epaper_Write_Command(0x50);
  Epaper_Write_Data(0xA9);
  Epaper_Write_Data(0x07);

  Epaper_Write_Command(0x91);   //This command makes the display enter partial mode
  Epaper_Write_Command(0x90);   //resolution setting
  Epaper_Write_Data (x_start/256);
  Epaper_Write_Data (x_start%256);   //x-start    

  Epaper_Write_Data (x_end/256);    
  Epaper_Write_Data (x_end%256-1);  //x-end 

  Epaper_Write_Data (y_start/256);  //
  Epaper_Write_Data (y_start%256);   //y-start    

  Epaper_Write_Data (y_end/256);    
  Epaper_Write_Data (y_end%256-1);  //y-end
  Epaper_Write_Data (0x01); 

  Epaper_Write_Command(0x13);        //writes New data to SRAM.
  for(i=0;i<PART_COLUMN*PART_LINE/8;i++)       
   {
     Epaper_Write_Data(datas[i]);   
   }  
    EPD_Update(); 

}
//Deep sleep function
void EPD_DeepSleep(void)
{   
    Epaper_Write_Command(0X50);  //VCOM AND DATA INTERVAL SETTING     
    Epaper_Write_Data(0xf7); //WBmode:VBDF 17|D7 VBDW 97 VBDB 57    WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7  

    Epaper_Write_Command(0X02);   //power off
    lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal    
    Epaper_Write_Command(0X07);   //deep sleep
    Epaper_Write_Data(0xA5);
}

//Partial refresh write address and data
void EPD_Dis_Part_RAM(unsigned int x_start,unsigned int y_start,
                        const unsigned char * datas_A,const unsigned char * datas_B,
                        const unsigned char * datas_C,const unsigned char * datas_D,const unsigned char * datas_E,
                        unsigned char num,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
  unsigned int i,j;  
  unsigned int x_end,y_end;

  x_end=x_start+PART_LINE*num-1; 
  y_end=y_start+PART_COLUMN-1;

  Epaper_Write_Command(0x50);
  Epaper_Write_Data(0xA9);
  Epaper_Write_Data(0x07);

  Epaper_Write_Command(0x91);   //This command makes the display enter partial mode
  Epaper_Write_Command(0x90);   //resolution setting
  Epaper_Write_Data (x_start/256);
  Epaper_Write_Data (x_start%256);   //x-start    

  Epaper_Write_Data (x_end/256);    
  Epaper_Write_Data (x_end%256-1);  //x-end 

  Epaper_Write_Data (y_start/256);  //
  Epaper_Write_Data (y_start%256);   //y-start    

  Epaper_Write_Data (y_end/256);    
  Epaper_Write_Data (y_end%256-1);  //y-end
  Epaper_Write_Data (0x01); 

  Epaper_Write_Command(0x13);        //writes New data to SRAM.
  for(i=0;i<PART_COLUMN;i++)       
   {
      for(j=0;j<PART_LINE/8;j++)       
        Epaper_Write_Data(datas_A[i*PART_LINE/8+j]);    
      for(j=0;j<PART_LINE/8;j++)       
        Epaper_Write_Data(datas_B[i*PART_LINE/8+j]);  
      for(j=0;j<PART_LINE/8;j++)       
        Epaper_Write_Data(datas_C[i*PART_LINE/8+j]);    
      for(j=0;j<PART_LINE/8;j++)       
        Epaper_Write_Data(datas_D[i*PART_LINE/8+j]);  
      for(j=0;j<PART_LINE/8;j++)       
        Epaper_Write_Data(datas_E[i*PART_LINE/8+j]);       
   }  
   
    
}
//Clock display
void EPD_Dis_Part_Time(unsigned int x_start,unsigned int y_start,
                        const unsigned char * datas_A,const unsigned char * datas_B,
                        const unsigned char * datas_C,const unsigned char * datas_D,const unsigned char * datas_E,
                        unsigned char num,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
  EPD_Dis_Part_RAM(x_start,y_start,datas_A,datas_B,datas_C,datas_D,datas_E,num,PART_COLUMN,PART_LINE);
  EPD_Update();
  Epaper_Write_Command(0X92);   //This command makes the display exit partial mode and enter normal mode. 
 
}                        


////////////////////////////////Other newly added functions////////////////////////////////////////////
//Display rotation 180 degrees initialization
void EPD_Init_Gui(void)
{
  digitalWrite(PIN_RST,0);   // Module reset   
  delay(10);//At least 10ms delay 
  digitalWrite(PIN_RST,1); 
  delay(10); //At least 10ms delay 

  Epaper_Write_Command(0x01);     //POWER SETTING
  Epaper_Write_Data (0x07);
  Epaper_Write_Data (0x07);    //VGH=20V,VGL=-20V
  Epaper_Write_Data (0x3f);   //VDH=15V
  Epaper_Write_Data (0x3f);   //VDL=-15V

  //Enhanced display drive(Add 0x06 command)
  Epaper_Write_Command(0x06);     //Booster Soft Start 
  Epaper_Write_Data (0x17);
  Epaper_Write_Data (0x17);   
  Epaper_Write_Data (0x28);   
  Epaper_Write_Data (0x17); 

  Epaper_Write_Command(0x04); //POWER ON
  delay(100);  
  lcd_chkstatus();        //waiting for the electronic paper IC to release the idle signal

  Epaper_Write_Command(0X00);     //PANNEL SETTING
  Epaper_Write_Data(0x1B);   //0X13 KW-3f   KWR-2F BWROTP 0f BWOTP 1f

  Epaper_Write_Command(0x61);         //tres      
  Epaper_Write_Data (0x03);   //source 800
  Epaper_Write_Data (0x20);
  Epaper_Write_Data (0x01);   //gate 480
  Epaper_Write_Data (0xE0);  

  Epaper_Write_Command(0X15);   
  Epaper_Write_Data(0x00);    

  Epaper_Write_Command(0X50);     //VCOM AND DATA INTERVAL SETTING
  Epaper_Write_Data(0x10);
  Epaper_Write_Data(0x07);

  Epaper_Write_Command(0X60);     //TCON SETTING
  Epaper_Write_Data(0x22);
}

/***********************************************************
            end file
***********************************************************/
