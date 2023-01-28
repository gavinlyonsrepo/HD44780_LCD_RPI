/*
 * File: main.cpp
 * Description: 
 * This file contains the "main" function for a clock demo (16x02 LCD)
 * to test the HD44780 LCD library, see issue one github
 * Author: Gavin Lyons.
 * Compiler: C++ g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
 * Tested: Raspbian 10, armv7l Linux 5.10.63-v7+ , RPI M3B Rev 1.2
 * Created : Jan 2023
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/HD44780_LCD_RPI
 * 
 * Notes: 
 * 
 * (1) set PCF8574_DebugSet(false) to true in Setup() for debug
 * 
 * (2) For description of entry modes , cursor types, custom characters 
 * and more see here http://dinceraydin.com/lcd/commands.htm
 * 
 * (3) -std=c++2a required in makefile
 * 
 * (4) press ctrl + c to quit
 */

// Section: Included library 
#include <iostream>
#include <ctime>
#include <bcm2835.h>
#include <signal.h> //catch user Ctrl+C
#include "HD44780_LCD.hpp"

// Section: Defines
#define DISPLAY_DELAY_1 1000

// Section: Globals
// myLCD(rows , cols , PCF8574 I2C address, I2C speed)
HD44780LCD myLCD( 2, 16, 0x27, 626); // instantiate an object

// Section: Function Prototypes
void setup(void);
void DisplayInfo(void);
void endTest(void);
std::string UTC_string(void);
void signal_callback_handler(int signum);

// Section: Main Loop

int main(int argc, char **argv)
{
	signal(SIGINT, signal_callback_handler);
	if(!bcm2835_init())
	{
		std::cout << "Library bcm2835 failed init" << std::endl ;
		return -1;
	}
	
	setup();
	while(1)
	{
		DisplayInfo();
		bcm2835_delay(DISPLAY_DELAY_1);
	} 
	endTest();
	
	return 0;
} // End of main

// Section :  Functions

void setup(void) 
{
	std::cout <<  "LCD Begin" << std::endl;  
	myLCD.PCF8574_LCD_I2C_ON();
	bcm2835_delay(DISPLAY_DELAY_1);
	myLCD.PCF8574_LCDInit(LCDCursorTypeOn);
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_DebugSet(false); // Set to true to turn on debug mode
}

void DisplayInfo(void) 
{
	std::string TimeString = UTC_string();
	std::cout<< TimeString << "\r" << std::flush;
	auto timeInfo = TimeString.substr(0, 10);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
	myLCD.print(timeInfo);
	auto DateInfo = TimeString.substr(11);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
	myLCD.print(DateInfo);
}


void endTest()
{
	myLCD.PCF8574_LCDDisplayON(false); //Switch off display
	myLCD.PCF8574_LCD_I2C_OFF();
	bcm2835_close(); // Close the library
	std::cout << std::endl;
	std::cout << "LCD End" << std::endl ;
}

//Return UTC time as a std:.string with format "yyyy-mm-dd hh:mm:ss".
std::string UTC_string() 
{
	std::time_t time = std::time({});
	char timeString[std::size("yyyy-mm-dd hh:mm:ss UTC")];
	std::strftime(std::data(timeString), std::size(timeString), "%F %T UTC", std::gmtime(&time));
	return timeString;
}

// Terminate program on ctrl + C 
void signal_callback_handler(int signum)
{
	endTest();
	exit(signum);
}
// *** EOF ***

