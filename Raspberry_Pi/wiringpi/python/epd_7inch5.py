# Seengreat 7.5 Inch E-Paper Display demo
# Author(s):Andy Li from Seengreat
import os
import sys
import spidev
import time
import wiringpi as wpi
import numpy as np
"""the following pin definiting use wiringPi number"""
CS_PIN     = 10
DC_PIN     = 6
BUSY_PIN   = 5
RST_PIN    = 0
EPD_WIDTH  = 800
EPD_HEIGHT = 480
EPD_ARRAY  = EPD_WIDTH*EPD_HEIGHT//8  

class EPD_7Inch5():
    def __init__(self):
        # gpio init
        wpi.wiringPiSetup()
        wpi.pinMode(DC_PIN, wpi.OUTPUT)  # D/C pin
        wpi.pinMode(RST_PIN, wpi.OUTPUT)  # reset pin
        #wpi.pinMode(CS_PIN, wpi.OUTPUT)  # CS pin
        wpi.pinMode(BUSY_PIN, wpi.INPUT)  # BUSY pin
        wpi.pullUpDnControl(BUSY_PIN, wpi.PUD_UP)
        # spi init
        self.bus = 0
        self.dev = 0
        self.spi_speed = 8000000
        self.spi = spidev.SpiDev()
        self.spi.open(self.bus, self.dev)
        self.spi.max_speed_hz = self.spi_speed
        self.spi.mode = 0b00
        
        self.w = EPD_WIDTH
        self.h = EPD_HEIGHT
        
    def write_cmd(self, cmd):
        """write command"""
        wpi.digitalWrite(DC_PIN,wpi.LOW)
        self.spi.writebytes([cmd])
        
    def write_data(self, value):
        """write data"""
        wpi.digitalWrite(DC_PIN,wpi.HIGH)
        self.spi.writebytes([value])
        
    def chkstatus(self):
        while wpi.digitalRead(BUSY_PIN)==wpi.LOW: 
            time.sleep(0.002)
        

    def reset(self):
        """reset the epd"""
        wpi.digitalWrite(RST_PIN,wpi.LOW)
        time.sleep(0.1)
        wpi.digitalWrite(RST_PIN,wpi.HIGH)
        time.sleep(0.1)
        
    #Full screen refresh initialization
    def init(self):
        self.reset()
        self.write_cmd(0x01) #POWER SETTING
        self.write_data(0x07)
        self.write_data(0x07) #VGH=20V,VGL=-20V
        self.write_data(0x3f) #VDH=15V
        self.write_data(0x3f) #VDL=-15V
        
        #Enhanced display drive(Add 0x06 command)
        self.write_cmd(0x06)   #Booster Soft Start 
        self.write_data(0x17)
        self.write_data(0x17)
        self.write_data(0x28)
        self.write_data(0x17)

        self.write_cmd(0x04)  #POWER ON
        time.sleep(0.1)   
        self.chkstatus()#waiting for the electronic paper IC to release the idle signal
      
        self.write_cmd(0X00)      #PANNEL SETTING
        self.write_data(0x1F)    #KW-3f   KWR-2F BWROTP 0f BWOTP 1f

        self.write_cmd(0x61)       #tres      
        self.write_data(0x03)     #source 800
        self.write_data(0x20) 
        self.write_data(0x01)    #gate 480
        self.write_data(0xE0)   

        self.write_cmd(0X15)    
        self.write_data(0x00)     
        
        self.write_cmd(0X50)      #VCOM AND DATA INTERVAL SETTING
        self.write_data(0x10)
        self.write_data(0x07) 

        self.write_cmd(0X60)      #TCON SETTING
        self.write_data(0x22) 
    #Fast refresh 1 initialization
    def init_fast(self):
        self.reset()
        self.write_cmd(0X00) #PANNEL SETTING
        self.write_data(0x1F) #KW-3f   KWR-2F BWROTP 0f BWOTP 1f

        self.write_cmd(0X50) #VCOM AND DATA INTERVAL SETTING
        self.write_data(0x10)
        self.write_data(0x07)

        self.write_cmd(0x04)  #POWER ON
        time.sleep(0.1)
        self.chkstatus() #waiting for the electronic paper IC to release the idle signal
        
        #Enhanced display drive(Add 0x06 command)
        self.write_cmd(0x06) #Booster Soft Start 
        self.write_data(0x27)
        self.write_data(0x27) 
        self.write_data(0x18) 
        self.write_data(0x17)  

        self.write_cmd(0xE0) 
        self.write_data(0x02) 
        self.write_cmd(0xE5) 
        self.write_data(0x5A) 

    #Partial refresh  initialization
    def init_part(self):
        self.reset()
        self.write_cmd(0X00)  #PANNEL SETTING
        self.write_data(0x1F) #KW-3f   KWR-2F BWROTP 0f BWOTP 1f

        self.write_cmd(0x04)  #POWER ON
        time.sleep(0.1)   
        self.chkstatus()   #waiting for the electronic paper IC to release the idle signal
        
        self.write_cmd(0xE0) 
        self.write_data(0x02) 
        self.write_cmd(0xE5) 
        self.write_data(0x6E) 

        ###############Display Update Function###########################
        #Full screen refresh update function
    def update(self):
        #Refresh
        self.write_cmd(0x12) #DISPLAY REFRESH   
        time.sleep(0.01)            #!!!The delay here is necessary, 200uS at least!!!     
        self.chkstatus()    #waiting for the electronic paper IC to release the idle signal

    ###############Display Data Transfer Function######################
    #Full screen refresh display function
    def whitescreen_all(self,datas):
        self.write_cmd(0x10)   #write old data
        for i in range( EPD_ARRAY):             
            self.write_data(0x00) 
        self.write_cmd(0x13)   #write new data
        for i in range( EPD_ARRAY):
            self.write_data(datas[i]) 
        self.update()
        
    #Fast refresh display function
    def whitescreen_all_fast(self, datas):
        self.write_cmd(0x10)   #write old data 
        for i in range( EPD_ARRAY):
            self.write_data(0x00) 
        self.write_cmd(0x13)   #write new data
        for i in range( EPD_ARRAY):              
            self.write_data(datas[i]) 
        self.update()      

    #Clear screen display
    def whitescreen_white(self):
        #Write Data
        self.write_cmd(0x10)       
        for i in range( EPD_ARRAY):       
            self.write_data(0x00)   
        self.write_cmd(0x13)       
        for i in range( EPD_ARRAY):      
            self.write_data(0x00)   
        self.update()  
    #Clear screen display
    def whitescreen_whitebasemap(self):
        #Write Data
        self.write_cmd(0x10)       
        for i in range( EPD_ARRAY):       
            self.write_data(0xFF)   #is  different
        self.write_cmd(0x13)       
        for i in range( EPD_ARRAY):       
            self.write_data(0x00)   
        self.update()    
    #Display all black
    def whiteScreen_black(self):
        #Write Data
        self.write_cmd(0x10)       
        for i in range( EPD_ARRAY):       
            self.write_data(0x00)   
        self.write_cmd(0x13)       
        for i in range( EPD_ARRAY):       
            self.write_data(0xff)   
        self.update()   
    #Partial refresh of background display, this function is necessary, please do not delete it!!!
    def set_Ramvalue_basemap(self, datas):
        self.write_cmd(0x10)   #write old data 
        for i in range( EPD_ARRAY):
            self.write_data(0xFF)  #is  different
        self.write_cmd(0x13)   #write new data 
        for i in range( EPD_ARRAY):
            self.write_data(datas[i]) 
        self.update()      

    #Partial refresh display
    def EPD_Dis_Part(self,x_start,y_start,datas,part_column,part_line):
        x_end=x_start+part_line-1  
        y_end=y_start+part_column-1 

        self.write_cmd(0x50) 
        self.write_data(0xA9) 
        self.write_data(0x07) 

        self.write_cmd(0x91)    #This command makes the display enter partial mode
        self.write_cmd(0x90)    #resolution setting
        self.write_data(x_start//256) 
        self.write_data(x_start%256)    #x-start    

        self.write_data(x_end//256)     
        self.write_data(x_end%256-1)   #x-end 

        self.write_data(y_start//256)   #
        self.write_data(y_start%256)    #y-start    

        self.write_data(y_end//256)     
        self.write_data(y_end%256-1)   #y-end
        self.write_data(0x01)  

        self.write_cmd(0x13)         #writes New data to SRAM.
        for i in range(part_column*part_line//8):       
            self.write_data(datas[i])   
        self.update()
    
    #Full screen partial refresh display
    def dis_partall(self, datas):
        part_column=EPD_HEIGHT
        part_line=EPD_WIDTH
        x_end=x_start+part_line-1  
        y_end=y_start+part_column-1 
        self.write_cmd(0x50) 
        self.write_data(0xA9) 
        self.write_data(0x07) 

        self.write_cmd(0x91)    #This command makes the display enter partial mode
        self.write_cmd(0x90)    #resolution setting
        self.write_data(x_start//256) 
        self.write_data(x_start%256)    #x-start    

        self.write_data(x_end//256)     
        self.write_data(x_end%256-1)   #x-end 

        self.write_data(y_start//256)   
        self.write_data(y_start%256)    #y-start    

        self.write_data(y_end//256)     
        self.write_data(y_end%256-1)   #y-end
        self.write_data(0x01)  

        self.write_cmd(0x13)         #writes New data to SRAM.
        for i in range(part_column*part_line//8):       
             self.write_data(datas[i])    
        self.update()  

    #Deep sleep function
    def deepsleep(self):
        self.write_cmd(0X50)   #VCOM AND DATA INTERVAL SETTING     
        self.write_data(0xf7)  #WBmode:VBDF 17|D7 VBDW 97 VBDB 57    WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7  

        self.write_cmd(0X02)    #power off
        self.chkstatus()         #waiting for the electronic paper IC to release the idle signal    
        self.write_cmd(0X07)    #deep sleep
        self.write_data(0xA5) 

    #Partial refresh write address and data
    def dis_part_ram(self,x_start,y_start,datas_A,datas_B,datas_C,datas_D,datas_E,num,part_column,part_line):
        x_end=x_start+part_line*num-1  
        y_end=y_start+part_column-1 

        self.write_cmd(0x50) 
        self.write_data(0xA9) 
        self.write_data(0x07) 

        self.write_cmd(0x91)    #This command makes the display enter partial mode
        self.write_cmd(0x90)    #resolution setting
        self.write_data(x_start//256) 
        self.write_data(x_start%256)    #x-start    

        self.write_data(x_end//256)     
        self.write_data(x_end%256-1)   #x-end 

        self.write_data(y_start//256)   #
        self.write_data(y_start%256)    #y-start    

        self.write_data(y_end//256)     
        self.write_data(y_end%256-1)   #y-end
        self.write_data(0x01)  

        self.write_cmd(0x13)         #writes New data to SRAM.
        for i in range(part_column):     
            for j in range(part_line//8):       
                self.write_data(datas_A[i*part_line//8+j])     
            for j in range(part_line//8):      
                self.write_data(datas_B[i*part_line//8+j])   
            for j in range(part_line//8):       
                self.write_data(datas_C[i*part_line//8+j])     
            for j in range(part_line//8):       
                self.write_data(datas_D[i*part_line//8+j])   
            for j in range(part_line//8):       
                self.write_data(datas_E[i*part_line//8+j])        

    #Clock display
    def dis_part_time(self,x_start,y_start,datas_A,datas_B,datas_C,datas_D,datas_E,num,part_column,part_line):
        self.dis_part_ram(x_start,y_start,datas_A,datas_B,datas_C,datas_D,datas_E,num,part_column,part_line) 
        self.update()
        self.write_cmd(0X92)    #This command makes the display exit partial mode and enter normal mode.
  
    def init_gui(self):
        self.reset()
        self.write_cmd(0x01)      #POWER SETTING
        self.write_data(0x07) 
        self.write_data(0x07)     #VGH=20V,VGL=-20V
        self.write_data(0x3f)    #VDH=15V
        self.write_data(0x3f)    #VDL=-15V

        #Enhanced display drive(Add 0x06 command)
        self.write_cmd(0x06)      #Booster Soft Start 
        self.write_data(0x17) 
        self.write_data(0x17)    
        self.write_data(0x28)    
        self.write_data(0x17)  

        self.write_cmd(0x04)  #POWER ON
        time.sleep(0.1)   
        self.chkstatus()         #waiting for the electronic paper IC to release the idle signal

        self.write_cmd(0X00)      #PANNEL SETTING
        self.write_data(0x1B)    #0X13 KW-3f   KWR-2F BWROTP 0f BWOTP 1f

        self.write_cmd(0x61)          #tres      
        self.write_data(0x03)    #source 800
        self.write_data(0x20) 
        self.write_data(0x01)    #gate 480
        self.write_data(0xE0)   

        self.write_cmd(0X15)    
        self.write_data(0x00)     

        self.write_cmd(0X50)      #VCOM AND DATA INTERVAL SETTING
        self.write_data(0x10)
        self.write_data(0x07) 

        self.write_cmd(0X60)      #TCON SETTING
        self.write_data(0x22)
        