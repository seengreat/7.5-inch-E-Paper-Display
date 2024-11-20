7.5 inch E-Paper Display from seengreat:www.seengreat.com
 =======================================
# Introduction  
![image](https://github.com/seengreat/7.5-inch-E-Paper-Display/blob/main/01.png )<br>   
   
## 1.1 Product Overview
This product is a 7.5-inch black-and-white e-ink display expansion module with a screen resolution of 800x480. It uses SPI communication and supports both full and partial refresh modes. The driver board is designed based on the 40-pin interface of Raspberry Pi, making it compatible with Raspberry Pi series boards. We provide example programs in C and Python for Raspberry Pi. A reserved SPI control interface allows easy integration with main control boards such as Arduino, STM32, and ESP32. We also offer example code for Arduino, STM32, and ESP32, which supports displaying images, English and numerical characters, and drawing points, lines, rectangles, and circles.<br>
## 1.2 Product Specifications
|Pixels	|800 x 480|
|-----------|----------|
|Display Colors	|Monochrome (Black and White)|
|Gray Levels	|2|
|Level-Shifting Chip	|TXS0108EPWR|
|Signal Interface	|SPI|
|Power Supply Voltage	|3.3V/5V|
|Display Area	|163.2mm x 97.9mm|
|Partial Refresh|0.3s (4-wire SPI mode)|
|Full Refresh	|3s (4-wire SPI mode)|

## 1.3 Interface Definition
|VCC	|3.3V/5V|
|----------------------|------------------------------------|
|GND	|Power ground|
|RST	|External reset pin (low level reset)|
|BUSY	|Busy status output pin|
|D/C	|Data/Command control pin (high level for data, low level for command)|
|MOSI	|SPI communication MOSI pin|
|CS	|SPI chip select pin (active low)|
|CLK	|SPI communication SCK pin|
|LED1_P	|LED1 control pin|
|LED2_P	|LED2 control pin|
|K1_P	|Button K1 control pin|
|K2_P	|Button K2 control pin|
# Usage
All example programs provided for this product are based on the 4-wire SPI mode, so the BS selection switch on the board is defaulted to the "0" position. Additionally, the RESE switch is defaulted to position A; setting it to position B may result in abnormal display.<br>
## 2.1 Resource Overview
The overview of the driver resources is shown in the figure below:<br>
![image](https://github.com/seengreat/7.5-inch-E-Paper-Display/blob/main/02.png )<br> 
                           Figure 2-1 Driver Board Resource Overview<br>
① E-paper screen matching resistor selection switch(default is "A" position)<br>
② E-paper screen connector<br>
③ SPI lines count selection switch(default is "0" position)<br>
④ Two LEDs<br>
⑤ Two buttons<br>
⑥ SPI control interface connector<br>
⑦ Control pins for LEDs and buttons<br>
⑧ Level-shifting chip TXS0108<br>
⑨ Raspberry Pi 40-pin GPIO interface<br>


## 2.2 Raspberry Pi Example Usage
The example code for the Raspberry Pi platform is divided into two versions: lgpio and wiringpi. The bookworm system uses the lgpio library, while the bullseye system uses the wiringpi library.<br>
### 2.2.1 Raspberry Pi Platform Interface Definition

The example programs for the Raspberry Pi board use the BCM numbering for pin definitions with the lgpio library and the wiringpi numbering with the wiringpi library. The pin wiring definitions for the Raspberry Pi board are as shown in the table below:<br>
|E-paper Interface (E-paper Connector)	|Pin Function	|BCM Numbering	|wiringpi Numbering|
|----------------------|------------------------------------|------------------------------|-------------------------|
|VCC	|3.3V	|	|	|		
|GND	|GND	|	|	|
|BUSY	|Busy Signal Pin	|24|	5|
|RSTN	|Reset	|17	|0	|
|D/C	|Command/Data Select	|25|	6|
|SDA	|SPI Data Out/Slave In	|10|	12|
|SCL	|SPI Clock		|	11|	14|
|CSB	|SPI Chip Select	|	8|	10|
|	|LED1		|	4|	7|
|	|LED2		|	18|	1|
|	|K1		|	27|	2|
|	|K2		|	22|	3|<br>

	Table 2-1 Driver Board and Raspberry Pi Pin Definitions<br>
### 2.2.2  wiringpi Library Installation
```sudo apt-get install wiringpi```<br>

```wget https://project-downloads.drogon.net/wiringpi-latest.deb ```#
Upgrade Raspberry Pi 4B Version<br>
```sudo dpkg -i wiringpi-latest.deb```<br>
```gpio -v ```<br>
If version 2.52 appears, it indicates that the installation has been successful.<br>
For the Bullseye branch system, use the following command:<br>
```git clone https://github.com/WiringPi/WiringPi```<br>
```cd WiringPi```<br>
```. /build``` <br>
```gpio -v ```#<br>
Running "gpio -v" will show version 2.70. If it doesn't appear, it means there is an installation error.<br>
If an error message "ImportError: No module named 'wiringpi'" appears when running a sample program in Python version, run the following command:<br>
For Python 2.x version<br>
```pip install wiringpi```<br>
For Python 3.x version<br>
```pip3 install wiringpi```<br>
Note: If the installation fails, you can try the following compiled installation.<br>
git clone --recursive https://github.com/WiringPi/WiringPi-Python.git
Note: The --recursive option can automatically pull submodules. Otherwise, you need to download them manually.
Enter the newly downloaded WiringPi-Python folder and use the following commands to compile and install:<br>
For Python 2.x version<br>
```sudo python setup.py install```<br>
For Python 3.x version<br>
```sudo python3 setup.py install```<br>
If the following error occurs:<br>
``Error: Building this module requires either that swig is installed``<br>
``(e.g.,'sudo apt install swig')or that wiringpi wrap.c from the``<br>
``source distribution(on pypi) is available.``<br>
In this case, enter the command sudo apt install swig to install SWIG. After completing the installation, run sudo python3 setup.py install to compile and install. If you see similar information, it indicates that the installation was successful.<br>
``ges``<br>
``Adding wiringpi 2.60.0 to easy-install.pth file``<br>
``Installed /usr/local/lib/python3.7/dist-packages/wiringpi-2.60.0-py3.7-linux-armv7``<br>
``Processing dependencies for wiringpi==2.60.0``<br>
``Finished processing dependencies for wiringpi==2.60.0``<br>


### 2.2.3 lgpio Library Installation
For the Bookworm system, the example programs use the lgpio library. The installation command for this library is:<br>
```wget https://github.com/joan2937/lg/archive/master.zip```<br>
```unzip master.zip```<br>
```cd lg-master```<br>
```make```<br>
```sudo make install```<br>
### 2.2.4 Enable the SPI Interface
```sudo raspi-config```<br>
Enable the SPI Interface<br>
Interfacing Options -> SPI -> Yes<br>
Check the enabled SPI devices:<br>
```Is /dev/spi* ```#<br>
This will print: “/dev/spidev0.0” and “/dev/spidev0.1”<br>
### 2.2.5 Python Library Installation
The example program uses the Python 3 environment. To run the Python example program, you need to install the PIL, numpy, and spidev libraries. Enter the following commands to install them:<br>
```sudo apt-get install python3-pil```<br>
```sudo apt-get install python3-numpy```<br>
```sudo apt-get install python3-pip```<br>
```sudo pip3 install spidev```<br>
### 2.2.6 C Version Example
Using the lgpio library as an example, navigate to the /demo codes/raspberry_pi/lgpio/c directory:<br>
```sudo make clean```<br>
```sudo make```<br>
```sudo ./main```<br>
After entering the above commands, you can observe the e-paper display.<br>
### 2.2.7 Python Version Example
Using the lgpio library as an example, navigate to the /demo codes/raspberry_pi/lgpio/python directory.<br>
```sudo python3 gui_demo.py```<br>
After entering the above commands, you can observe the e-paper display.<br>
## 2.3 Arduino Example Usage
### 2.3.1 Hardware Interface Configuration
Table 2-2 shows the wiring definitions between the Arduino Mega and the e-paper display.<br>
|E-paper Interface	|Arduino Mega|
|----------------------|----------------|
|VCC|	5V|
|GND|	GND|
|CS|	D53|
|CLK|	D52|
|MOSI|	D51|
|DC|	D8|
|RST|	D9|
|BUSY|	D10|<br>

Table 2-2 E-paper Display and Arduino UNO Pin Definitions<br><br>

|E-paper Interface	|Arduino UNO|
|----------------------|----------------|
|VCC	|5V|
|GND	|GND|
|CS	|D7|
|CLK	|D13|
|MOSI	|D11|
|DC	|D10|
|RST	|D9|
|BUSY	|D8|<br>

Table 2-3 E-paper Display and Arduino UNO Pin Definitions<br>
### 2.3.2 Example Usage
Open the project file /demo codes/Arduino_MEGA/Arduino_MEGA.ino or /demo codes/Arduino_UNO/Arduino_UNO.ino in the Arduino IDE. Select the correct Board and Port based on your setup. Click Verify, and once the verification is successful, upload it to the module. Observe the e-paper display.<br>
<br>
## 2.4 STM32 Example Usage
The example code is based on STM32F103C8T6.<br>
### 2.4.1 Hardware Interface Configuration
|E-paper Interface	|STM32 Interface|
|----------------------|----------------|
|VCC	|3.3V|
|GND	|GND|
|CS	|PB12|
|CLK	|PB13|
|MOSI	|PB15|
|DC	|PA8|
|RST	|PA11|
|BUSY	|PA12|<br>

### 2.4.2 Example Usage
Open the example in the directory /demo codes/STM32 using Keil uVision5 software. After compiling without errors, download it to the module and observe the e-paper display.<br>
## 2.5 ESP32 Example Usage
The example program uses the ESP32 module ESP32-WROOM-32E.<br>
### 2.5.1 Hardware Interface Configuration
|E-paper Interface	|ESP32 Interface|
|----------------------|----------------|
|VCC	|3.3V|
|GND	|GND|
|CS	|IO27|
|CLK	|IO18|
|MOSI	|IO23|
|DC	|IO14|
|RST	|IO33|
|BUSY	|IO13|
<br>

### 2.5.2 Example Usage

Open the project file "/demo codes /Arduino_ESP32" using the Arduino IDE. Select the correct Board and Port based on your setup. Click Verify, and once the verification is successful, upload it to the module and observe the e-paper display.<br>


__Thank you for choosing the products of Shengui Technology Co.,Ltd. For more details about this product, please visit:
www.seengreat.com__
