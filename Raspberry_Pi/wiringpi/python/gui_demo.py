from PIL import ImageDraw, Image, ImageFont
import time
from epd_7inch5 import *
from epd_gui import *
from image import *

'''define pin conection in wiringpi number
7.5inch_EPD    Raspberry Pi
      VCC   --->   3.3V
      GND   --->   GND
      RST   --->   P0
      BUSY  --->   P5
      D/C   --->   P6
      MOSI  --->   MOSI
      CLK   --->   CLK
      CS    --->   (CE0)P10 
'''
'''
screen coordinates gor gui functions
-----------------> x (0~800)
|
|
|
|
y(0~480)
'''
if __name__ == '__main__':
    gui = EPD_GUI()  
    gui.epd.init()
    print("show white")
    gui.epd.whitescreen_white()  
    gui.epd.deepsleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s
    print("show image_1")
# 
    gui.epd.init()  # Electronic paper initialization
    gui.epd.whitescreen_all(gImage_1)  # Refresh the picture in full screen
    gui.epd.deepsleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s
    print("show image_2")
    gui.epd.init()  # Electronic paper initialization
    gui.epd.whitescreen_all_fast(gImage_2)  # Refresh the picture in full screen
    gui.epd.deepsleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s
    
    gui.epd.init()  # EPD init
    gui.epd.whitescreen_white()# EPD Clear
    gui.epd.set_Ramvalue_basemap(gImage_basemap)
    gui.epd.init_part();
    for i in range(6):
        gui.epd.dis_part_time(240, 180, Num[1], Num[0], gImage_numdot,Num[0],Num[i],5, 104, 48)
        time.sleep(0.3)
    gui.epd.deepsleep()
    time.sleep(2)
    print("out of time show loop")

    # Drawing
    gui.epd.init_gui()  # Electronic paper initialization
    gui.clear(WHITE)

    font_16 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_16)  # read chinese font file
    font_20 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_20)  # read chinese font file
    font_24 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_24)  # read chinese font file
    font_28 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_28)  # read chinese font file
    # Point
    gui.draw_point(2, 2, BLACK, PIXEL_1X1, DOT_STYLE_DFT)
    gui.draw_point(4, 4, BLACK, PIXEL_2X2, DOT_STYLE_DFT)
    gui.draw_point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT)
    gui.draw_point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT)
    # Line
    gui.draw_line(1, 3, 1, 480, BLACK, PIXEL_1X1, LINE_SOLID)
    gui.draw_line(1, 1, 800, 1, BLACK, PIXEL_1X1, LINE_SOLID)
    # Rectangle
    gui.draw_rectangle(20, 20, 100, 90, BLACK, FILL_EMPTY, PIXEL_1X1)
    gui.draw_rectangle(120, 35, 170, 75, BLACK, FILL_FULL, PIXEL_1X1)
    # Circle
    gui.draw_circle(260, 60, 30, BLACK, FILL_EMPTY, PIXEL_1X1)
    gui.draw_circle(320, 60, 18, BLACK, FILL_FULL, PIXEL_1X1)
    
    gui.draw_str(30, 120, "abcdefg", BLACK, FONT_SIZE_16, font_16)
    gui.draw_str(150,118, "ABCabc012345", BLACK, FONT_SIZE_20, font_20)
    gui.draw_str(50, 140, "7.5\" E-Paper", BLACK, FONT_SIZE_24, font_24)
    gui.draw_str(50, 165, "2 Gray", BLACK, FONT_SIZE_24, font_24)
    gui.draw_str(50, 190, "800 x 400", BLACK, FONT_SIZE_24, font_24)
    gui.draw_str(50, 215, "Black and White", BLACK, FONT_SIZE_24, font_24)
    gui.draw_str(57, 240, "SEENGREAT", BLACK, FONT_SIZE_28, font_28)
    # TEST_PIN = 1
    gui.epd.whitescreen_all(gui.img)  # display image
    gui.epd.deepsleep()  # EPD_DeepSleep, Sleep instruction is necessary, please do not delete!!!
    time.sleep(3)  # 2s
# 
#     # Clear screen
    gui.epd.init()  # EPD init  initialization
    gui.clear(WHITE)  # Show all white
    gui.epd.whitescreen_all(gui.img)
    gui.epd.deepsleep()  # Enter deep sleep, Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)
    print("end")
    exit()
    while True:
        pass
