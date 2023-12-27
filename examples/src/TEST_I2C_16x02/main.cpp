/*!
	@file    main.cpp
	@author   Gavin Lyons
	@brief 
		 This file contains the "main" function for  project, I2C test
 		 to test the HD44780_LCD_RPI library

	@note 
		-# Test 901 :: I2c Test
*/

// Section: Included library
#include <iostream>
#include <bcm2835.h>
#include "HD44780_LCD.hpp"


// Section: Globals
// myLCD(rows , cols , PCF8574 I2C address, I2C speed)
HD44780PCF8574LCD myLCD(2, 16, 0x27, 0); // instantiate an object

// Section: Function Prototypes
bool setup(void);
void helloWorld(void);
void endTest(void);

// Section: Main Loop
int main(int argc, char **argv)
{
	if (!setup()) return -1;
	helloWorld();
	endTest();
	return 0;
} 

// Section :  Functions

bool setup(void) {
	std::cout <<  "LCD Test Begin" << std::endl;
	
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		std::cout << "Error 1201: init bcm2835 library , Is it installed ?"  << std::endl;
		return false;
	}

	// Turn on I2C bus (optionally it may already be on)
	if (!myLCD.LCD_I2C_ON())
	{
		std::cout << "Error 1202: bcm2835_i2c_begin :Cannot start I2C, Running as root?" << std::endl;
		return false;
	}
	
	bcm2835_delay(1000);
	myLCD.LCDDebugSet(true); // Turn debug messages on 
	myLCD.LCD_I2C_SetSpeed();
	if (myLCD.LCDCheckConnection() != 0)
	{
		std::cout << "Error 1203: bcm2835_i2c_begin :LCD not on bus?" << std::endl;
		return false;
	}
	
	// print out library versions & flag status( Note optional)
	std::cout << "bcm2835 library Version Number :" << bcm2835_version() << std::endl;
	std::cout << "HD44780_LCD_RPI lib Version Num :"  << myLCD.LCDVerNumGet() << std::endl;
	std::cout << "Debug status is : " << (myLCD.LCDDebugGet() ? "On" : "Off") << std::endl ;
	std::cout <<  "Backlight status is : " << (myLCD.LCDBackLightGet() ? "On" : "Off") << std::endl ;
	std::cout << "I2C Debug Error : " << myLCD.LCDI2CErrorGet() << std::endl; // Print I2C error flag
	std::cout << "I2C Error Timeout mS : " << myLCD.LCDI2CErrorTimeoutGet() << std::endl; // Print I2C Timeout
	
	myLCD.LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.LCDClearScreen();
	return true;
}

void helloWorld(void) 
{
	char testString[] = "I2C Test";
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.LCDSendString(testString);
	bcm2835_delay(5000);
}


void endTest()
{
	myLCD.LCDDisplayON(false); //Switch off display
	myLCD.LCD_I2C_OFF(); // Switch off I2C , optional.
	bcm2835_close(); // Close the library
	std::cout << "LCD Test End" << std::endl ;
}

// *** EOF ***

