[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ lcd ](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/LCDPCF.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Installation](#installation)
  * [Test](#test)
  * [Hardware](#hardware)
  * [Software](#software)
  * [Output](#output)

Overview
--------------------
* Name : HD44780_LCD_RPI
* Description :

0. Library to support the HD44780 LCD , (I2C PCF8574 "backpack" interface) 
   for the Raspberry PI.
1. Dynamic install-able system level Raspberry Pi C++ library.
2. Backlight, scroll, cursor and entrymode control.
3. Custom character support + print class for numerical data.
4. Hardware I2C using bcm2835 library
5. Dependency: bcm2835 Library
6. Tested on size 16x02 and 20x04 (but may work on other sizes eg 16x04 , untested)

* Author: Gavin Lyons
* Developed on 
	1. Raspberry PI 3 model b
	2. C++, g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
	3. Raspbian 10  stretch OS, armv7l Linux 5.10.63-v7+ 
	4. bcm2835 Library 1.68 


Installation
------------------------------

1. Make sure I2C bus is enabled on your raspberry PI

2. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.68.)
	* The bcm2835 library is a dependency and provides low level I2C bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

3. Download the HD44780_LCD_RPI library 
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.
    
```sh
curl -sL https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/archive/1.3.tar.gz | tar xz
```

4. Run "make" to run the makefile in repo base folder to install library, it will be 
    installed to usr/lib and usr/include
    
```sh
cd HD44780_LCD_RPI-1.3
sudo make
```

Test 
-----------------------------

1. Next step is to test LCD and installed library with the example test file.
Wire up your LCD. Next enter the examples folder and run the makefile in THAT folder, 
This 2nd makefile builds the example file using the just installed library.
and creates a test exe file in "bin". 

```sh
cd examples/
make
make run
```

2. There are 3 examples files. 
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder.
in the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY.
Comment out the rest and repeat: make & make run.

| Filepath | File Function | Screen Size |
| ---- | ---- | ---- | 
| src/TEST_16x02 | Carries out test sequence testing features | 16x02 |
| src/TEST_20x04 | Carries out test sequence testing features | 20x04 |
| src/CLOCK_16x02 | A basic clock Demo | 16x02 |

Hardware
----------------------------

Connections 

1. LCD SCLK = SCLK1 I2C pins P1-05 GPIO3
2. LCD SDA = SDA1 I2C pins P1-03 GPIO2

Software 
-------------------------

*I2C*

Hardware I2C.
Clock rate Settings and I2C Address is set in the constructor in main.cpp.

1. I2C LCD Slave Address is set to 0x27 by default(your module could be different).
2. I2C Clock rate can be a passed into in the constructor as a argument, five possible values : 

| Value | Method | I2C speed | 
| ---- | ---- | ---- | 
| 0 (default) | bcm2835_i2c_set_baudrate(100000) | 100Khz | 
| 2500 | bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500) | 100Khz | 
| 626 | bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626) | 399.4 kHz | 
| 150 | bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_150) | 1.666 MHz | 
| 148 | bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_148) | 1.689 MHz | 

For more info on bcm2835I2CClockDivider & bcm2835I2CReasonCodes see [bcm2835 doc's for details](http://www.airspayce.com/mikem/bcm2835/group__constants.html)

*Debug*

User can turn on debug messages with PCF8574_DebugSet method see example file.


Output
---------------------

Output  of custom character test in test file.

![ pic ](https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/blob/main/extras/image/custom_output.jpg)

20x04 display. 

![ pic2 ](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574/blob/main/extras/image/2004.jpg)
