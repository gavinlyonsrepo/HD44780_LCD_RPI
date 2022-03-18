
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
2. Backlight, Scroll, Cursor control.
3. Custom character support + print class for numerical data.
4. Hardware I2C using bcm2835 library
5. Dependency: bcm2835 Library
6. Tested on size 16x02 but may work on other sizes (20x4 8X2 etc)

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
curl -sL https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/archive/1.1.tar.gz | tar xz
```

4. Run "make" to run the makefile in repo base folder to install library, it will be 
    installed to usr/lib and usr/include
    
```sh
cd HD44780_LCD_RPI-1.1
sudo make
```

Test 
-----------------------------

1. Next step is to test LCD and installed library with the example test file.
Wire up your LCD. Next enter the examples folder and run the makefile in THAT folder, 
This 2nd makefile builds the example file using the just installed library.
and creates a test exe file in "bin". Be sure to use "sudo" as the bcm2835 requires root permissions by default [ see here for more details on that](http://www.airspayce.com/mikem/bcm2835/) 


```sh
cd examples/
make
sudo bin/test
```

Hardware
----------------------------

Connections 

1. LCD SCLK = SCLK1 I2C pins P1-05 GPIO3
2. LCD SDA = SDA1 I2C pins P1-03 GPIO2

Software 
-------------------------

*I2C*

Hardware I2C.
Clock rate Settings are in the "PCF8574_LCD_I2C_ON" method.
Address is passed in constructor main.cpp.

1. I2C LCD Slave Address is set to 0x27 by default(your module could be different).
2. Clock rate is to BCM2835_I2C_CLOCK_DIVIDER_626   
	This can be increased if necessary to BCM2835_I2C_CLOCK_DIVIDER_148
	for more faster bus. See [bcm2835 for details](http://www.airspayce.com/mikem/bcm2835/) 
3. Baudrate method can also be used. 

*Debug*

User can turn on debug messages with PCF8574_DebugSet method see example file.

Output
---------------------

Output  of custom character test in test file.

![ pic ](https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/blob/master/extras/image/custom_output.jpg

